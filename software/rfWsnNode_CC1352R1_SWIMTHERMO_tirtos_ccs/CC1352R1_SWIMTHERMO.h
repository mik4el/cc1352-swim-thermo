/*
 * Copyright (c) 2017-2018, Texas Instruments Incorporated
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
/** ===========================================================================
 *  @file       CC1352R1_SWIMTHERMO.h
 *
 *  @brief      CC1352R1_SWIMTHERMO Board Specific header file.
 *
 *  The CC1352R1_SWIMTHERMO header file should be included in an application as
 *  follows:
 *  @code
 *  #include "CC1352R1_SWIMTHERMO.h"
 *  @endcode
 *
 *  ===========================================================================
 */
#ifndef __CC1352R1_SWIMTHERMO_BOARD_H__
#define __CC1352R1_SWIMTHERMO_BOARD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include <ti/drivers/PIN.h>
#include <ti/devices/cc13x2_cc26x2_v1/driverlib/ioc.h>

/* Externs */
extern const PIN_Config BoardGpioInitTable[];

/* Defines */
#define CC1352R1_SWIMTHERMO

/* Mapping of pins to board signals using general board aliases
 *      <board signal alias>         <pin mapping>   <comments>
 */

/* Mapping of pins to board signals using general board aliases
 *      <board signal alias>                  <pin mapping>
 */

/* CC1352R1_SWIMTHERMO IO v1.1 */
#define CC1352R1_SWIMTHERMO_SWITCH              IOID_5 // IOID_8 in HW v1.0
#define CC1352R1_SWIMTHERMO_T_ON_1              IOID_6  // IOID_9 in HW v1.0.
#define CC1352R1_SWIMTHERMO_T_ON_2              IOID_7  // IOID_11 in HW v1.0.
#define CC1352R1_SWIMTHERMO_TAO_1               IOID_24 // IOID_10 in HW v1.0.
#define CC1352R1_SWIMTHERMO_TAO_2               IOID_23 // IOID_12 in HW v1.0.
#define CC1352R1_SWIMTHERMO_LED_EASYSET         IOID_13
#define CC1352R1_SWIMTHERMO_PSU_ENABLE          IOID_14

/* SPI Board */
#define CC1352R1_SWIMTHERMO_SPI0_MISO             PIN_UNASSIGNED
#define CC1352R1_SWIMTHERMO_SPI0_MOSI             CC1352R1_SWIMTHERMO_LED_EASYSET
#define CC1352R1_SWIMTHERMO_SPI0_CLK              PIN_UNASSIGNED
#define CC1352R1_SWIMTHERMO_SPI0_CSN              PIN_UNASSIGNED
#define CC1352R1_SWIMTHERMO_SPI1_MISO             PIN_UNASSIGNED
#define CC1352R1_SWIMTHERMO_SPI1_MOSI             PIN_UNASSIGNED
#define CC1352R1_SWIMTHERMO_SPI1_CLK              PIN_UNASSIGNED
#define CC1352R1_SWIMTHERMO_SPI1_CSN              PIN_UNASSIGNED

/*!
 *  @brief  Initialize the general board specific settings
 *
 *  This function initializes the general board specific settings.
 */
void CC1352R1_SWIMTHERMO_initGeneral(void);


/*!
 *  @def    CC1352R1_SWIMTHERMO_ADCBufName
 *  @brief  Enum of ADCs
 */
typedef enum CC1352R1_SWIMTHERMO_ADCBufName {
    CC1352R1_SWIMTHERMO_ADCBUF0 = 0,

    CC1352R1_SWIMTHERMO_ADCBUFCOUNT
} CC1352R1_SWIMTHERMO_ADCBufName;

/*!
 *  @def    CC1352R1_SWIMTHERMO_ADCBuf0SourceName
 *  @brief  Enum of ADCBuf channels
 */
