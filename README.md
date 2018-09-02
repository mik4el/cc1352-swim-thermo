# cc1352-swim-thermo
This project tries to create a high-accuracy swim thermometer broadcasting sensor data and handling interaction on dual band radio (BLE and long range sub 1 GHz) using the Texas Instrument CC1352 MCU. Temperature sensing is done using the high precision temperature sensor LMT70. This repo contains both software and hardware implementation.

Requires Code Composer Studio (CCS) v8 for software and Kicad for hardware. Software is programmed using the CC1352 Launchpad.

## Known issues:
Has a hardware issue that makes the max TX power to be 10dBm caused by poor and non reference design following decoupling capacitor placement and power plane layout.

![alt text](https://github.com/mik4el/cc1352-swim-thermo/raw/master/cc1352v1.JPG)
