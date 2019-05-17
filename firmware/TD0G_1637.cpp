

/*
      a
    f   b
      g
    e   c
      d
*/

#include "TD0G_1637.h"
#include <Arduino.h>

#define COM1    0b01000000
#define COM2    0b11000000
#define COM3    0b10000000
#define WAIT 	delayMicroseconds(50);

const uint8_t numberSegment[] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};


tm1637::tm1637(uint8_t _clkPin, uint8_t _dioPin)
{
	clkPin = _clkPin;
	dioPin = _dioPin;
}

void tm1637::forceLineState(uint8_t _digit, uint8_t _high, uint8_t _low){
  if (_high != 255) linesForcedHigh[_digit] = _high;
  if (_low != 255) linesForcedLow[_digit] = 127 - _low;
  refresh(_digit);
}

void tm1637::clearLineState(){
  for (byte i = 0; i < 4; i++){
    if (linesForcedHigh[i] || linesForcedLow[i] != 127){ linesForcedHigh[i] = 0; linesForcedLow[i] = 127; refresh(i);}
  }
}

void tm1637::flipDisplay(uint8_t _inv){
  if (_inv) _inv = 1;
  flipped = _inv;
  refresh();
}

void tm1637::setBrightness(uint8_t _b, uint8_t _on)
{
	brightness = (_b & 0b00000111) | ((_on & 1) << 3);
  beginTransfer();
  sendByte(COM3 + (brightness & 0b00001111));
  endTransfer();
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
    
    if (!flipped) sendByte((lastBytes[_pos] & linesForcedLow[_pos]) | linesForcedHigh[_pos]);
    else sendByte((lastBytes[_pos] & linesForcedLow[_pos]) | linesForcedHigh[_pos], 1);

  }
  endTransfer();
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
	  if (!flipped) sendByte((_segments[k] & linesForcedLow[k]) | linesForcedHigh[k]);
    else sendByte((_segments[_length - k] & linesForcedLow[_length - k]) | linesForcedHigh[_length - k], 1);    //NEW 1
  }

	endTransfer();
}



void tm1637::showNumberDec(int num, bool leading_zero, uint8_t length, uint8_t pos)
{
  showNumberDecEx(num, 0, leading_zero, length, pos);
}

void tm1637::showNumberDecEx(int num, uint8_t dots, bool leading_zero,
                                    uint8_t length, uint8_t pos)
{
  uint8_t digits[4];
	const static int divisors[] = { 1, 10, 100, 1000 };
	bool leading = true;

	for(int8_t k = 0; k < 4; k++) {
	  int divisor = divisors[3 - k];
		int d = num / divisor;
    uint8_t digit = 0;

		if (d == 0) {
		  if (leading_zero || !leading || (k == 3))
		      digit = numberSegment[d];
	      else
		      digit = 0;
		}
		else {
			digit = numberSegment[d];
			num -= d * divisor;
			leading = false;
		}
    
    // Add the decimal point/colon to the digit
    digit |= (dots & 0x80); 
    dots <<= 1;
    
    digits[k] = digit;
	}

	setSegments(digits + (4 - length), length, pos);
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

uint8_t tm1637::sendByte(uint8_t b, bool inverted)
{
  uint8_t data;
  if (inverted) {data = (b & 0b01000000); data += (b & 0b00000111) << 3; data += (b & 0b00111000) >> 3;}
  else data = b;

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

  // DISPLAY_CLK_PIN to high
  pinMode(clkPin, INPUT);
  WAIT;
  uint8_t reply = digitalRead(dioPin);
  if (reply == 0) pinMode(dioPin, OUTPUT);


  WAIT;
  pinMode(clkPin, OUTPUT);
  WAIT;

  return reply;
}


void tm1637::writeAddress(uint8_t _addr){
  beginTransfer();
  sendByte(COM2 + (_addr & 0x03));
}
