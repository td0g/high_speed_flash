# The MK2!

Building on the original Edgerton design, the E2-A is a high-speed flash used for photographing bullets or other projectiles.  The project is open-source.  Please see https://td0g.ca/2020/06/29/e2-a-the-high-speed-led-strobe-mk-2/ for more information.

## Building Your Flash
Please download and follow the [Assembly Manual](https://github.com/td0g/high_speed_flash/raw/master/MK2/E2-A/E2-A%20Assembly%20Manual.pdf).  This detailed manual should answer most of your questions.  Please contact me at tyler@td0g.ca if you find any errors or inconsistencies, or simply have questions!

The **EMCB1** main control board is slightly different than the one on the original Edgerton.  I plan to manufacture a few and make them available for sale.  You can also manufacture your own using the documentation in this repository!

The **ECC0** high-voltage boost converter is my own design.  Just like the EMCB1, you may build your own or contact me to purchase one.  Note that the ECC0 may eventually be updated to be compatible with the Edgerton design, but for now the maximum input voltage is 6 Volts.

The firmware has been tested and will make setup quick and easy.  Please flash it to your microcontroller using the Arduino IDE.  Before turning on your flash, please review the User's Manual.

## Files

**Assembly Manual** - The best way to build the flash

**GERBER's** - All the files you need to send to a board house if you want to have the control board built

**BOM** - Itemized list of all materials needed to manufacture the E2-A

**Firmware** - Upload using the Arduino IDE

**STL** - 3D print files

**Mechanical Drawings** - Drawings for manufacturing the aluminum faceplate

## License

Documentation is licensed under a [Creative Commons Attribution 4.0 International License](https://creativecommons.org/licenses/by/4.0/)

Software is licensed under a [GNU GPL v3 License](https://www.gnu.org/licenses/gpl-3.0.txt)

## Roadmap

### Rev 0

COMPLETE

### Rev 1

Finish this readme

Assembly Manual - complete and upload

Back_Cover.STL - redesign external trigger jack housing

EMCB1 - Some components are placed too closely, will need to increase board width by a couple mm

Operator's Manual - update

### Rev 2

Adding third PCB to design: EIB0.  This board will be mounted near the encoder and display.  It will host the 5V and 12.8V regulator.  The 12.8V regulator will be an SX1308-based boost converter, no longer an off-the-shelf converter.  The board will be connected to the EMCB1 control board using an FFC ribbon, which will dramatically improve cable management.  Battery power will be supplied to the EMCB1 by 22-gauge wires so that the XL6005 and MIC3172 have ample current supply.

The EMCB1 board will be updated with a 'modelling lamp' function, based on the XL6005 boost converter SMPS.  Max power is ~10W, efficiency is ~60%.

The EMCB1 and ECC0 boards will be modified such that they can connect directly - no wires.  Fewer wires = fewer failure opportunities and more compact design

The film capacitors will be rotated slightly to improve spacing.

INVESTIGATE: Modified version to use 18650 battery(s).  1x 18650 would run the strobe, 2x could run the XL6005 boost converter at a higher power than 4x AA batteries.

INVESTIGATE: Increasing maximum exposure time

INVESTIGATE: Replacing TC4452 gate driver with more powerful driver
