# CC1352 Swimthermo: rfWsnNode_CC1352R1_SWIMTHERMO_tirtos_ccs
Based of https://github.com/mik4el/cc1352-swim-thermo/tree/master/software/rfWsnNode_CC1352R1_LAUNCHXL_tirtos_ccs but adapted for the Swimthermo board. Works with e.g. https://github.com/mik4el/cc1350-swim-thermo/tree/master/software/rfWsnConcentrator_CC1350_LAUNCHXL_tirtos_ccs.

Features include:

* Sleeps e.g. 10s, then reads two LMT70 sensors and broadcasts and acks packets with two temperature values.
* Low current sleep mode ~60uA with current board (should be possible to reach ~4uA with improved hardware design)

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
