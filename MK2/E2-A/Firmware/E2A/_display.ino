void runDisplay(){
  static byte _menuPosition = 0b11111111;
  if (menuPosition != _menuPosition){
    menuPosition &= 0b01111111;
    if (menuPosition != _menuPosition) printDisplayMain();
    display.setTextColor(SSD1306_WHITE); // Draw white text
    switch (menuPosition){
      case 0:
    display.fillRect(0, 0, 128, 32, SSD1306_BLACK );
        display.setCursor(0, 6);     // Start at top-left corner
        display.setTextSize(2);
        display.print(F("STROBE"));
        display.setCursor(16, 23);     // Start at top-left corner
        display.setTextSize(1);
        display.print(F("DURATION"));
        display.setCursor(80,0);
        display.setTextSize(2);      // Normal 1:1 pixel scale
        display.cp437(true);         // Use full 256 char 'Code Page 437' font
        switch (pulseDuration){
          case 0:
            display.setCursor(78,0);
            display.print(F("0.5"));
          break;
          case 1:
            display.print(F("1"));
          break;
          case 2:
            display.print(F("2"));
          break;
          case 3:
            display.print(F("4"));
          break;
        }
        display.setTextSize(1);
        PRINT_MICROSECOND
        display.setCursor(104, 18);
        display.print(pulseCount);
        display.print(F("x"));
        if (pulseCount > 1){
          if (pulseRate < 3) display.setCursor(87, 26);
          else display.setCursor(79, 26);
          
          display.print((int)(pulseRate + 1) * 250);
          PRINT_MICROSECOND
        }
        if (flashDelay){
          if (flashDelay < 10) display.setCursor(98, 34);
          else if (flashDelay < 100) display.setCursor(92, 34);
          else if (flashDelay < 1000) display.setCursor(86, 34);
          else display.setCursor(80, 34);
          display.print(flashDelay);
          display.print("ms");
        }
        display.display();      // Show initial text
       break;


       
       case 1:
        display.fillRect(0, 0, 128, 32, SSD1306_BLACK );
        display.setCursor(4, 6);     // Start at top-left corner
        display.setTextSize(2);
        display.print(F("MULTI"));
        display.setTextSize(1);
              display.setCursor(15, 24);     // Start at top-left corner
        display.print(F("STROBE"));
                 if (pulseDuration == 0) display.setCursor(87, 0);
        else display.setCursor(97,0);
        //display.cp437(true);         // Use full 256 char 'Code Page 437' font
        printDuration();
        display.fillRect(80, 8, 44, 26, SSD1306_BLACK);
        if (pulseCount > 1) display.setCursor(80, 8);
        else display.setCursor(80, 12);
        display.setTextSize(2);
        display.print(pulseCount);
        display.setTextSize(1);
        if (pulseCount > 1) display.setCursor(89, 13);
        else display.setCursor(89, 15);
        display.print(F(" X"));
        display.setTextSize(1);
        
        if (pulseCount > 1){
          if (pulseRate < 3) display.setCursor(87, 26);
          else display.setCursor(79, 26);
          display.print((int)(pulseRate + 1) * 250);
          PRINT_MICROSECOND
        }
        if (flashDelay){
          if (flashDelay < 10) display.setCursor(98, 34);
          else if (flashDelay < 100) display.setCursor(92, 34);
          else if (flashDelay < 1000) display.setCursor(86, 34);
          else display.setCursor(80, 34);
          display.print(flashDelay);
          display.print("ms");
        }
        display.display();      // Show initial text
       break;


       
       case 2:
    display.fillRect(0, 0, 128, 32, SSD1306_BLACK );
        display.setCursor(9, 6);     // Start at top-left corner
        display.setTextSize(1);
        display.print(F("STROBE"));
        display.setTextSize(2);
        display.setCursor(2, 16);     // Start at top-left corner
        display.print(F("RATE"));
        display.setCursor(80,0);
        display.setTextSize(1);
        display.cp437(true);         // Use full 256 char 'Code Page 437' font
         if (pulseDuration == 0) display.setCursor(87, 0);
        else display.setCursor(97,0);
        printDuration();
        display.setCursor(104, 8);
        display.print(pulseCount);
        display.print(F("x"));
        //display.fillRect(64, 16, 64, 16, SSD1306_BLACK );
        display.setTextSize(2);
        display.setCursor(60, 16);
        display.print((int)(pulseRate + 1) * 250);
        display.setTextSize(1);
        PRINT_MICROSECOND;
        display.setCursor(76, 34);
        if (flashDelay){
          if (flashDelay < 10) display.setCursor(98, 34);
          else if (flashDelay < 100) display.setCursor(92, 34);
          else if (flashDelay < 1000) display.setCursor(86, 34);
          else display.setCursor(80, 34);
          display.print(flashDelay);
          display.print(F("ms"));
        }
        display.display();      // Show initial text
       break;


       
       case 3:
    display.fillRect(0, 0, 128, 47, SSD1306_BLACK );
        display.setCursor(6, 6);
        display.setTextSize(1);
        display.print("INITIAL");
        display.setTextSize(2);
              display.setCursor(0, 16);     // Start at top-left corner
        display.print("DELAY");
        display.setCursor(80,0);
        display.setTextSize(1);
        display.cp437(true);         // Use full 256 char 'Code Page 437' font
         if (pulseDuration == 0) display.setCursor(87, 0);
        else display.setCursor(97,0);
        printDuration();
        display.setCursor(104, 8);
        display.print(pulseCount);
        display.print("x");
        if (pulseCount > 1){
          if (pulseRate < 3) display.setCursor(87, 16);
          else display.setCursor(79, 16);
          display.print((int)(pulseRate + 1) * 250);
          PRINT_MICROSECOND
        }
        //display.fillRect(70, 24, 58, 16, SSD1306_BLACK );
        display.setTextSize(2);
        if (flashDelay < 10) display.setCursor(78, 24);
        else if (flashDelay < 1000) display.setCursor(70,24);
        else display.setCursor(64, 24);
        display.print(flashDelay);
        display.setTextSize(1);
        if (flashDelay < 100)  display.setCursor(92, 28);
        else if (flashDelay < 1000) display.setCursor(100, 28);
        else display.setCursor(108, 28);
        display.print(" ms");
        display.display();      // Show initial text
       break;



       
       case 4:
        int8_t _left = -2;
        if (pulseCount == 1) _left += 14;
        if (flashDelay < 1000) _left += 3;
        if (flashDelay < 100) _left += 3;
        if (flashDelay < 10) _left += 3;
        if (!flashDelay) _left += 14;
        if (pulseDuration != 0) _left += 4;
        if (pulseRate < 3) _left += 3;
        display.setCursor(_left,38);
        display.setTextSize(1);
        display.cp437(true);         // Use full 256 char 'Code Page 437' font
        if (_left == -2){ 
          display.setCursor(0, 38);
          display.print(F(".5"));
          PRINT_MICROSECOND;
        }
        else {
          display.setCursor(_left,38);
          printDuration();
        }
        display.print(F("|"));
        display.print(pulseCount);
        display.print(F("x"));
        if (pulseCount > 1){
          display.print(F("|"));
          display.print((int)(pulseRate + 1) * 250);
          PRINT_MICROSECOND
          
        }
        if (flashDelay){
          display.print(F("|"));
          display.print(flashDelay);
          display.print("ms");
        }
        display.display();      // Show initial text
       break;
       case 8:
        display.fillRect(54, 6, 45, 19, SSD1306_BLACK );
        display.setCursor(56,8);
        display.cp437(true);
        display.setTextSize(2);
        if (modellingLamp) {
          display.print(modellingLamp);
          display.setTextSize(1);
          display.print("%");
        }
        else display.print("OFF");
        display.fillRect(104, 2, 26, 16, SSD1306_BLACK );
        if (pulseRate > 99) {
          display.setTextSize(1);
          display.setCursor(104, 6);
        }
        else display.setCursor(104, 2);
        display.print(pulseRate);
        display.setTextSize(1);
        display.setCursor(106, 19);
        display.print("kHz");
        display.display();      // Show initial text
       break;
       case 16: case 17:
        display.fillRect(64, 0, 64, 32, SSD1306_BLACK );
        if (menuPosition == 16){
          display.drawTriangle(64,2,64, 8,68, 5, SSD1306_WHITE);
          display.setCursor(70,2);
          display.setTextSize(2);      // Normal 1:1 pixel scale
          display.cp437(true);         // Use full 256 char 'Code Page 437' font
          display.print(modellingLamp);
          display.print(" %");
          display.setCursor(70,18);
          display.setTextSize(1);      // Normal 1:1 pixel scale
          display.print(pulseRate);
          display.print(" kHz");
        }
        else{
          display.drawTriangle(64,12,64, 18,68, 15, SSD1306_WHITE);
          display.setCursor(70,2);
          display.setTextSize(1);      // Normal 1:1 pixel scale
          display.cp437(true);         // Use full 256 char 'Code Page 437' font
          display.print(modellingLamp);
          display.print(" %");
          display.setCursor(70,12);
          display.setTextSize(2);      // Normal 1:1 pixel scale
          display.print(pulseRate);
          display.print(" kHz");
        }
        display.display();      // Show initial text
       break;
    }
  }
  printTemp();
  printVoltage();
  _menuPosition = menuPosition;
}


