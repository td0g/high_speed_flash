# The Video Version!

An extension of the E2-A, the E2-V has all of the same functionality but includes a constant-illumination mode.  The project is open-source.  Please see https://td0g.ca/2020/06/29/e2-a-the-high-speed-led-strobe-mk-2/ for more information.

## What's The Same / Different?

Much of the design should remain the same.  The goal is to have an external power supply which can be connected in place of the battery to provide constant current to the LED's.

The gate driver needs to have an automated switch for the high-pass filter which disables the filter if the LED capacitor anode voltage is less than ~45 volts.  Otherwise the external power supply cannot provide constant current to the LED's as the transistors cannot remain on for more than 4.5 microseconds.

An offline drive would seem to be the simplest approach, but designing the driver safely presents a significant challenge.  A safer alternative would be to use a 140-watt encapsulated 36V or 48V power supply to power a buck converter.

Because the external power supply will operate with its ground common to the strobe, it requires a high-side switching buck power supply.  The LM3409HV seems to be a suitable candidate.

The buck power supply must be controlled by the strobe.  An opto-isolated interface bus would seem to be the safest way.

The external power supply must also supply a ~6-volt rail to power the strobe.  This requires yet another buck power supply.

Finally, the firmware must be heavily modified to control the power supplies safely.

## ToDo

Finish development of the LM3409HV-based LED driver

Develop 48V to 6V buck regulator

Modify firmware to support constant illumination mode

Integrate LED driver into E2 design

Ensure LED's are load balanced (https://www.onsemi.com/pub/Collateral/AND8109-D.PDF)
