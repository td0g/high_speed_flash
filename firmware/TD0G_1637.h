#ifndef __TD0G_1637__
#define __TD0G_1637__

#include <inttypes.h>

#define SEG_A   0b00000001
#define SEG_B   0b00000010
#define SEG_C   0b00000100
#define SEG_D   0b00001000
#define SEG_E   0b00010000
#define SEG_F   0b00100000
#define SEG_G   0b01000000

class tm1637 {

public:
  tm1637(uint8_t _clkPin, uint8_t pinDISPLAY_DIO_PIN);
  void setBrightness(uint8_t _b, uint8_t _on = 1);
  void setSegments(const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0);
  void showNumberDec(int num, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);
  void showNumberDecEx(int num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);
  void flipDisplay(uint8_t invert);
  void forceLineState(uint8_t digit, uint8_t high = 255, uint8_t low = 255);
  void clearLineState();
  void refresh(uint8_t pos = 4);

private:
  void beginTransfer();
  void endTransfer();
  uint8_t sendByte(uint8_t b, bool inverted = 0);
  void writeAddress(uint8_t add = 0);
	uint8_t clkPin;
	uint8_t dioPin;
	uint8_t brightness;
	uint8_t flipped;
	uint8_t lastBytes[4];
	uint8_t digitMask = 0b00001111;
	uint8_t linesForcedHigh[4] = {0, 0, 0, 0};
	uint8_t linesForcedLow[4] = {255, 255, 255, 255};
};

#endif
