# CC1352 Swimthermo: fulltest_CC1352R1_SWIMTHERMO_tirtos_ccs
Aims to test all the functionality of the CC1352 Swimthermo except for BLE radio. The features include:

* Animate led over SPI LED EasySet for the http://www.ti.com/product/TLC5973.
* Read two LMT70 and print values to console over UART.
* Button that changes led animation and trigger reading LMT70 values.
* Echo packets with long range 2.5kbps mode over 868 MHz sent from rfEchoTx_CC1352R1_LAUNCHXL_tirtos_ccs.

## Pinout
* CC1352R1_SWIMTHERMO_SWITCH: IOID_5
    * Button input.
* CC1352R1_SWIMTHERMO_T_ON_1: IOID_6
    * Turn on LMT70 1
* CC1352R1_SWIMTHERMO_T_ON_2: IOID_7
    * Turn on LMT70 2
* CC1352R1_SWIMTHERMO_TAO_1: IOID_24
   * Read analogue LMT70 1 value 
* CC1352R1_SWIMTHERMO_TAO_2: IOID_23
   * Read analogue LMT70 2 value 
* CC1352R1_SWIMTHERMO_LED_EASYSET: IOID_13
   * SPI port for the LED using EasySet. 
* CC1352R1_SWIMTHERMO_PSU_ENABLE: IOID_14
   * Enable step-up converter VDD to 5V for LED.
