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

/*
      a
    f   b
      g
    e   c
      d
*/

class tm1637 {
  public:
    tm1637(uint8_t _clkPin, uint8_t pinDISPLAY_DIO_PIN);
    void printInt(int num, uint8_t style = RIGHT_JUSTIFY, uint8_t length = 4, uint8_t pos = 0);
    void setSegments(const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0);
    void colon(uint8_t _c = 1);
    void setBrightness(uint8_t _b, uint8_t _on = 1);
    void flipDisplay(uint8_t invert);
    void forceLineState(uint8_t digit, uint8_t high = 255, uint8_t low = 255);
    void clearLineState();
    void refresh(uint8_t pos = 4);
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
    uint8_t sendByte(uint8_t b, bool inverted = 0);
};

#endif
