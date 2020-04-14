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

#include "TD0G_1637.h"
#include <Arduino.h>

#define COM1  0b01000000 //SEGMENT STATE
#define COM2  0b11000000 //ADDRESS
#define COM3  0b10000000 //BRIGHTNESS
#define WAIT 	delayMicroseconds(50);

tm1637::tm1637(uint8_t _clkPin, uint8_t _dioPin)
{
	clkPin = _clkPin;
	dioPin = _dioPin;
}

void tm1637::printInt(int _num, uint8_t _style, uint8_t _length, uint8_t _pos)
{
  const uint8_t numberSegment[] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};  //numbers 0 - 9
  uint8_t digits[4];                          //Segment array (to send to setSegments)
  const static int E[] = {1000, 100, 10, 1};  //Log scale

  for(int8_t k = 0; k < 4; k++) { //Loop through all four digits
    int divisor = E[k];
    int d = _num / divisor;
    uint8_t digit = 0;
    if (!d) {   
      if (_style || (k == 3)) digit = numberSegment[d];
      else digit = 0;
    }
    else {
      _style = 1;
      digit = numberSegment[d % 10];
      _num -= d * divisor;
    }
    digits[k] = digit;
  }

  setSegments(digits + (4 - _length), _length, _pos);
}

void tm1637::setSegments(const uint8_t _segments[], uint8_t _length, uint8_t _pos)
{
  _length--;
  
  beginTransfer();
  sendByte(COM1);
  endTransfer();

  if (!flipped) writeAddress(_pos);
  else writeAddress(3 - _pos - _length);
  
  for (uint8_t k=0; k <= _length; k++)
  {
    lastBytes[k + _pos] = _segments[k];     //Save byte in case refresh is called
    if (!flipped) sendByte((_segments[k] & linesForcedLow[k]) | linesForcedHigh[k] | colonMask);
    else sendByte((_segments[_length - k] & linesForcedLow[_length - k]) | linesForcedHigh[_length - k] | colonMask, 1);    //NEW 1
  }

  endTransfer();
}

void tm1637:: colon(uint8_t _c){
  if (!_c && colonMask){  //Turning colon off and colon is currently on
    colonMask = 0;
    refresh(2);
  }
  if (_c && !colonMask) { //Turning colon on and colon is currently off
    colonMask = 0b10000000;
    refresh(2);
  }
}

void tm1637::setBrightness(uint8_t _b, uint8_t _on)
{
  brightness = (_b & 0b00000111) | ((_on & 1) << 3);  //brightness = [][][][][ON/OFF][BRIGHTNESS_MSB][BRIGHTNESS][BRIGHTNESS_LSB]
  beginTransfer();
  sendByte(COM3 | brightness);
  endTransfer();
}

void tm1637::flipDisplay(uint8_t _inv){
  if (_inv & 0b11111110) _inv = 1 - flipped;  //parameter is not 0 or 1: just flip display
  flipped = _inv;                             //NOTE: changes don't take affect until next printInt(), printSegment(), or refresh()
}

void tm1637::forceLineState(uint8_t _digit, uint8_t _high, uint8_t _low){
  if (!(_high & 0b10000000)) linesForcedHigh[_digit] = _high;
  if (!(_low & 0b10000000)) linesForcedLow[_digit] = 127 - _low;
  refresh(_digit);
}

void tm1637::clearLineState(){
  for (byte i = 0; i < 4; i++){
    if (linesForcedHigh[i] || linesForcedLow[i] != 127){ linesForcedHigh[i] = 0; linesForcedLow[i] = 127; refresh(i);}
  }
}

void tm1637::refresh(uint8_t _pos)
{
  if (_pos == 4) setSegments(lastBytes);
  else {
    beginTransfer();
    sendByte(COM1);
    endTransfer();
  
    if (flipped) writeAddress(3 - _pos);
    else writeAddress(_pos);
    
    if (!flipped) sendByte((lastBytes[_pos] & linesForcedLow[_pos]) | linesForcedHigh[_pos] | colonMask);
    else sendByte((lastBytes[_pos] & linesForcedLow[_pos]) | linesForcedHigh[_pos] | colonMask, 1);

  }
  endTransfer();
}

void tm1637::beginTransfer()
{
  pinMode(dioPin, OUTPUT);
  WAIT;
}

void tm1637::endTransfer()
{
	pinMode(dioPin, OUTPUT);
	WAIT;
	pinMode(clkPin, INPUT);
	WAIT;
	pinMode(dioPin, INPUT);
	WAIT;
}

void tm1637::writeAddress(uint8_t _addr){
  beginTransfer();
  sendByte(COM2 | (_addr & 0x03));
}

uint8_t tm1637::sendByte(uint8_t _b, bool _inverted)
{
  uint8_t data;
  if (_inverted) {data = (_b & 0b11000000); data += (_b & 0b00000111) << 3; data += (_b & 0b00111000) >> 3;}
  else data = _b;

  for(uint8_t i = 0; i < 8; i++) {
    pinMode(clkPin, OUTPUT);
    WAIT;
    if (data & 1) pinMode(dioPin, INPUT);
    else pinMode(dioPin, OUTPUT);
    WAIT;
    pinMode(clkPin, INPUT);
    WAIT;
    data = data >> 1;
  }

  pinMode(clkPin, OUTPUT);
  pinMode(dioPin, INPUT);
  WAIT;
  pinMode(clkPin, INPUT);
  WAIT;
  uint8_t reply = digitalRead(dioPin);
  if (reply == 0) pinMode(dioPin, OUTPUT);

  WAIT;
  pinMode(clkPin, OUTPUT);
  WAIT;

  return reply;
}
