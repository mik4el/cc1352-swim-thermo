# cc1352-swim-thermo
This project tries to create a high-accuracy swim thermometer broadcasting sensor data and handling interaction on dual band radio (BLE and long range sub 1 GHz) using the Texas Instrument CC1352 MCU. Temperature sensing is done using the high precision temperature sensor LMT70. This repo contains both software and hardware implementation.

Requires Code Composer Studio (CCS) v8 and sdk version v2.30.00.xx for software and Kicad for hardware since it was developed on pre-production silicon (rev. C). Software is programmed using the CC1352 Launchpad.

Hardware version 1.1 tested with radio range over 500m+ using long range 14dBm and simple dipole antenna.

![alt text](https://github.com/mik4el/cc1352-swim-thermo/raw/master/cc1352v1_1.jpg)
