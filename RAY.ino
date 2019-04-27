
/*
 * Written by Tyler Gerritsen
 * vtgerritsen@gmail.com
 */
 
/*

CHANGELOG
0.1 2019-04-27
  Functional
*/

#define NOP __asm__ __volatile__ ("nop\n\t")
#define NOP4 NOP; NOP; NOP; NOP
#define REFRESH_DISPLAY menuPosition = menuPosition | 0b10000000

#include "TM1637DisplayAdvanced.h"
#define CLK 3                       //TM1637 Module
#define DIO 4                       //TM1637 Module
TM1637Display display(CLK, DIO);

#define BUTTON_PIN 18
#define BUTTON_DEBOUNCE 150
#define BUTTON_LONGHOLD 800
#define BUTTON_PRESSED !(PINC & 0b00010000)
#define ENCODER_A_PIN 19
#define ENCODER_B_PIN 2
#define ENCODER_STATE ((PINC & 0b00100000) >> 5) | ((PIND & 0b00000100) >> 1)

#define FLASH_PIN 8

#define TRIGGER_PIN 17 //10
#define TRIGGER_HIGH (PINC & 0b00001000)

#define HIGH_VOLT_PIN 5
#define HIGH_VOLT_ON pinMode(HIGH_VOLT_PIN, OUTPUT);
#define HIGH_VOLT_OFF pinMode(HIGH_VOLT_PIN, INPUT);

#define HIGH_VOLT_OUT_ANALOG 14
#define HIGH_VOLT_IN_ANALOG 15
#define INPUT_VOLT_ANALOG 16

const byte sPin = 10;
const byte trigPin = TRIGGER_PIN;
const byte highVoltPin = HIGH_VOLT_IN_ANALOG;
const byte relayPin = HIGH_VOLT_PIN;

const uint8_t CHAR_blank[] = {0, 0, 0, 0};
const uint8_t TXT_all[] = {255, 0, 255, 0};

byte menuPosition = 0;
byte flashState = 254;
byte flashDuration = 1;
int highVolt;
unsigned int lowVolt;

