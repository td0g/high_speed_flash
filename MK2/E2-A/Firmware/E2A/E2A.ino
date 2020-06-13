  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #include <EEPROM.h>
  
  #define SCREEN_WIDTH 128 // OLED display width, in pixels
  #define SCREEN_HEIGHT 64 // OLED display height, in pixels
  
  #define SOUND
  
  #define SUBMODEL E2A
  
  #define BUTTON_LONGHOLD 600
  #define BUTTON_DEBOUNCE 25
  // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
  #define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  #define EEPROM_BATTERY_CAL_2V 82
  #define EEPROM_BATTERY_CAL_6V 209
  #define EEPROM_ANODE_CAL 150
  #define BUTTON_PIN 2
  #define ENCODER_A_PIN 3
  #define ENCODER_B_PIN 4
  #define BUTTON_PRESSED (!(PIND & 0b00000100))
  #define BUTTON_NOT_PRESSED PIND & 0b00000100
  #define ENCODER_STATE ((PIND >> 3) & 0b00000011)
  #define HIGH_POWER_BUCK_CONTROL_PIN 5 //Output Low
  #define FLASH_PIN 6 //Output Low
  #define FLASH_ON PORTD = 0b01011100 //Keep encoder/button pullups on
  #define FLASH_OFF PORTD = 0b00011100 //Keep encoder/button pullups on
  #define HIGH_POWER_BUCK_CONTROL_A_PIN 7 //Output Low
  #define HIGH_POWER_BUCK_CONTROL_B_PIN 8 //Output Low
  #define SOUND_PIN 9 //Output Low
  #define HIGH_VOLTAGE_CONTROL_PIN 11 //Output Low
  #define LOW_POWER_BOOST_CONTROL_PIN 12  //Output Low
  #define INPUT_VOLTAGE_PIN 14 //Input 1/4.3 voltage divider
  #define CAPACITOR_VOLTAGE_PIN 15 //Input 2.7/70.7 voltage divider
  #define TRIGGER_PIN 17  //Input
  #define DISPLAY_SDA_PIN 18
  #define DISPLAY_SCL_PIN 19
  
  #define TRIGGER_HIGH (PINC & 0b00001000)
  #define TRIGGER_HIGH_BUTTON_NOT_PRESSED (PINC & 0b00011000)
  #define TRIGGER_LOW (!(PINC & 0b00001000))
  #define TRIGGER_LOW_BUTTON_NOT_PRESSED (!(PINC & 0b00001000) && (PINC & 0b00010000))

  #define MAX_PULSES 6
  #define MAX_PULSE_RATE 7
  #define MIN_ANODE_VOLTAGE 40
  #define MIN_BATTERY_VOLTAGE 2.95

  #define EEPROM_BATTERY_CAL_2V 50
  #define EEPROM_BATTERY_CAL_6V 100
  #define EEPROM_ANODE_CAL 150
  #define SERIAL_NUMBER 1
  #define FIRMWARE_VERSION "1.0"

  #define HIGH_VOLT_ON digitalWrite(HIGH_VOLTAGE_CONTROL_PIN, 1)
  #define HIGH_VOLT_OFF digitalWrite(HIGH_VOLTAGE_CONTROL_PIN, 0)
  
  //pulseDuration Calibration
  #define NOP __asm__ __volatile__ ("nop\n\t")
  #define DELAY_500_NS NOP; NOP; NOP; NOP; NOP
  #define DELAY_1US DELAY_500_NS; DELAY_500_NS; NOP; NOP; NOP
  #define DELAY_2US DELAY_1US; DELAY_1US; NOP; NOP; NOP
  #define DELAY_4US DELAY_2US; DELAY_2US; NOP; NOP; NOP

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

  #define PRINT_MICROSECOND display.write(230); display.print("s");

  byte menuPosition = 0b10000000;
  byte modellingLamp = 0;
  byte modellingLampLast = 1;
  byte pulseDuration = 1;
  unsigned int flashDelay = 0;
  byte pulseCount = 1;
  unsigned int pulseRate = 1;
  int temperature;
  byte activeTrigger;
  unsigned int delayTime[8] = {4000, 8000, 12000, 16000, 20000, 24000, 28000, 32000};
  byte soundOn = 1;
  byte flipDisplay = 1;
  byte reverseEncoder = 0;
  
  unsigned int lowVoltAdc = 0;
  unsigned int highVoltAdc = 0;
  unsigned int calBattery2V;
  unsigned int calBattery6V;
  byte calAnode;
  byte voltageSafetyOverride = 0;


void setup() {
  digitalWrite(FLASH_PIN, 0);
  pinMode(FLASH_PIN, OUTPUT);
  pinMode(LOW_POWER_BOOST_CONTROL_PIN, OUTPUT);
  pinMode(HIGH_VOLTAGE_CONTROL_PIN, OUTPUT);
  pinMode(ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(ENCODER_B_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(SOUND_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  importEEPROM();
  if (flipDisplay) display.setRotation(2);
  Serial.println("E2|A Ready");
  soundStartup();
  //ADC & UI
  ADCSRA =  bit (ADEN);   // turn ADC on
  ADCSRA |= bit (ADPS0) | bit (ADPS1) | bit (ADPS2);
  
//Timer1 for multi-strobe
  TCCR1A = 0;
  TCCR1B = 0b00000001;         // div1 = 1/16us per increment, 16-bit normal mode
  while (BUTTON_PRESSED && millis() < 5000){
    runDisplay();
    runTrigger();
    analogReadAll();
  }
  if (BUTTON_PRESSED){
    initMenu();
  }
}

void loop(){
  runDisplay();
  runUI();
  runTrigger();
  analogReadAll();
}




/*
  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);


void testdrawline() {
  int16_t i;

  display.clearDisplay(); // Clear display buffer

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn line
    delay(1);
  }
  for(i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000); // Pause for 2 seconds
}

void testdrawrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

void testfillrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=3) {
    // The INVERSE color is used so rectangles alternate white/black
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, SSD1306_INVERSE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

void testdrawcircle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfillcircle(void) {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
    // The INVERSE color is used so circles alternate white/black
    display.fillCircle(display.width() / 2, display.height() / 2, i, SSD1306_INVERSE);
    display.display(); // Update screen with each newly-drawn circle
    delay(1);
  }

  delay(2000);
}

void testdrawroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfillroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    // The INVERSE color is used so round-rects alternate white/black
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, SSD1306_INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testdrawtriangle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfilltriangle(void) {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
    // The INVERSE color is used so triangles alternate white/black
    display.fillTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, SSD1306_INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}

void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}
#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };
void testdrawbitmap(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}

    // Draw each snowflake:
    for(f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
    }

}
*/
