
/*
 * Written by Tyler Gerritsen
 * vtgerritsen@gmail.com
 */
 
/*

CHANGELOG
0.1 2019-04-27
  Functional
  
0.2 2019-05-05
  Added EEPROM Data Logging
  Displays Voltage Drop and Current after flash
  Many bug fixes

0.2.1 2019-05-10
  Bug fixes
  
0.2.2  2019-05-15
  Changed display library
  Improved trigger response time slightly
  Replaced relay with MOSFET

0.2.3  2019-06-08
  Sound!
  Added colon control to TM1637 code
  Removed input voltage sensor
  
*/

#define BUTTON_DEBOUNCE 150
#define BUTTON_LONGHOLD 700   //Millis for longhold
#define CAPACITANCE_UF 3.3    //Microfarads Per LED
#define EIGHT_MHZ                  //Comment out for 16 MhZ
#define EEPROM_HISTORY_END 1020 //Leave two bytes for flash counter

#define DISPLAY_CLK_PIN 3                       //TM1637 Module
#define DISPLAY_DIO_PIN 4                       //TM1637 Module
#define FLASH_PIN 8
#define FLASH_ON PORTB = 0b00000011
#define FLASH_OFF PORTB = 0b00000010
#define BUTTON_PIN 18
#define BUTTON_PRESSED !(PINC & 0b00010000)
#define BUTTON_NOT_PRESSED PINC & 0b00010000
#define ENCODER_A_PIN 19
#define ENCODER_B_PIN 2
#define ENCODER_STATE ((PINC & 0b00100000) >> 5) | ((PIND & 0b00000100) >> 1)
#define TRIGGER_PIN 17 //10
#define TRIGGER_HIGH (PINC & 0b00001000)
#define TRIGGER_HIGH_BUTTON_NOT_PRESSED (PINC & 0b00011000)

#define HIGH_VOLT_PIN 5
#define HIGH_VOLT_ON digitalWrite(HIGH_VOLT_PIN, 1);
#define HIGH_VOLT_OFF digitalWrite(HIGH_VOLT_PIN, 0);

#define HIGH_VOLT_OUT_ANALOG 14
#define HIGH_VOLT_IN_ANALOG 15
#define INPUT_VOLT_ANALOG 16

#define NOP __asm__ __volatile__ ("nop\n\t")
#ifdef EIGHT_MHZ
  #define DELAY_500_NS NOP; NOP; NOP; NOP
#else
  #define DELAY_500_NS NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP
#endif
#define REFRESH_DISPLAY menuPosition = menuPosition | 0b10000000


#include "TD0G_1637.h"
tm1637 display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

#include <EEPROM.h>

const byte sPin = 10;
const byte trigPin = TRIGGER_PIN;
const byte highVoltPin = HIGH_VOLT_IN_ANALOG;
const byte relayPin = HIGH_VOLT_PIN;

const uint8_t CHAR_blank[] = {0, 0, 0, 0};
const uint8_t TXT_all[] = {255, 0, 255, 0};
byte nothing[4] = {0, 0, 0, 0};

byte menuPosition = 1;
byte flashState = 254;
byte flashDuration = 1;
unsigned long highVolt;
unsigned long nextVolt;
unsigned long highVolt2;
unsigned long nextVolt2;
unsigned int lowVolt;

unsigned int flashCount;

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
  pinMode(HIGH_VOLT_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(11, OUTPUT); //3500 - 4100

//Display
  display.setSegments(CHAR_blank);
  display.setBrightness(7);
  display.flipDisplay(1);
  REFRESH_DISPLAY;

//Serial
  Serial.begin(9600);
  Serial.println("EDGERTON Ready");
  
  for (int i = 3500; i < 4200; i += 200){
    tone(11, i, 80);
    delay(100);
  }

//Get flash count
  flashCount = EEPROM.read(EEPROM_HISTORY_END);
  flashCount = flashCount << 8;
  flashCount |= EEPROM.read(EEPROM_HISTORY_END + 1);
  display.colon(0);
}

void loop() {
  runDisplay();
  runUI();
  runSerial();
}
