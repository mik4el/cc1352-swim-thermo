/*
 * Copyright (c) 2018, Texas Instruments Incorporated
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

/*
 * ======== CC1352R1_LAUNCHXL_fxns.c ========
 *  This file contains the board-specific initialization functions, and
 *  RF callback function for antenna switching.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <ti/devices/cc13x2_cc26x2_v1/driverlib/ioc.h>
#include <ti/devices/cc13x2_cc26x2_v1/driverlib/cpu.h>
#include <ti/drivers/rf/RF.h>
#include <ti/drivers/pin/PINCC26XX.h>

#include "Board.h"

/*
 *  ======== Board_initHook ========
 *  Called by Board_init() to perform board-specific initialization.
 */
void Board_initHook()
{
}

/*
 *  For the SysConfig generated Board.h file, Board_RF_SUB1GHZ will not be
 *  defined unless the RF module is added to the configuration.  Therefore,
 *  we don't include this code if Board_RF_SUB1GHZ is not defined.
 */
#if defined(Board_RF_SUB1GHZ)

/*
 * ======== rfDriverCallback ========
 * This is an implementation for the CC1352R1 launchpad which uses a
 * single signal for antenna switching.
 */
void rfDriverCallback(RF_Handle client, RF_GlobalEvent events, void *arg)
{
    (void)client;
    RF_RadioSetup* setupCommand = (RF_RadioSetup*)arg;

    if ((events & RF_GlobalEventRadioSetup) &&
            (setupCommand->common.commandNo == CMD_PROP_RADIO_DIV_SETUP)) {
        /* Sub-1 GHz, requires antenna switch high */
        PINCC26XX_setOutputValue(Board_RF_SUB1GHZ, 1);
    }
    else if (events & RF_GlobalEventRadioPowerDown) {
        /* Disable antenna switch to save current */
        PINCC26XX_setOutputValue(Board_RF_SUB1GHZ, 0);
    }
}
#endif