typedef enum CC1352R1_SWIMTHERMO_ADCBuf0ChannelName {
    CC1352R1_SWIMTHERMO_ADCBUF0CHANNEL0 = 0,
    CC1352R1_SWIMTHERMO_ADCBUF0CHANNEL1,
    CC1352R1_SWIMTHERMO_ADCBUF0CHANNELVDDS,
    CC1352R1_SWIMTHERMO_ADCBUF0CHANNELDCOUPL,
    CC1352R1_SWIMTHERMO_ADCBUF0CHANNELVSS,

    CC1352R1_SWIMTHERMO_ADCBUF0CHANNELCOUNT
} CC1352R1_SWIMTHERMO_ADCBuf0ChannelName;

/*!
 *  @def    CC1352R1_SWIMTHERMO_ADCName
 *  @brief  Enum of ADCs
 */
typedef enum CC1352R1_SWIMTHERMO_ADCName {
    CC1352R1_SWIMTHERMO_ADC0 = 0,
    CC1352R1_SWIMTHERMO_ADC1,
    CC1352R1_SWIMTHERMO_ADCDCOUPL,
    CC1352R1_SWIMTHERMO_ADCVSS,
    CC1352R1_SWIMTHERMO_ADCVDDS,

    CC1352R1_SWIMTHERMO_ADCCOUNT
} CC1352R1_SWIMTHERMO_ADCName;

/*!
 *  @def    CC1352R1_SWIMTHERMO_GPTimerName
 *  @brief  Enum of GPTimer parts
 */
typedef enum CC1352R1_SWIMTHERMO_GPTimerName {
    CC1352R1_SWIMTHERMO_GPTIMER0A = 0,
    CC1352R1_SWIMTHERMO_GPTIMER0B,
    CC1352R1_SWIMTHERMO_GPTIMER1A,
    CC1352R1_SWIMTHERMO_GPTIMER1B,
    CC1352R1_SWIMTHERMO_GPTIMER2A,
    CC1352R1_SWIMTHERMO_GPTIMER2B,
    CC1352R1_SWIMTHERMO_GPTIMER3A,
    CC1352R1_SWIMTHERMO_GPTIMER3B,

    CC1352R1_SWIMTHERMO_GPTIMERPARTSCOUNT
} CC1352R1_SWIMTHERMO_GPTimerName;

/*!
 *  @def    CC1352R1_SWIMTHERMO_GPTimers
 *  @brief  Enum of GPTimers
 */
typedef enum CC1352R1_SWIMTHERMO_GPTimers {
    CC1352R1_SWIMTHERMO_GPTIMER0 = 0,
    CC1352R1_SWIMTHERMO_GPTIMER1,
    CC1352R1_SWIMTHERMO_GPTIMER2,
    CC1352R1_SWIMTHERMO_GPTIMER3,

    CC1352R1_SWIMTHERMO_GPTIMERCOUNT
} CC1352R1_SWIMTHERMO_GPTimers;


/*!
 *  @def    CC1352R1_SWIMTHERMO_SPIName
 *  @brief  Enum of SPI names
 */
typedef enum CC1352R1_SWIMTHERMO_SPIName {
    CC1352R1_SWIMTHERMO_SPI0 = 0,
    CC1352R1_SWIMTHERMO_SPI1,

    CC1352R1_SWIMTHERMO_SPICOUNT
} CC1352R1_SWIMTHERMO_SPIName;

/*!
 *  @def    CC1352R1_SWIMTHERMO_UDMAName
 *  @brief  Enum of DMA buffers
 */
typedef enum CC1352R1_SWIMTHERMO_UDMAName {
    CC1352R1_SWIMTHERMO_UDMA0 = 0,

    CC1352R1_SWIMTHERMO_UDMACOUNT
} CC1352R1_SWIMTHERMO_UDMAName;

#ifdef __cplusplus
}
#endif

#endif /* __CC1352R1_SWIMTHERMO_BOARD_H__ */
