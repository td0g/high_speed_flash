# The MK2!

Building on the original Edgerton design, the E2-A is a high-speed flash used for photographing bullets or other projectiles.  The project is open-source.  Please see https://td0g.ca/2020/06/29/e2-a-the-high-speed-led-strobe-mk-2/ for more information.

## Building Your Flash
Please download and follow the [Assembly Manual](https://github.com/td0g/high_speed_flash/tree/master/MK2/E2-A)  (TO BE COMPLETED).  This detailed manual should answer most of your questions.  Please contact me at tyler@td0g.ca if you find any errors or inconsistencies, or simply have questions!

The **EMCB1** main control board is slightly different than the one on the original Edgerton.  I plan to manufacture a few and make them available for sale.  You can also manufacture your own using the documentation in this repository!

The **ECC0** high-voltage boost converter is my own design.  Just like the EMCB1, you may build your own or contact me to purchase one.  Note that the ECC0 may eventually be updated to be compatible with the Edgerton design, but for now the maximum input voltage is 6 Volts.

The firmware has been tested and will make setup quick and easy.  Please flash it to your microcontroller using the Arduino IDE.  Before turning on your flash, please review the User's Manual.

## Files

**Assembly Manual** - The best way to build the flash

**GERBER's** - All the files you need to send to a board house if you want to have the control board built

**BOM** - Itemized list of all materials needed to manufacture the E2-A

**Firmware** - Upload using the Arduino IDE.

## License

Documentation is licensed under a [Creative Commons Attribution 4.0 International License](https://creativecommons.org/licenses/by/4.0/)

Software is licensed under a [GNU GPL v3 License](https://www.gnu.org/licenses/gpl-3.0.txt)

## ToDo

Finish this readme

Complete and upload the assembly manual

Update the operator's manual
