void flashRoutine(){
  if (batteryVoltage() < MIN_BATTERY_VOLTAGE * 10){
    soundError(1);
  }
  else {
    chargeFlash();
        
    if (!flashDelay) runFlash();
    else forceFlash();
    
    HIGH_VOLT_OFF;
    
    if (BUTTON_NOT_PRESSED) {
      recordPerformance();
      unsigned long _t;
      _t = millis() + 3000;
      while (millis() < _t && BUTTON_NOT_PRESSED){};
      float _c;
      _c = (nextVolt - highVolt) / 8;
      _c *= CAPACITANCE_UF;
      _c *= 2;
      byte x[4] = {1, 2, 4, 8};
      _c /= x[menuPosition];
      _c *= 100;
    
      nextVolt *= 10000;
      nextVolt /= highVolt;
    
      display.forceLineState(2, 0, 0b01111111);
      display.forceLineState(3, 0b01110011, 0b00001100);
      display.printInt(nextVolt);
      _t = millis() + 1000;
      while (millis() < _t && BUTTON_NOT_PRESSED){};
      display.setSegments(TXT_nothing);
      display.clearLineState();
      _t = millis() + 250;
      while (millis() < _t && BUTTON_NOT_PRESSED){};
      nextVolt = _c;
      display.forceLineState(2, 0, 0b01111111);
      display.forceLineState(3, 0b01110111, 0b00001000);
        display.printInt(nextVolt);
        _t = millis() + 1000;
        while (millis() < _t && BUTTON_NOT_PRESSED){};
        display.setSegments(TXT_nothing);
        display.clearLineState();
        _t = millis() + 250;
        while (millis() < _t && BUTTON_NOT_PRESSED){};
        REFRESH_DISPLAY;
        while (!TRIGGER_HIGH || BUTTON_PRESSED){}; 
      }
  }
}

void chargeFlash(){
  byte i = 10;
  unsigned long _t;
  display.clearLineState();
  display.printInt(analogRead(HIGH_VOLT_OUT_ANALOG) * highVoltCal);
  highVolt = 0;
  HIGH_VOLT_ON;
  soundCharging();
  _t = millis() + 500;
  while (millis() < _t) display.printInt(capacitorVoltage());
  while (i && analogRead(HIGH_VOLT_OUT_ANALOG) > highVolt + 1){
    readBtn();
    i--;
    highVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
    _t = millis() + 400;
    soundTick();
    while (millis() < _t || BUTTON_PRESSED) display.printInt(analogRead(HIGH_VOLT_OUT_ANALOG) * highVoltCal);
  }
  highVolt2 = analogRead(HIGH_VOLT_OUT_ANALOG);
  highVolt = 0;
  for (byte i = 0; i < 8; i++) highVolt += analogRead(HIGH_VOLT_OUT_ANALOG);
  highVolt = highVolt >> 3;
  soundDone();
}
