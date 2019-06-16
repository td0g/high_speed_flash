void runUI(){
  static unsigned long _t;
  static unsigned long _lastInput = DISPLAY_TIMEOUT;

  int8_t _e = readEncoder();
  switch (menuPosition){
    case 0:
      if (_e == 1 && flashDuration < 3){flashDuration++; soundTick(); REFRESH_DISPLAY;}
      else if (_e == -1 && flashDuration){flashDuration--; soundTick(); REFRESH_DISPLAY;}
    break;
    case 1:
      if (_e == 1 && flashDelay < 9000){
        if (flashDelay < 100) flashDelay++; 
        else if (flashDelay < 200) flashDelay += 2;
        else if (flashDelay < 500) flashDelay += 5;
        else if (flashDelay < 1000) flashDelay += 10;
        else if (flashDelay < 2000) flashDelay += 20;
        else flashDelay += 50;
        soundTick(); 
        REFRESH_DISPLAY;
      }
      else if (_e == -1 && flashDelay){
        if (flashDelay < 101) flashDelay--; 
        else if (flashDelay < 201) flashDelay -= 2;
        else if (flashDelay < 501) flashDelay -= 5;
        else if (flashDelay < 1001) flashDelay -= 10;
        else if (flashDelay < 2001) flashDelay -= 20;
        else flashDelay -= 50;
        soundTick(); 
        REFRESH_DISPLAY;
      }
    break;
    case 2:
    break;
  }

  byte _b = readBtn();
  if (_b == 1) {menuPosition = (menuPosition + 1) % 2; soundTick(); display.colon(0);}
  else if (_b == 2) flashRoutine();

  if (_e != 0 || _b != 0) {
    if (!_lastInput) display.setBrightness(7);
    _lastInput = millis() + DISPLAY_TIMEOUT;
  }
  else if (_lastInput && _lastInput < millis()) {
    _lastInput = 0;
    display.setBrightness(1);
  }
}

void runDisplay(){
  static unsigned long _timer = 0;
  static byte _menuPosition;
  static byte _switch;
          byte txt500[4] = {0b00111111, 0b01101101, 0b00011100, 0b00101101};
          byte txt1000[4] = {0b00000110, 0, 0b00011100, 0b00101101};
          byte txt2000[4] = {0b01011011, 0, 0b00011100, 0b00101101};
          byte txt4000[4] = {0b01100110, 0, 0b00011100, 0b00101101};

  if (_menuPosition != menuPosition) {
      display.clearLineState();
      display.colon(0);
    _switch = 3;
    _timer = millis() - 1;
    menuPosition &= 0b01111111;
   _menuPosition = menuPosition;
  }
  
  if (millis() > _timer){
    _switch = (_switch + 1) % 4;
    if (!_switch){
      _timer = millis() + 1000;
      if (batteryVoltage() > MIN_BATTERY_VOLTAGE) _timer = millis() + 4000;
      switch (menuPosition){
        case 0:
          if (!flashDuration) display.setSegments(txt500);
          else if (flashDuration == 1) display.setSegments(txt1000);
          else if (flashDuration == 2) display.setSegments(txt2000);
          else if (flashDuration == 3) display.setSegments(txt4000);
        break;
        case 1:
          display.printInt(flashDelay);
        break;
        case 2:
        break;
      }
    }
    else if (_switch & 1){
      display.colon(0);
      display.setSegments(TXT_nothing);
      display.clearLineState();
      _timer = millis() + 250;
    }
    else {
      display.forceLineState(3, 0b00011100, 0b11100011);
    _timer = millis() + 1000;
      lowVolt += analogRead(INPUT_VOLT_ANALOG) * 20;
      lowVolt = lowVolt >> 1;
      display.colon();
      display.printInt(lowVolt);
    }
  }
}
