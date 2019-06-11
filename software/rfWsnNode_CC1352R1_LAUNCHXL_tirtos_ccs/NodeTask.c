/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/***** Includes *****/
/* XDCtools Header files */ 
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */ 
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Clock.h>

/* TI-RTOS Header files */ 
#include <ti/drivers/PIN.h>
#include <ti/display/Display.h>
#include <ti/display/DisplayExt.h>

/* Board Header files */
#include "Board.h"

/* Application Header files */ 
#include "SceAdc.h"
#include "NodeTask.h"
#include "NodeRadioTask.h"

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/aon_batmon.h)
#include DeviceFamily_constructPath(driverlib/aux_adc.h) //for ADC operations

/***** Defines *****/
#define NODE_TASK_STACK_SIZE 1024
#define NODE_TASK_PRIORITY   3

#define NODE_EVENT_ALL                  0xFFFFFFFF
#define NODE_EVENT_NEW_ADC_VALUE    (uint32_t)(1 << 0)
#define NODE_EVENT_UPDATE_LCD       (uint32_t)(1 << 1)

/* A change mask of 0xFF0 means that changes in the lower 4 bits does not trigger a wakeup. */
#define NODE_ADCTASK_CHANGE_MASK                    0xFF0

/* Minimum slow Report interval is 50s (in units of samplingTime)*/
#define NODE_ADCTASK_REPORTINTERVAL_SLOW                50
/* Minimum fast Report interval is 1s (in units of samplingTime) for 30s*/
#define NODE_ADCTASK_REPORTINTERVAL_FAST                5
#define NODE_ADCTASK_REPORTINTERVAL_FAST_DURIATION_MS   30000

#define NUM_EDDYSTONE_URLS      5

/***** Variable declarations *****/
static Task_Params nodeTaskParams;
Task_Struct nodeTask;    /* Not static so you can see in ROV */
static uint8_t nodeTaskStack[NODE_TASK_STACK_SIZE];
Event_Struct nodeEvent;  /* Not static so you can see in ROV */
static Event_Handle nodeEventHandle;
static uint16_t latestAdcValue;
static int32_t latestInternalTempValue;
static uint16_t latestBatt;

/* Pin driver handle */
static PIN_Handle buttonPinHandle;
static PIN_Handle ledPinHandle;
static PIN_State buttonPinState;
static PIN_State ledPinState;

/* Display driver handles */
static Display_Handle hDisplayLcd;

