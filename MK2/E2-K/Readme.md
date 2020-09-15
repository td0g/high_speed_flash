# The MK2 - With Extra Power!

An extension of the E2-A, the E2-K is designed for higher power and shorter strobe durations.  The project is open-source.  Please see https://td0g.ca/2020/06/29/e2-a-the-high-speed-led-strobe-mk-2/ for more information.

## What's The Same / Different?

The CREE CXA2530 remains the best bang-for-buck LED in its class.  So there's no point in using a different LED.

Adding more LED's is a possibility, but doing so would require a new mechanical design and gate driver circuit.  It's more economical to use two strobes instead of redesigning.  So the E2-K will continue to use 12x LED's.

The 120V limitation in the E2-A is well within the safe limits of the LED, given a maximum strobe duration of 4 microseconds.  It can even do it three times in a row at 1 kHz!  So, it seems possible that the LED voltage can be increased significantly - perhaps greater than 200V.  This would result in more power output.  To protect the LED's, the maximum strobe duration would be limited to 1 microsecond.  The maximum strobe rate would be set to 1 Hz - no multi-strobe bursts!

## ToDo

Develop firmware with modified limitations (1 microsecond MAX, no multi-strobes)

Source a transistor: Ids >60A, Vds >250V

Test (and possibly modify) gate driver circuit

Conduct testing to find safe limitations for E2-K

Identify causes of failure, work to prevent them
