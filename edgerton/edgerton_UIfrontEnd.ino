//High-level UI functions

void runUI(){ //Recieves user input and affects menu options and settings
  static unsigned long _t;
  static unsigned long _lastInput = DISPLAY_TIMEOUT;

//Respond to encoder input
  int8_t _e = readEncoder();
  switch (menuPosition){
    case 0: //Change flash duration
      if (_e == 1 && flashDuration < 3){flashDuration++; soundTick(); REFRESH_DISPLAY;}
      else if (_e == -1 && flashDuration){flashDuration--; soundTick(); REFRESH_DISPLAY;}
    break;
    case 1: //Change flash delay
      if (_e == 1 && flashDelay < 9000000){
        if (flashDelay < 1000) flashDelay += 50; 
        else if (flashDelay < 2000) flashDelay += 100;
        else if (flashDelay < 5000) flashDelay += 500;
        else if (flashDelay < 10000) flashDelay += 1000;
        else if (flashDelay < 20000) flashDelay += 2000;
        else if (flashDelay < 50000) flashDelay += 5000;
        else flashDelay += 10000;
        soundTick(); 
        REFRESH_DISPLAY;
      }
      else if (_e == -1 && flashDelay){
        if (flashDelay < 1001) flashDelay -= 50; 
        else if (flashDelay < 2001) flashDelay -= 100;
        else if (flashDelay < 10001) flashDelay -= 1000;
        else if (flashDelay < 20001) flashDelay -= 2000;
        else flashDelay -= 5000;
        soundTick(); 
        REFRESH_DISPLAY;
      }
    break;
    case 2:
    break;
  }

//Respond to button input
  byte _b = readBtn();
  if (_b == 1) {menuPosition = (menuPosition + 1) % 2; soundTick(); display.colon(0);}  //Change between flash duration / delay menus
  else if (_b == 2) flashRoutine();

//No button or encoder input for some time?  Darken display.  Was there an input?  Make sure display is bright.
  if (_e != 0 || _b != 0) {
    if (!_lastInput) display.setBrightness(7);
    _lastInput = millis() + DISPLAY_TIMEOUT;
  }
  else if (_lastInput && _lastInput < millis()) {
    _lastInput = 0;
    display.setBrightness(1);
  }
}

void runDisplay(){  //Writes to display
  static unsigned long _timer = 0;
  static byte _menuPosition;
  static byte _switch;
  byte txt500[4] = {0b00111111, 0b01101101, 0b00011100, 0b00101101};
  byte txt1000[4] = {0b00000110, 0, 0b00011100, 0b00101101};
  byte txt2000[4] = {0b01011011, 0, 0b00011100, 0b00101101};
  byte txt4000[4] = {0b01100110, 0, 0b00011100, 0b00101101};

//If menu position has changed, refresh display
  if (_menuPosition != menuPosition) {
    display.clearLineState();
    display.colon(0);
    _switch = 3;
    _timer = millis() - 1;
    menuPosition &= 0b01111111;
   _menuPosition = menuPosition;
  }
  
//Display battery voltage occasionally
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
          if (flashDelay > 9000) {
            display.colon(0);
            display.printInt(flashDelay / 1000);
          }
          else {
            display.colon(1);
            display.printInt(flashDelay / 10);
          }
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
      display.colon();
      display.printInt(batteryVoltage());
    }
  }
}
