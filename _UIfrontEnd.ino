void runUI(){
  static unsigned long _t;
  
  menuPosition += readEncoder();
  if (menuPosition == 255) menuPosition = 0;
  else if (menuPosition == 4) menuPosition = 3;

  byte _b = readBtn();
  if (menuPosition < 5){
    if (_b){
      byte nothing[4] = {0, 0, 0, 0};
      chargeFlash();
      if (_b == 2){
        display.setSegments(nothing);
        forceFlash();
        HIGH_VOLT_OFF;     
        recordPerformance();
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
        display.showNumberDec(nextVolt);
          unsigned long _t;
          _t = millis() + 1000;
          while (millis() < _t && BUTTON_NOT_PRESSED){};
        display.setSegments(nothing);
        display.clearLineState();
          _t = millis() + 250;
          while (millis() < _t && BUTTON_NOT_PRESSED){};
        nextVolt = _c;
        display.forceLineState(2, 0, 0b01111111);
        display.forceLineState(3, 0b01110111, 0b00001000);
        display.showNumberDec(nextVolt);
          _t = millis() + 1000;
          while (millis() < _t && BUTTON_NOT_PRESSED){};
        display.setSegments(nothing);
        display.clearLineState();
          _t = millis() + 250;
          while (millis() < _t && BUTTON_NOT_PRESSED){};
        REFRESH_DISPLAY;
      }
      else if (_b == 1){
        runFlash();
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
          display.showNumberDec(nextVolt);
          _t = millis() + 1000;
          while (millis() < _t && BUTTON_NOT_PRESSED){};
          display.setSegments(nothing);
          display.clearLineState();
          _t = millis() + 250;
          while (millis() < _t && BUTTON_NOT_PRESSED){};
          nextVolt = _c;
          display.forceLineState(2, 0, 0b01111111);
          display.forceLineState(3, 0b01110111, 0b00001000);
          display.showNumberDec(nextVolt);
          _t = millis() + 1000;
          while (millis() < _t && BUTTON_NOT_PRESSED){};
          display.setSegments(nothing);
          display.clearLineState();
          _t = millis() + 250;
          while (millis() < _t && BUTTON_NOT_PRESSED){};
        }
        REFRESH_DISPLAY;
        while (!TRIGGER_HIGH || BUTTON_PRESSED){};
      }
     
    }
  }
  else {
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
  }
}

void runDisplay(){
  static unsigned long _timer = 0;
  static byte _menuPosition;
  static byte _switch;
          byte txt500[4] = {0b00111111, 0b01101101, 0b00111110, 0b00101101};
          byte txt1000[4] = {0b00000110, 0, 0b00111110, 0b00101101};
          byte txt2000[4] = {0b01011011, 0, 0b00111110, 0b00101101};
          byte txt4000[4] = {0b01100110, 0, 0b00111110, 0b00101101};

  if (_menuPosition != menuPosition) {
      display.clearLineState();
    _switch = 3;
    _timer = millis() - 1;
    menuPosition &= 0b01111111;
   _menuPosition = menuPosition;
  }
  
  if (millis() > _timer){
    _switch = (_switch + 1) % 4;
    if (!_switch){
    _timer = millis() + 3000;
      display.forceLineState(3, 255, 255);
      switch (menuPosition){
        case 0:
          display.setSegments(txt500);
        break;
        case 1:
          display.setSegments(txt1000);
        break;
        case 2:
          display.setSegments(txt2000);
        break;
        case 3:
          display.setSegments(txt4000);
        break;
      }
    }
    else if (_switch & 1){
      display.setSegments(nothing);
      display.clearLineState();
      _timer = millis() + 250;
    }
    else {
      display.forceLineState(3, 0b00011100, 0b11100011);
    _timer = millis() + 1000;
      lowVolt += analogRead(INPUT_VOLT_ANALOG) * 20;
      lowVolt = lowVolt >> 1;
      display.showNumberDec(lowVolt);
    }
  }
}