/* Enable the 3.3V power domain used by the LCD */
PIN_Config pinTable[] = {
    NODE_ACTIVITY_LED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

/*
 * Application button pin configuration table:
 *   - Buttons interrupts are configured to trigger on falling edge.
 */
PIN_Config buttonPinTable[] = {
    Board_PIN_BUTTON0  | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
#ifdef FEATURE_BLE_ADV
    Board_PIN_BUTTON1  | PIN_INPUT_EN | PIN_PULLUP | PIN_IRQ_NEGEDGE,
#endif
    PIN_TERMINATE
};

static uint8_t nodeAddress = 0;

#ifdef FEATURE_BLE_ADV
static BleAdv_Stats bleAdvStats = {0};
#endif

/***** Prototypes *****/
static void nodeTaskFunction(UArg arg0, UArg arg1);
static void updateLcd(void);
static void adcCallback(uint16_t adcValue);
static void buttonCallback(PIN_Handle handle, PIN_Id pinId);

/***** Function definitions *****/
void NodeTask_init(void)
{
    /* Create event used internally for state changes */
    Event_Params eventParam;
    Event_Params_init(&eventParam);
    Event_construct(&nodeEvent, &eventParam);
    nodeEventHandle = Event_handle(&nodeEvent);

    /* Create the node task */
    Task_Params_init(&nodeTaskParams);
    nodeTaskParams.stackSize = NODE_TASK_STACK_SIZE;
    nodeTaskParams.priority = NODE_TASK_PRIORITY;
    nodeTaskParams.stack = &nodeTaskStack;
    Task_construct(&nodeTask, nodeTaskFunction, &nodeTaskParams, NULL);
}

#ifdef FEATURE_BLE_ADV
void NodeTask_advStatsCB(BleAdv_Stats stats)
{
    memcpy(&bleAdvStats, &stats, sizeof(BleAdv_Stats));
}
#endif

static void nodeTaskFunction(UArg arg0, UArg arg1)
{
    /* Initialize display and try to open both UART and LCD types of display. */
    Display_Params params;
    Display_Params_init(&params);
    params.lineClearMode = DISPLAY_CLEAR_BOTH;

    /* Open both an available LCD display and an UART display.
     * Whether the open call is successful depends on what is present in the
     * Display_config[] array of the board file.
     *
     * Note that for SensorTag evaluation boards combined with the SHARP96x96
     * Watch DevPack, there is a pin conflict with UART such that one must be
     * excluded, and UART is preferred by default. To display on the Watch
     * DevPack, add the precompiler define BOARD_DISPLAY_EXCLUDE_UART.
     */
    hDisplayLcd = Display_open(Display_Type_LCD, &params);

    /* Check if the selected Display type was found and successfully opened */
    if (hDisplayLcd)
    {
        Display_printf(hDisplayLcd, 0, 0, "Waiting for ADC...");
    }

    /* Open LED pins */
    ledPinHandle = PIN_open(&ledPinState, pinTable);
    if (!ledPinHandle)
    {
        System_abort("Error initializing board 3.3V domain pins\n");
    }

    /* Start the SCE ADC task with 1s sample period and reacting to change in ADC value. */
    SceAdc_init(0x00010000, NODE_ADCTASK_REPORTINTERVAL_FAST, NODE_ADCTASK_CHANGE_MASK);
    SceAdc_registerAdcCallback(adcCallback);
    SceAdc_start();

    buttonPinHandle = PIN_open(&buttonPinState, buttonPinTable);
    if (!buttonPinHandle)
    {
        System_abort("Error initializing button pins\n");
    }

    /* Setup callback for button pins */
    if (PIN_registerIntCb(buttonPinHandle, &buttonCallback) != 0)
    {
        System_abort("Error registering button callback function");
    }

    while (1)
    {
        /* Wait for event */
        uint32_t events = Event_pend(nodeEventHandle, 0, NODE_EVENT_ALL, BIOS_WAIT_FOREVER);

        /* If new ADC value, send this data */
        if (events & NODE_EVENT_NEW_ADC_VALUE) {
            /* Toggle activity LED */
            PIN_setOutputValue(ledPinHandle, NODE_ACTIVITY_LED,!PIN_getOutputValue(NODE_ACTIVITY_LED));

            /* Send ADC value to concentrator */
            NodeRadioTask_sendAdcData(latestAdcValue);

            /* Update display */
            updateLcd();
        }
        /* If new ADC value, send this data */
        if (events & NODE_EVENT_UPDATE_LCD) {
            /* update display */
            updateLcd();
        }
    }
}

static void updateLcd(void)
{
#ifdef FEATURE_BLE_ADV
    char advMode[16] = {0};
#endif

    /* get node address if not already done */
    if (nodeAddress == 0)
    {
        nodeAddress = nodeRadioTask_getNodeAddr();
    }

    double tempFormatted = FIXED2DOUBLE(FLOAT2FIXED(convertADCToTempDouble(latestAdcValue)));
    if (tempFormatted > 128.0) {
        tempFormatted = tempFormatted - 256.0; //display negative temperature correct
    }
    double internalTempFormatted = latestInternalTempValue;
    if (internalTempFormatted > 128.0) {
        internalTempFormatted = internalTempFormatted - 256.0; //display negative temperature correct
    }

    /* print to LCD */
    Display_clear(hDisplayLcd);
    Display_printf(hDisplayLcd, 0, 0, "NodeID: 0x%02x", nodeAddress);
    Display_printf(hDisplayLcd, 1, 0, "ADC: %04d", latestAdcValue);

    Display_printf(hDisplayLcd, 2, 0, "TempA: %3.3f", tempFormatted);
    Display_printf(hDisplayLcd, 3, 0, "TempI: %3.3f", internalTempFormatted);
    Display_printf(hDisplayLcd, 4, 0, "Batt: %i", latestBatt);

#ifdef FEATURE_BLE_ADV
    if (advertisementType == BleAdv_AdertiserMs)
    {
         strncpy(advMode, "BLE MS", 6);
    }
    else if (advertisementType == BleAdv_AdertiserUrl)
    {
         strncpy(advMode, "Eddystone URL", 13);
    }
    else if (advertisementType == BleAdv_AdertiserUid)
    {
         strncpy(advMode, "Eddystone UID", 13);
    }
    else
    {
         strncpy(advMode, "None", 4);
    }

    /* print to LCD */
    Display_printf(hDisplayLcd, 6, 0, "Adv Mode:");
    Display_printf(hDisplayLcd, 7, 0, "%s", advMode);
    Display_printf(hDisplayLcd, 8, 0, "Adv successful | failed");
    Display_printf(hDisplayLcd, 9, 0, "%04d | %04d",
                   bleAdvStats.successCnt + bleAdvStats.failCnt);
#endif
}

static void adcCallback(uint16_t adcValue)
{
    /* Calibrate and save latest values */
    uint32_t calADC12_gain = AUXADCGetAdjustmentGain(AUXADC_REF_FIXED);
    int8_t calADC12_offset = AUXADCGetAdjustmentOffset(AUXADC_REF_FIXED);
    latestAdcValue = AUXADCAdjustValueForGainAndOffset(adcValue, calADC12_gain, calADC12_offset);
    latestInternalTempValue = AONBatMonTemperatureGetDegC();
    latestBatt = (AONBatMonBatteryVoltageGet() * 125) >> 5;
    /* Post event */
    Event_post(nodeEventHandle, NODE_EVENT_NEW_ADC_VALUE);
}

/*
 *  ======== buttonCallback ========
 *  Pin interrupt Callback function board buttons configured in the pinTable.
 */
static void buttonCallback(PIN_Handle handle, PIN_Id pinId)
{
    /* Debounce logic, only toggle if the button is still pushed (low) */
    CPUdelay(8000*50);

#ifdef FEATURE_BLE_ADV
    if (PIN_getInputValue(Board_PIN_BUTTON1) == 0)
    {
        if (advertisementType == BleAdv_AdertiserUrl)
        {
            advertisementType = BleAdv_AdertiserNone;
        } else {
            advertisementType = BleAdv_AdertiserUrl;
        }

        //Set advertisement type
        BleAdv_setAdvertiserType(advertisementType);

        /* update display */
        Event_post(nodeEventHandle, NODE_EVENT_UPDATE_LCD);
    }
#endif
}

#ifdef FEATURE_BLE_ADV
void rfSwitchCallback(RF_Handle h, RF_ClientEvent event, void* arg){
#if defined(Board_DIO30_SWPWR)
    //Turn on switch
    PIN_setOutputValue(blePinHandle, Board_DIO30_SWPWR, 1);
#endif
    PIN_setOutputValue(blePinHandle, RF_SWITCH_PIN, 1);
}
#endif