void printDuration(){
  switch (pulseDuration){
    case 0:
      display.print(F("0.5"));
    break;
    case 1:
      display.print(F("1"));
    break;
    case 2:
      display.print(F("2"));
    break;
    case 3:
      display.print(F("4"));
    break;
  }
  PRINT_MICROSECOND
}

void printDisplayMain(){
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE); // Draw white text
  switch (menuPosition & 0b01111111){
    case 0: case 1: case 2: case 3:
      display.cp437(true);         // Use full 256 char 'Code Page 437' font
      display.setCursor(0, 48);
      display.print(F("Battery"));
      display.setCursor(0, 32);     // Start at top-left corner
      display.setTextSize(3);
      //display.print("STROBE");
      display.setCursor(80,0);
     break;
    case 4:
      display.cp437(true);         // Use full 256 char 'Code Page 437' font
      display.setCursor(0, 48);
      display.print(F("Battery"));
      display.setCursor(20, 10);     // Start at top-left corner
      display.setTextSize(3);
      display.print(F("READY"));
      display.setTextSize(1);
      display.setCursor(30, 0);
      display.print(F("Hold to ARM"));
      display.setCursor(80,0);
     break;
 #if (SUBMODEL != E2A)
     case 8:
      display.cp437(true);
      display.setCursor(0, 32);
      display.setTextSize(3);
      display.print("LAMP");
      display.fillRect(52, 4, 49, 23, SSD1306_WHITE);
     break;
     case 16: case 17:
      display.cp437(true);         // Use full 256 char 'Code Page 437' font
      display.setCursor(0, 32);     // Start at top-left corner
      display.setTextSize(3);
      display.print("Lamp");
     break;
 #endif
  }
}

