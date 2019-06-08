void forceFlash(){
  switch (menuPosition){
  case 0:
      FLASH_ON;
      NOP;
      FLASH_OFF;
  break;
  case 1:
      FLASH_ON;
      NOP; NOP; NOP; NOP; NOP; 
      FLASH_OFF;
  break;
  case 2:
      FLASH_ON;
      DELAY_500_NS;
      DELAY_500_NS;
      DELAY_500_NS;
      NOP; NOP; NOP;
      FLASH_OFF;
  break;
  case 3:
      FLASH_ON;
      DELAY_500_NS;
      DELAY_500_NS;
      DELAY_500_NS;
      DELAY_500_NS;
      DELAY_500_NS;
      DELAY_500_NS;
      DELAY_500_NS;
      NOP;
      FLASH_OFF;
  break;
    default:
    break;
  }
  FLASH_OFF;
  updateFlashCount();
  nextVolt2 = 0;
  for (byte i = 0; i < 8; i++) nextVolt2 += analogRead(HIGH_VOLT_OUT_ANALOG);
  nextVolt2 = nextVolt2 >> 3;
  delay(1);
  nextVolt = 0;
  for (byte i = 0; i < 8; i++) nextVolt += analogRead(HIGH_VOLT_OUT_ANALOG);
  nextVolt = nextVolt >> 3;
}


void runFlash(){
  display.setSegments(nothing);
  switch (menuPosition){
  case 0:
    while (TRIGGER_HIGH_BUTTON_NOT_PRESSED){};
      if (BUTTON_NOT_PRESSED) FLASH_ON;
      NOP;
      FLASH_OFF;
  break;
  case 1:
    while (TRIGGER_HIGH_BUTTON_NOT_PRESSED){};
      if (BUTTON_NOT_PRESSED) FLASH_ON;
      NOP; NOP; NOP; NOP; NOP; 
      FLASH_OFF;
  break;
  case 2:
    while (TRIGGER_HIGH_BUTTON_NOT_PRESSED){};
      if (BUTTON_NOT_PRESSED) FLASH_ON;
      DELAY_500_NS;
      DELAY_500_NS;
      DELAY_500_NS;
      NOP; NOP; NOP;
      FLASH_OFF;
  break;
  case 3:
    while (TRIGGER_HIGH_BUTTON_NOT_PRESSED){};
      if (BUTTON_NOT_PRESSED) FLASH_ON;
      DELAY_500_NS;
      DELAY_500_NS;
      DELAY_500_NS;
      DELAY_500_NS;
      DELAY_500_NS;
      DELAY_500_NS;
      DELAY_500_NS;
      NOP;
      FLASH_OFF;
  break;
  default:
     break;
  }
  FLASH_OFF;
  if (BUTTON_NOT_PRESSED) updateFlashCount();
  delay(50);
  nextVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
}



void chargeFlash(){
  byte i = 10;
  unsigned long _t;
  display.clearLineState();
  display.printInt(analogRead(HIGH_VOLT_OUT_ANALOG) * 4 / 9);
  delay(250);
  highVolt = 0;
  HIGH_VOLT_ON;
  _t = millis() + 500;
  while (millis() < _t) display.printInt(analogRead(HIGH_VOLT_OUT_ANALOG)/ 8);
  while (i && analogRead(HIGH_VOLT_OUT_ANALOG) > highVolt + 1){
    readBtn();
    i--;
    highVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
    _t = millis() + 500;
    while (millis() < _t || BUTTON_PRESSED) display.printInt(analogRead(HIGH_VOLT_OUT_ANALOG) / 8);
  }
  highVolt2 = analogRead(HIGH_VOLT_OUT_ANALOG);
  highVolt = 0;
  for (byte i = 0; i < 8; i++) highVolt += analogRead(HIGH_VOLT_OUT_ANALOG);
  highVolt = highVolt >> 3;
}


void initializeFlash(){
  /*
   * ERRORS:
   * ERR 0 - Capacitor already charged prior to initiation
   * ERR 1 - High-Volt on briefly, no charge
   * ERR 2 - High-Volt off briefly, no discharge
   * ERR 3 - High-Volt on, charge not reaching full
   * ERR 4 - Flash activated, no discharge
   */
  if (flashState == 254){
    flashState = 0;
    highVolt = analogRead(INPUT_VOLT_ANALOG) * 20;
    Serial.print("highVolt ");
    Serial.println(highVolt);
    display.printInt(analogRead(HIGH_VOLT_OUT_ANALOG));
    if (highVolt > 110) {
      highVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
      Serial.print("highVolt ");
      Serial.println(highVolt);
      flashState =  1;
      delay(500);
      HIGH_VOLT_ON;
      unsigned long _t;
      _t = millis() + 1000;
      while (millis() < _t) display.printInt(analogRead(HIGH_VOLT_OUT_ANALOG));
      Serial.println(analogRead(HIGH_VOLT_OUT_ANALOG));
      if (analogRead(highVoltPin) > highVolt) {
        flashState = 2;
        highVolt = analogRead(highVoltPin);
        _t = millis() + 3000;
        HIGH_VOLT_OFF;
        highVolt = highVolt - min(highVolt, 10);
        while (analogRead(highVoltPin) > highVolt && millis() < _t)display.printInt(analogRead(HIGH_VOLT_OUT_ANALOG));
        if (millis() < _t) {
          flashState = 3;
          HIGH_VOLT_ON;
          highVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
          _t = millis() + 500;
          while (millis() < _t) display.printInt(analogRead(HIGH_VOLT_OUT_ANALOG));
          byte i = 10;
          while (i && analogRead(HIGH_VOLT_OUT_ANALOG) > highVolt + 1){
            i--;
            highVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
            _t = millis() + 500;
            while (millis() < _t) display.printInt(analogRead(HIGH_VOLT_OUT_ANALOG));
          }
         if (i){
            flashState = 4;
            HIGH_VOLT_OFF;
            delay(10);
            int _highVolt;
            highVolt = analogRead(highVoltPin);
            flashDuration = 1;
            //forceFlash();
            _highVolt = analogRead(highVoltPin);
            if (highVolt < _highVolt - 2) flashState = 255;
           flashState = 255;
         }
        }
      }
    }
  }
  HIGH_VOLT_OFF;
}
