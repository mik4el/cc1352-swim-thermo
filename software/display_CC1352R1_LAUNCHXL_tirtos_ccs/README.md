---
# display

---

## Example Summary

Application that writes to the UART and the available LCD display using Display
driver.

## Peripherals Exercised

* `Board_GPIO_LED0` - Toggled by example task periodically
* `Board_UART` - (If enabled) Outputs test strings
* `Board_SPI ` - (If enabled) Outputs test strings to LCDs

## Resources & Jumper Settings

> This example requires an external display device.  If you're using an IDE
(such as CCS or IAR), please refer to Board.html in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find Board.html in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

## Example Usage

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's
 Homepage"), etc.) to the appropriate COM port.
 *The COM port can be determined via Device Manager in Windows or via
 `ls /dev/tty*` in Linux.

The connection will have the following settings:
```
    Baud-rate:     115200
    Data bits:          8
    Stop bits:          1
    Parity:          None
    Flow Control:    None
```

* Run the example
* A splash screen will be displayed on LCD's and a welcome string on UART
* `Board_GPIO_LED0` is toggled periodically
* The state of LED0 is output periodically on UART and LCD

## Application Design Details

* Check the Board.html file for any board-specific settings if using an externel display device.
* If an externel display is not used, the code can be disabled by adding the
precompiler define `BOARD_DISPLAY_EXCLUDE_LCD`.

> For SensorTags there is a pin conflict, so either the DEVPACK-DEBUG or the
DEVPACK-WATCH must be used and `BOARD_DISPLAY_EXCLUDE_UART` must be added to
the global precompiler defines in order to use LCD.

> For IAR users using any SensorTag(STK) Board, the XDS110 debugger must be
selected with the 4-wire JTAG connection within your projects' debugger
configuration.

TI-RTOS:

* When building in Code Composer Studio, the kernel configuration project will
be imported along with the example. The kernel configuration project is
referenced by the example, so it will be built first. The "release" kernel
configuration is the default project used. It has many debug features disabled.
These features include assert checking, logging and runtime stack checks. For a
detailed difference between the "release" and "debug" kernel configurations and
how to switch between them, please refer to the SimpleLink MCU SDK User's
Guide. The "release" and "debug" kernel configuration projects can be found
under &lt;SDK_INSTALL_DIR&gt;/kernel/tirtos/builds/&lt;BOARD&gt;/(release|debug)/(ccs|gcc).

FreeRTOS:

* Please view the `FreeRTOSConfig.h` header file for example configuration
information.
