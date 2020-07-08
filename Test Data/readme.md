# Research and Development Testing Data

The high-speed flash project relies heavily on testing to find the limitations of the LED's.  
Without understanding the limits, a high-speed flash will either (1) underperform, resulting in higher costs and inferior performance, or (2) fail prematurely.

## Overview of the Test Data

I have a significant amount of testing data available in my personal notes.  The testing data is slowly being uploaded to this repository.

Testing data includes measurements of the LED performance under various conditions, conditions under which the LED's will fail, and other tests related to the strobe's infrastructure (control board, power supply architecture, etc.)

## A Brief History

During development of the original Edgerton, I had access to an oscilloscope and camera.  
It came as a surprise that my Sekonic L-358 was unable to measure the light output from the flash.
Many days were spent strobing LED's at a low frequency (1 - 2 hz) at various drive voltages to establish the limitations.

Since then, I've come to acquire my own oscilloscope, an [active light probe](https://www.instructables.com/id/Light-Probe-MK-I/) (Thank you NQTRONIX for your donation!!!), an integrating sphere, and various other testing equipment.
With this new equipment, I've been able to focus on the actual light output from the LED's instead of simply the voltage they could handle.

Unfortunately, my earlier tests were not as well documented as I would have liked.  
Every LED failure has been documented in my notes, but I tended to 'thrash' in testing, resulting in poor documentation leading up to the failures.
The documentation of the tests have improved recently.

## Future Goals

I plan to continue destructive testing of LED's in order to find ways to improve the performance and robustness of the flash.

I am also working to find a method of examining the failed LED's so that the mode of failure can be better understood.
