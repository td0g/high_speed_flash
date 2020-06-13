void initMenu(){
  byte _m = 0;
  printInitMenu(_m);
  while (BUTTON_PRESSED){};
  while (_m != 255){
    byte _b = readBtn();
    int8_t _e = readEncoder();
    if (_b == 1) {
      _m++;
      printInitMenu(_m);
      if (_m == 7) _m = 255;
    }
    else if (_b == 2) _m = 255;
    if (_e != 0){
      switch (_m){
        case 0:
          reverseEncoder = 1 - reverseEncoder;
        break;
        case 1:
          flipDisplay = 1 - flipDisplay;
          if (flipDisplay) display.setRotation(2);
          else display.setRotation(0);
          display.display();
        break;
        case 2:
          soundOn = 1 - soundOn;
        break;
        case 3:
          if (_e == 1 || calBattery2V) calBattery2V += _e;
        break;
        case 4:
          if (_e == 1 || calBattery6V) calBattery6V += _e;
        break;
        case 5:
          if (_e == 1 || calAnode) calAnode += _e;
        break;
      }
      soundTick();
      printInitMenu(_m);
    }
  }
  updateSettingsEEPROM();
  menuPosition = 0b10000000;
  printDisplayMain();
}

void printInitMenu(byte _m){
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setTextSize(1);
  if (_m < 6){
    display.setCursor(10, 0);
    if (_m == 0) display.print(">");
    display.print(F("ReverseEncoder "));
    display.print(reverseEncoder);
    display.setCursor(10, 9);
    if (_m == 1) display.print(">");
    display.print(F("FlipDisplay "));
    display.print(flipDisplay);
    display.setCursor(10, 18);
    if (_m == 2) display.print(">");
    display.print(F("SoundOn "));
    display.print(soundOn);
    display.setCursor(10, 27);
    if (_m == 3) display.print(F(">"));
    display.print(F("CalBattery2V "));
    display.print(calBattery2V);
    display.setCursor(10, 36);
    if (_m == 4) display.print(F(">"));
    display.print(F("CalBattery6V "));
    display.print(calBattery6V);
    display.setCursor(10, 45);
    if (_m == 5) display.print(F(">"));
    display.print(F("CalAnode "));
    display.print(calAnode);
  }
  else {
      display.setCursor(10, 0);
      display.print(F("0.5"));
      PRINT_MICROSECOND;
      display.print(F(": "));
      display.print(readFlashCount(0));
      display.setCursor(10, 9);
      display.print(F("1.0"));
      PRINT_MICROSECOND;
      display.print(F(": "));
      display.print(readFlashCount(1));
      display.setCursor(10, 18);
      display.print(F("2.0"));
      PRINT_MICROSECOND;
      display.print(F(": "));
      display.print(readFlashCount(2));
      display.setCursor(10, 27);
      display.print(F("4.0"));
      PRINT_MICROSECOND;
      display.print(F(": "));
      display.print(readFlashCount(3));
      display.setCursor(10, 36);
      display.print(F("TOTAL: "));
      display.print(readFlashCount(4));
  }
  display.setCursor(10, 54);
  display.print(F("F/W: "));
  display.print(FIRMWARE_VERSION);
  display.setCursor(64, 54);
  display.print(F("S/N: 0"));
  display.print(SERIAL_NUMBER);
  display.display();
}