void setup() {
//GPIO Setup
  pinMode(FLASH_PIN, OUTPUT);
  digitalWrite(FLASH_PIN, 0);
  HIGH_VOLT_OFF;
  pinMode(ENCODER_B_PIN, INPUT_PULLUP);
  pinMode(ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(HIGH_VOLT_IN_ANALOG, INPUT);
  pinMode(HIGH_VOLT_OUT_ANALOG, INPUT);
  pinMode(INPUT_VOLT_ANALOG, INPUT);
  lowVolt = analogRead(INPUT_VOLT_ANALOG) * 20;
  pinMode(TRIGGER_PIN, INPUT_PULLUP);

//Display
  display.setSegments(TXT_all);
  display.setBrightness(7);
  display.setInverted(1);
  REFRESH_DISPLAY;

//Serial
  Serial.begin(9600);
  Serial.println("RAY Ready");
}

void loop() {
  runDisplay();
  runUI();
  runExternal();  //Upon recieving a 1ms low signal, charge flash and wait for next signal to fire
}


void runUI(){
  static unsigned long _t;
  
  menuPosition += readEncoder();
  if (menuPosition == 255) menuPosition = 0;
  else if (menuPosition == 5) menuPosition = 4;

  byte _b = readBtn();
	switch (menuPosition){
	  case 0:
      flashState = 255;
      if (readBtn()) initializeFlash();
      _t = millis();
      if (flashState != 255){
        while (readBtn() != 2){
          if (millis() > _t){
            if (_t & 1) {
              byte err[4] = {0, SEG_A | SEG_D | SEG_E | SEG_F | SEG_G, SEG_E | SEG_G, SEG_E  | SEG_G};
              display.setSegments(err);
            }
            else {
              display.showNumberDec(flashState);
            }
            _t += 801;
          }
        }
      }
		break;
    case 1:
      flashDuration = 0;
      if (_b == 2) {
        chargeFlash();
        byte nothing[4] = {0, 0, 0, 0};
        display.setSegments(nothing);
        Serial.println("500 ns");
        Serial.println("Capacitor Voltage Before, After:");
        Serial.print(analogRead(HIGH_VOLT_IN_ANALOG));
        Serial.print(" ");
        Serial.println(analogRead(HIGH_VOLT_OUT_ANALOG));
        Serial.println(" -- ");
        highVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
        forceFlash();
        unsigned long nextVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
        nextVolt *= 100;
        nextVolt /= highVolt;
        display.showNumberDec(nextVolt);
       unsigned long _t = millis() + 800;
       while (millis() < _t) {
        Serial.print(analogRead(HIGH_VOLT_IN_ANALOG));
        Serial.print(" ");
        Serial.println(analogRead(HIGH_VOLT_OUT_ANALOG));
       }
       REFRESH_DISPLAY;
       HIGH_VOLT_OFF;
  	  }
      else if (_b == 1){
      chargeFlash();
        runFlash();
        delay(3000);
        REFRESH_DISPLAY;
       HIGH_VOLT_OFF;
       while (!TRIGGER_HIGH){};
      }
    break;
  	case 2:
      flashDuration = 1;
      if (_b == 2) {
        chargeFlash();
        byte nothing[4] = {0, 0, 0, 0};
        display.setSegments(nothing);
        Serial.println("1000 nanoseconds");
        Serial.println("Capacitor Voltage Before, After:");
        Serial.print(analogRead(HIGH_VOLT_IN_ANALOG));
        Serial.print(" ");
        Serial.println(analogRead(HIGH_VOLT_OUT_ANALOG));
        Serial.println(" -- ");
        highVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
        forceFlash();
        unsigned long nextVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
        nextVolt *= 100;
        nextVolt /= highVolt;
        display.showNumberDec(nextVolt);
       unsigned long _t = millis() + 800;
       while (millis() < _t) {
        Serial.print(analogRead(HIGH_VOLT_IN_ANALOG));
        Serial.print(" ");
        Serial.println(analogRead(HIGH_VOLT_OUT_ANALOG));
       }
       REFRESH_DISPLAY;
       HIGH_VOLT_OFF;
      }
      else if (_b == 1){
      chargeFlash();
        runFlash();
        delay(3000);
        REFRESH_DISPLAY;
       HIGH_VOLT_OFF;
       while (!TRIGGER_HIGH){};
      }
  	break;
    case 3:
      flashDuration = 2;
      if (_b == 2) {
        chargeFlash();
        byte nothing[4] = {0, 0, 0, 0};
        display.setSegments(nothing);
        Serial.println("2000 nanoseconds");
        Serial.println("Capacitor Voltage Before, After:");
        Serial.print(analogRead(HIGH_VOLT_IN_ANALOG));
        Serial.print(" ");
        Serial.println(analogRead(HIGH_VOLT_OUT_ANALOG));
        Serial.println(" -- ");
        highVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
        forceFlash();
        unsigned long nextVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
        nextVolt *= 100;
        nextVolt /= highVolt;
        display.showNumberDec(nextVolt);
       unsigned long _t = millis() + 800;
       while (millis() < _t) {
        Serial.print(analogRead(HIGH_VOLT_IN_ANALOG));
        Serial.print(" ");
        Serial.println(analogRead(HIGH_VOLT_OUT_ANALOG));
       }
       REFRESH_DISPLAY;
       HIGH_VOLT_OFF;
      }
      else if (_b == 1){
      chargeFlash();
        runFlash();
        delay(3000);
        REFRESH_DISPLAY;
       HIGH_VOLT_OFF;
       while (!TRIGGER_HIGH){};
      }
    break;
    case 4:
      flashDuration = 3;
      if (_b == 2) {
        chargeFlash();
        byte nothing[4] = {0, 0, 0, 0};
        display.setSegments(nothing);
        Serial.println("4000 nanoseconds");
        Serial.println("Capacitor Voltage Before, After:");
        Serial.print(analogRead(HIGH_VOLT_IN_ANALOG));
        Serial.print(" ");
        Serial.println(analogRead(HIGH_VOLT_OUT_ANALOG));
        Serial.println(" -- ");
        highVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
        forceFlash();
        unsigned long nextVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
        nextVolt *= 100;
        nextVolt /= highVolt;
        display.showNumberDec(nextVolt);
       unsigned long _t = millis() + 800;
       while (millis() < _t) {
        Serial.print(analogRead(HIGH_VOLT_IN_ANALOG));
        Serial.print(" ");
        Serial.println(analogRead(HIGH_VOLT_OUT_ANALOG));
       }
       REFRESH_DISPLAY;
       HIGH_VOLT_OFF;
      }
      else if (_b == 1){
      chargeFlash();
        runFlash();
        delay(3000);
        REFRESH_DISPLAY;
       HIGH_VOLT_OFF;
       while (!TRIGGER_HIGH){};
      }
    break;
  }
}

void runDisplay(){
  static unsigned long _timer = 0;
  static byte _menuPosition;
  static byte _switch;
  byte txtDiag[4] = {SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,SEG_C,SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,SEG_A | SEG_C | SEG_D | SEG_E | SEG_F};

  if (_menuPosition != menuPosition) {
    _switch = 1;
    _timer = millis() - 1;
    menuPosition &= 0b01111111;
   _menuPosition = menuPosition;
  }
  
  if (millis() > _timer){
    _timer = millis() + 1000;
    _switch = 1 - _switch;
    if (!_switch){
      switch (menuPosition){
        case 0:
          display.setSegments(txtDiag);
        break;
        case 1:
          display.showNumberDec(500);
        break;
        case 2:
          display.showNumberDec(1000);
        break;
        case 3:
          display.showNumberDec(2000);
        break;
        case 4:
          display.showNumberDec(4000);
        break;
      }
    }
    else {
      lowVolt += analogRead(INPUT_VOLT_ANALOG) * 20;
      lowVolt = lowVolt >> 1;
      display.showNumberDec(lowVolt);
    }
  }
}

void forceFlash(){

  switch (flashDuration){
  break;
  case 1:
      PORTB = 0b00000011;
      NOP4;
      NOP4;
      PORTB = 0b00000010;
  break;
  case 2:
      PORTB = 0b00000011;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      PORTB = 0b00000010;
  break;
  case 3:
      PORTB = 0b00000011;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      PORTB = 0b00000010;
  break;
    default:
      PORTB = 0b00000011;
      NOP4;
      PORTB = 0b00000010;
  }
  PORTB = 0b00000010;
}


void runFlash(){
      byte nothing[4] = {0, 0, 0, 0};
      display.setSegments(nothing);
  switch (flashDuration){
	break;
	case 1:
	  while ((TRIGGER_HIGH)){};
	    PORTB = 0b00000011;
      NOP4;
      NOP4;
	    PORTB = 0b00000010;
	break;
	case 2:
	  while ((TRIGGER_HIGH)){};
	    PORTB = 0b00000011;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
	    PORTB = 0b00000010;
	break;
	case 3:
	  while ((TRIGGER_HIGH)){};
	    PORTB = 0b00000011;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
      NOP4;
	    PORTB = 0b00000010;
	break;
  default:
	  while ((TRIGGER_HIGH)){};
	    PORTB = 0b00000011;
      NOP4;
	    PORTB = 0b00000010;
     break;
  }
  PORTB = 0b00000010;
}



void chargeFlash(){
  byte i = 10;
  unsigned long _t;
  
  highVolt = 0;
  HIGH_VOLT_ON;
  _t = millis() + 500;
  while (millis() < _t) display.showNumberDec(analogRead(HIGH_VOLT_OUT_ANALOG));
  while (i && analogRead(HIGH_VOLT_OUT_ANALOG) > highVolt + 1){
    i--;
    highVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
    _t = millis() + 500;
    while (millis() < _t) display.showNumberDec(analogRead(HIGH_VOLT_OUT_ANALOG));
  }
}


void initializeFlash(){
  /*
   * ERRORS:
   * ERR 0 - Capacitor already charged prior to initiation
   * ERR 1 - High-Volt on briefly, no charge
   * ERR 2 - High-Volt off briefly, no discharge
   * ERR 3 - High-Volt on, charge not reaching full
   * ERR 4 - Flash activated, no discharge
   */
  if (flashState == 254){
    flashState = 0;
	  highVolt = analogRead(INPUT_VOLT_ANALOG) * 20;
    Serial.print("highVolt ");
    Serial.println(highVolt);
    display.showNumberDec(analogRead(HIGH_VOLT_OUT_ANALOG));
	  if (highVolt > 110) {
      highVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
      Serial.print("highVolt ");
      Serial.println(highVolt);
      flashState =  1;
      delay(500);
      HIGH_VOLT_ON;
  		unsigned long _t;
  		_t = millis() + 1000;
  		while (millis() < _t) display.showNumberDec(analogRead(HIGH_VOLT_OUT_ANALOG));
      Serial.println(analogRead(HIGH_VOLT_OUT_ANALOG));
  		if (analogRead(highVoltPin) > highVolt) {
        flashState = 2;
        highVolt = analogRead(highVoltPin);
        _t = millis() + 3000;
        HIGH_VOLT_OFF;
        highVolt = highVolt - min(highVolt, 10);
  		  while (analogRead(highVoltPin) > highVolt && millis() < _t)display.showNumberDec(analogRead(HIGH_VOLT_OUT_ANALOG));
  		  if (millis() < _t) {
          flashState = 3;
          HIGH_VOLT_ON;
          highVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
          _t = millis() + 500;
          while (millis() < _t) display.showNumberDec(analogRead(HIGH_VOLT_OUT_ANALOG));
          byte i = 10;
  			  while (i && analogRead(HIGH_VOLT_OUT_ANALOG) > highVolt + 1){
            i--;
            highVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
  			    _t = millis() + 500;
            while (millis() < _t) display.showNumberDec(analogRead(HIGH_VOLT_OUT_ANALOG));
  			  }
         if (i){
            flashState = 4;
            HIGH_VOLT_OFF;
    			  delay(10);
    			  int _highVolt;
    			  highVolt = analogRead(highVoltPin);
    			  flashDuration = 1;
    			  //forceFlash();
    			  _highVolt = analogRead(highVoltPin);
    			  if (highVolt < _highVolt - 2) flashState = 255;
           flashState = 255;
         }
  		  }
  		}
	  }
	}
  HIGH_VOLT_OFF;
}

int8_t readEncoder(){
  static const byte _p [4] = {2, 3, 1, 0};
  static byte _inLast;
  byte _in = _p[ENCODER_STATE];
  if (_inLast == (_in + 1) % 4){
    _inLast = _in;
    if (_in == 1) {
      return -1;
    }
  }
  else if ((_inLast + 1) % 4 == _in) {
    _inLast = _in;
    if (_in == 2){
      return 1;
    }
  }
  else _inLast = _in;
  return 0;
}

uint8_t readBtn(){  //Return 1 if short pressed & released, Return 2 if long pressed PRIOR to release
  static unsigned long debounceTimer;
  static byte buttonPos;
    if (BUTTON_PRESSED && millis() > debounceTimer){   //Button depressed
      if (!buttonPos){ buttonPos = 1; debounceTimer = millis() + BUTTON_LONGHOLD;}
      else if (buttonPos == 1){buttonPos = 2; return 2;}
    }
    else if (buttonPos && !(BUTTON_PRESSED)) {
      debounceTimer = millis() + BUTTON_DEBOUNCE;
      if (buttonPos == 1) {buttonPos = 0; return 1;}
      buttonPos = 0;
    }
    return 0;
}
