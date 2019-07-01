/*
  Firmware for Edgerton High-Speed LED flash
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
  New tripwire function
  Added colon control to TM1637 code
  Removed high-voltage input sensor
  General code hygiene improvements

0.2.4  2019-06-28
  Trigger now automatically detects active low/high, will activate on signal change
  Chirp indicates trigger state change when not in standby mode
  Improved delay precision
  
  
*/

//Basic Configuration
  #define MIN_BATTERY_VOLTAGE 8.5
  #define BUTTON_DEBOUNCE 150
  #define BUTTON_LONGHOLD 700   //Millis for longhold
  #define CAPACITANCE_UF 3.3    //Microfarads Per LED
  #define EIGHT_MHZ             //Comment out for 16 MhZ
  #define SOUND                 //Comment out if no piezo buzzer is installed
  #define EEPROM_HISTORY_END 1012 //Leave two bytes for flash counter, eight for voltage calibration
  #define DISPLAY_TIMEOUT 6000

//Duration Calibration
  #define DELAY_500NS NOP;
  #define DELAY_1US NOP;NOP;NOP;NOP;NOP; 
  #define DELAY_2US DELAY_500_NS;DELAY_500_NS;DELAY_500_NS;NOP;NOP;NOP;
  #define DELAY_4US DELAY_500_NS;DELAY_500_NS;DELAY_500_NS;DELAY_500_NS;DELAY_500_NS;DELAY_500_NS;DELAY_500_NS;

//Pin Configuration
  #define DISPLAY_CLK_PIN 3                       //TM1637 Module
  #define DISPLAY_DIO_PIN 4                       //TM1637 Module
  #define SOUND_PIN 11
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
  #define TRIGGER_LOW (!(PINC & 0b00001000))
  #define TRIGGER_LOW_BUTTON_NOT_PRESSED (!(PINC & 0b00001000) && (PINC & 0b00010000))
  #define HIGH_VOLT_PIN 5
  #define HIGH_VOLT_ON digitalWrite(HIGH_VOLT_PIN, 1);
  #define HIGH_VOLT_OFF digitalWrite(HIGH_VOLT_PIN, 0);
  #define HIGH_VOLT_OUT_ANALOG 14
  #define INPUT_VOLT_ANALOG 16

//Other Definitions - DO NOT EDIT
  #define NOP __asm__ __volatile__ ("nop\n\t")
  #ifdef EIGHT_MHZ
    #define DELAY_500_NS NOP; NOP; NOP; NOP
  #else
    #define DELAY_500_NS NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP
  #endif
  #define REFRESH_DISPLAY menuPosition = menuPosition | 0b10000000

  #define TONE_A 3520 //Good Volume
  #define TONE_A_S 3729 //Good Volume
  #define TONE_B 3951 //Good Volume
  #define TONE_C 4186 //Good Volume
  #define TONE_C_S 4435
  #define TONE_D 4698
  #define TONE_D_S 4978
  #define TONE_E 5274
  #define TONE_F 5588
  #define TONE_G 6272

//Display
  #include "TD0G_1637.h"
  tm1637 display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);
  const uint8_t TXT_all[] = {255, 0, 255, 0};
  const uint8_t TXT_nothing[4] = {0, 0, 0, 0};

//Non-Volatile Memory
  #include <EEPROM.h>

//Global Variables
  const byte sPin = 10;
  const byte trigPin = TRIGGER_PIN;
  const byte relayPin = HIGH_VOLT_PIN;

  unsigned long flashDelay = 0;
  byte menuPosition = 0;
  byte flashDuration = 1;
  unsigned long highVolt;
  unsigned long nextVolt;
  unsigned long highVolt2;
  unsigned long nextVolt2;
  unsigned int lowVolt;

  unsigned int flashCount;

  float batteryVoltCal;
  float highVoltCal;

void setup() {
//GPIO Setup
  pinMode(FLASH_PIN, OUTPUT);
  digitalWrite(FLASH_PIN, 0);
  HIGH_VOLT_OFF;
  pinMode(ENCODER_B_PIN, INPUT_PULLUP);
  pinMode(ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(HIGH_VOLT_OUT_ANALOG, INPUT);
  pinMode(INPUT_VOLT_ANALOG, INPUT);
  lowVolt = analogRead(INPUT_VOLT_ANALOG) * 20;
  pinMode(HIGH_VOLT_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  #ifdef SOUND
    pinMode(SOUND_PIN, OUTPUT); //3500 - 4100
  #endif

//Display
  display.setSegments(TXT_nothing);
  display.setBrightness(7);
  display.flipDisplay(1);
  REFRESH_DISPLAY;

//Serial
  Serial.begin(9600);
  Serial.println("EDGERTON Ready");

//Calibration
  EEPROM.get(EEPROM_HISTORY_END, batteryVoltCal);
  EEPROM.get(EEPROM_HISTORY_END + 4, highVoltCal);
  initialSetup();
  
//Welcome Tone
  soundStartup();

//Get flash count
  flashCount = EEPROM.read(EEPROM_HISTORY_END + 8);
  flashCount = flashCount << 8;
  flashCount |= EEPROM.read(EEPROM_HISTORY_END + 9);
  display.colon(0);
}

void loop() {
  runDisplay();
  runUI();
  runSerial();
  runAudio();
}
