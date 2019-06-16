
int8_t readEncoder(){
  static const byte _p [4] = {2, 3, 1, 0};
  static byte _inLast;
  byte _in = _p[ENCODER_STATE];
  static int8_t _delta;
  if (_inLast == (_in + 1) % 4){
    _inLast = _in;
    _delta++;
  }
  else if ((_inLast + 1) % 4 == _in) {
    _inLast = _in;
    _delta--;
  }
  else _inLast = _in;
  if (_delta == 4) {
    _delta = 0;
    return 1;
  }
  else if (_delta == -4){
    _delta = 0;
    return -1;
  }
  return 0;
}

uint8_t readBtn(){  //Return 1 if short pressed & released, Return 2 if long pressed PRIOR to release
  static unsigned long debounceTimer;
  static byte buttonPos;
    if (BUTTON_PRESSED && millis() > debounceTimer){   //Button depressed
      if (!buttonPos){ buttonPos = 1; debounceTimer = millis() + BUTTON_LONGHOLD;}
      else if (buttonPos == 1){buttonPos = 2; return 2;}
    }
    else if (buttonPos && !(BUTTON_PRESSED)) {
      debounceTimer = millis() + BUTTON_DEBOUNCE;
      if (buttonPos == 1) {buttonPos = 0; return 1;}
      buttonPos = 0;
    }
    return 0;
}
