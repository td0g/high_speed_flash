void initialSetup(){
  unsigned long _t = millis() + 2000;
  while (BUTTON_PRESSED && millis() < _t){};
  if (BUTTON_PRESSED){
    display.colon();
    display.forceLineState(3, 0b00011100, 0b11100011);
    unsigned int _v;
    _v = batteryVoltage();
    _v = min(_v, 1400);
    _v = max(_v, 600);
    display.printInt(_v);
    soundTick();
    while (BUTTON_PRESSED){};
    delay(200);
    while (BUTTON_NOT_PRESSED){
      int8_t _e = readEncoder();
      if (_e == 1 && _v < 2000){
        _v += 10;
        display.printInt(_v);
      }
      else if (_e == -1 && _v > 500){
        _v -= 10;
        display.printInt(_v);
      }
    }
    float _c;
    _c  = _v;
    _c /= analogRead(INPUT_VOLT_ANALOG);
    EEPROM.put(EEPROM_HISTORY_END, _c);

    display.colon(0);
    HIGH_VOLT_ON;
    display.setSegments(TXT_nothing);
    display.clearLineState();
    delay(400);
    display.forceLineState(3, 0b00011100, 0b11100011);
    while (BUTTON_PRESSED){};
    delay(200);

    _v = 450;
    display.printInt(_v);
    while (BUTTON_NOT_PRESSED){
      int8_t _e = readEncoder();
      if (_e == 1 && _v < 5000){
        _v += 10;
        display.printInt(_v);
      }
      else if (_e == -1 && _v > 200){
        _v -= 10;
        display.printInt(_v);
      }
    }
    _v /= 10;
    _c  = _v;
    _c /= analogRead(HIGH_VOLT_OUT_ANALOG);
    EEPROM.put(EEPROM_HISTORY_END + 4, _c);

    
    HIGH_VOLT_OFF;
    display.setSegments(TXT_nothing);
    display.clearLineState();
    while (BUTTON_PRESSED){};
    delay(50);
    EEPROM.get(EEPROM_HISTORY_END, batteryVoltCal);
    EEPROM.get(EEPROM_HISTORY_END + 4, highVoltCal);
    
  }
}
