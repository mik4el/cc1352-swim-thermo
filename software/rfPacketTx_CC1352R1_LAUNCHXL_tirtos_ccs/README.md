# rfPacketTx

---

Project Setup using the System Configuration Tool (SysConfig)
-------------------------
The purpose of SysConfig is to provide an easy to use interface for configuring
drivers, RF stacks, and more. The .syscfg file provided with each example
project has been configured and tested for that project. Changes to the .syscfg
file may alter the behavior of the example away from default. Some parameters
configured in SysConfig may require the use of specific APIs or additional
modifications in the application source code. More information can be found in
SysConfig by hovering over a configurable and clicking the question mark (?)
next to it's name.

Example Summary
---------------
The Packet TX example illustrates how to do simple packet transmission using
the RF driver. This example is meant to be used with the Packet RX
example or SmartRF Studio. For every packet transmitted, CONFIG_PIN_RLED is toggled.
The frequency and other RF settings can be modified using SmartRF Studio.

Peripherals Exercised
---------------------
* `CONFIG_PIN_RLED` - Toggled when data is transmitted over the RF interface

Resources & Jumper Settings
---------------------------
> If you're using an IDE (such as CCS or IAR), please refer to Board.html in your project
directory for resources used and board-specific jumper settings. Otherwise, you can find
Board.html in the directory &lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

Board Specific Settings
-----------------------
1. The default frequency is:
    - 433.92 MHz for the CC1350-LAUNCHXL-433
    - 433.92/490 MHz for the CC1352P-4-LAUNCHXL
    - 2440 MHz on the CC2640R2-LAUNCHXL
    - 868.0 MHz for other launchpads
In order to change frequency, modify the ti_radio_config.c file. This can be
done using the code export feature in Smart RF Studio, or directly in the file
2. On the CC1352P1 the high PA is enabled (high output power) for all
Sub-1 GHz modes by default.
3. On the CC1352P-2 the high PA operation for Sub-1 GHz modes is not supported
4. On the CC1352P-4 the high PA is enabled (high output power) for all
Sub-1 GHz modes by default.
    - The center frequency for 2-GFSK is set to 490 MHz
    - **CAUTION:** The center frequency for SimpleLink long range (SLR) is set to 433.92 MHz,
    but the high output power violates the maximum power output requirement
    for this band
5. The CC2640R2 is setup to run all proprietary physical modes at a center
frequency of 2440 MHz, at a data rate of 250 Kbps

Example Usage
-------------
Run the example. On another board, run the Packet RX example.
CONFIG_PIN_RLED is toggled when data is transmitted.

Application Design Details
--------------------------
This examples consists of a single task and the exported SmartRF Studio radio
settings.

When the task is executed it:

1. Configures the radio for Proprietary mode
2. Gets access to the radio via the RF drivers RF_open
3. Sets up the radio using CMD_PROP_RADIO_DIV_SETUP command
4. Set the output power to 14 dBm (requires that CCFG_FORCE_VDDR_HH = 1 in ccfg.c),
this requirement holds for CC13x2P boards when using the default PA
5. Sets the frequency using CMD_FS command
6. Create packet (with increasing sequence number and random content)
7. Transmit packet using CMD_PROP_TX command with blocking RF driver call
8. Toggle CONFIG_PIN_RLED to indicate packet transmitted
9. Power down the radio using the yield function
10. Set the main core to sleep for the duration of `PACKET_INTERVAL` ms
11. Transmit packets forever by repeating step 6-10

Note for IAR users: When using the CC1310DK, the TI XDS110v3 USB Emulator must
be selected. For the CC1310_LAUNCHXL, select TI XDS110 Emulator. In both cases,
select the cJTAG interface.