void printTemp(){
  #if SUBMODEL != E2A
  static unsigned long tempTimer = 0;
  static int _temperature;
  static byte _menuPosition;
  if (millis() > tempTimer || menuPosition != _menuPosition){
    tempTimer = millis() + 1000;
    if (temperature != _temperature || menuPosition != _menuPosition){
      _temperature = temperature;
      switch (menuPosition){
        case 8: case 16: case 17:
          display.fillRect(8, 16, 42, 16, SSD1306_BLACK);
          display.setCursor(8, 20);
          display.setTextSize(1);
          display.print(temperature);
          display.print((char)223);
          display.print(" C");
          display.display();
        break;
      }
    }
  }
  _menuPosition = menuPosition;
  #endif
}


void printVoltage(){
  static unsigned long tempTimer = 0;
  static unsigned int _vh;
  static unsigned int _vl;
  static byte _menuPosition = 255;
  static byte _blink = 0;

  if (millis() > tempTimer || menuPosition != _menuPosition){
    tempTimer = millis() + 300;
    _blink = 1 - _blink;
    unsigned int vh = anodeVoltage();
    if (highVoltAdc != _vh || menuPosition != _menuPosition || vh >= 80){
      _vh = highVoltAdc;
      display.fillRect(102, 48, 24, 10, SSD1306_BLACK);
      if (vh > 4){
        display.setCursor(80, 48);
        display.print("LED");
        display.setCursor(102, 48);
        display.setTextSize(1);
        display.print(vh);
        display.print("V");
      }
      else {
        display.fillRect(70, 48, 30, 10, SSD1306_BLACK);
      }
      display.display();
    }
    
    float vl = batteryVoltage();
    if (lowVoltAdc != _vl || menuPosition != _menuPosition || vl <= 3.0){
      _vl = lowVoltAdc;
      display.fillRect(45, 48, 30, 10, SSD1306_BLACK);
      if (_blink || vl >= MIN_BATTERY_VOLTAGE){
        display.setCursor(45, 48);
        display.setTextSize(1);
        display.print(vl, 1);
        display.print("V");
      }
      display.display();
    }
    //if (menuPosition == 4 && vl < MIN_BATTERY_VOLTAGE) menuPosition = 0b10000000; //Return to main menu
  }
  _menuPosition = menuPosition;
}
