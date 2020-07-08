/*
  TM1637 display library for Arduino
  Written by Tyler Gerritsen
  vtgerritsen@gmail.com
  www.td0g.ca
 
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __TD0G_1637__
#define __TD0G_1637__

#include <inttypes.h>

#define SEG_A  0b00000001
#define SEG_B  0b00000010
#define SEG_C  0b00000100
#define SEG_D  0b00001000
#define SEG_E  0b00010000
#define SEG_F  0b00100000
#define SEG_G  0b01000000
#define RIGHT_JUSTIFY 0
#define LEFT_JUSTIFY 1

/*
      a
    f   b
      g
    e   c
      d
*/

class tm1637 {
  public:
    tm1637(uint8_t _clkPin, uint8_t _dioPin);
/*
  Constructor
  Do not change CLK or DIO pin state or pin mode outside this library
  PARAMETERS: CLK pin, DIO pin
 */

 
    void printInt(int num, uint8_t style = RIGHT_JUSTIFY, uint8_t length = 4, uint8_t pos = 0);
/*
  Prints an integer value to the display
  If integer is > 9999, then it will display integer % 10000
  PARAMETERS: integer to print
  OPTIONAL PARAMETERS: Justification (RIGHT_JUSTIFY,LEFT_JUSTIFY), length of integer (digits), starting position (digit)
 */

 
    void setSegments(const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0);
/*
  Prints a custom character set to the display
  PARAMETERS: Byte array to print (default length: 4)
  OPTIONAL PARAMETERS: Length of array if < 4, starting position if array length < 4
 */

 
    void colon(uint8_t _c = 1);
/*
  Turns the centre colon on / off
  PARAMETERS: colon on/off (default: on)
 */

 
    void setBrightness(uint8_t _b, uint8_t _on = 1);
/*
  Sets the display brightness
  PARAMETERS: brightness (0 - 7)
  OPTIONAL PARAMETERS: Display on/off
 */

 
    void flipDisplay(uint8_t invert = 2);
/*
  Rotates the display 180 degrees
  Note: call refresh() to display change
  OPTIONAL PARAMETERS: 0 / 1 rotation (default: opposite rotation)
 */

 
    void forceLineState(uint8_t digit, uint8_t high = 255, uint8_t low = 255);
/*
  Overwrites the state of a digit in the display
  Any segment not forced high or low will display the integer or custom character previously printed
  Segments forced high take priority
  NOTE: if MSB is 1 then the parameter will be ignored (eg. if high = 128 to 255 then no segments will be set high)
  PARAMETERS: digit (0 - 4)
  OPTIONAL PARAMETERS: lines forced on (see segment bits above), lines forced off (see segment bits above)
 */

 
    void clearLineState();
/*
  Clears all forced line segment states
 */

 
    void refresh(uint8_t pos = 4);
/*
  Re-prints the display
  OPTIONAL PARAMETERS: digit to re-print (default is all digits)
 */

 
  private:
  	uint8_t clkPin;
  	uint8_t dioPin;
  	uint8_t brightness;
  	uint8_t flipped;
  	uint8_t lastBytes[4];
  	uint8_t digitMask = 0b00001111;
    uint8_t colonMask = 0;
  	uint8_t linesForcedHigh[4] = {0, 0, 0, 0};
  	uint8_t linesForcedLow[4] = {255, 255, 255, 255};
    void beginTransfer();
    void endTransfer();
    void writeAddress(uint8_t add = 0);
    uint8_t sendByte(uint8_t _b, bool _inverted = 0);
};

#endif
