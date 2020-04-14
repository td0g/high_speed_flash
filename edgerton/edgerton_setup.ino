void initialSetup(){  //Check if button is held at startup, then run calibration routine

//is button being held for at least two seconds?
  unsigned long _t = millis() + 2000;
  while (BUTTON_PRESSED && millis() < _t){};
  if (BUTTON_PRESSED){  //Button was held for 2 seconds

//Display voltage
    display.colon();
    display.forceLineState(3, 0b00011100, 0b11100011);
    unsigned int _v;
    _v = batteryVoltage();
    _v = min(_v, 1400);
    _v = max(_v, 600);
    display.printInt(_v);

//Sound audible tick
    soundTick();

//Wait for button to release
    while (BUTTON_PRESSED){};
    delay(200);  //Debounce
    
//Calibrate battery voltage
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
    
//Write battery calibration factor to EEPROM
    float _c;
    _c  = _v;
    _c /= analogRead(INPUT_VOLT_ANALOG);
    EEPROM.put(EEPROM_HISTORY_END, _c);
    batteryVoltCal = _c;

//Calibrate capacitor voltage
    display.colon(0); //Turn off colon
    HIGH_VOLT_ON;     //Turn on capacitor charger
    display.setSegments(TXT_nothing);
    display.clearLineState();
    delay(400);
    display.forceLineState(3, 0b00011100, 0b11100011);  //Write 'v'
    while (BUTTON_PRESSED){};                     //Wait for button to be released
    delay(200);                                   //Debounce
    EEPROM.get(EEPROM_HISTORY_END + 4, _c);       //Retrieve current calibration factor
    if(isnan(_c) || isinf(_c) || _c < 0 || _c > 0.5) _c = 0.25;  //If current calibration factor isn't available, set it to abritrary value
    unsigned long _timer = millis();
    while (BUTTON_NOT_PRESSED){
      int8_t _e = readEncoder();
      if (_e != 0 || millis() > _timer){

        //Get good analog reading from high-voltage analog pin
        unsigned int _a = 0;
        for (byte i = 0; i < 8; i++) _a += analogRead(HIGH_VOLT_OUT_ANALOG);
        _a /= 8;
        _v = _a * _c;   //Calculated voltage
        if (_a > 1020) display.printInt(9999);  //Indicating that signal is clipping
        else {
          if (_e != 0){   //Encoder was turned
            _v += _e;
            _c = (float)_v / _a;
            //display.printInt(_v * 10);  
          }
          display.printInt(_v * 10);  //Multiply by 10 to shift left for 'v' char
        }
        _timer = millis() + 500;
      }
    }
    EEPROM.put(EEPROM_HISTORY_END + 4, _c); //Button was pressed, save calibration factor
    capacitorVoltCal = _c;
    HIGH_VOLT_OFF;
    
//Reset display to blank state
    display.setSegments(TXT_nothing);
    display.clearLineState();

//Wait for button to release
    while (BUTTON_PRESSED){}; 
    delay(50);  //Debounce
  }
}
