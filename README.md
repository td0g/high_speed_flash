# high_speed_flash
Firmware for a high-speed LED flash.

## Description

This is the firmware for Edgerton, a home-built high-speed LED flash.  The firmware runs on an ATMega328P microcontroller.  Schematics will be made available soon.

## Use

When the flash is turned on, the display shows the flash duration selected and the battery voltage.  Rotating the encoder adjusts the flash duration.

A short press of the encoder will put the flash into external trigger mode, where it will wait for a signal from the 3.5mm jack.

A long-hold of the encoder will fire the flash ASAP.
