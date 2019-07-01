void forceFlash(){
  byte _pinNoTrigger;
  display.setSegments(TXT_nothing);
  _pinNoTrigger = PINC;
  unsigned long _trigTime;
  while (PINC == _pinNoTrigger){};
  _trigTime = micros();
  _trigTime += flashDelay;
  if (BUTTON_NOT_PRESSED){
    switch (menuPosition){
    case 0:
        while (micros() < _trigTime){};
        FLASH_ON;
        DELAY_500NS;
        FLASH_OFF;
    break;
    case 1:
        while (micros() < _trigTime){};
        FLASH_ON;
        DELAY_1US; 
        FLASH_OFF;
    break;
    case 2:
        while (micros() < _trigTime){};
        FLASH_ON;
        DELAY_2US;
        FLASH_OFF;
    break;
    case 3:
        while (micros() < _trigTime){};
        FLASH_ON;
        DELAY_4US;
        FLASH_OFF;
    break;
      default:
      break;
    }
  }
  FLASH_OFF;
  soundDone();
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
  byte _pinNoTrigger;
  display.setSegments(TXT_nothing);
  _pinNoTrigger = PINC;
  switch (menuPosition){
  case 0:
    while (PINC == _pinNoTrigger){};
      if (BUTTON_NOT_PRESSED) FLASH_ON;
      DELAY_500NS;
      FLASH_OFF;
  break;
  case 1:
    while (PINC == _pinNoTrigger){};
      if (BUTTON_NOT_PRESSED) FLASH_ON;
      DELAY_1US; 
      FLASH_OFF;
  break;
  case 2:
    while (PINC == _pinNoTrigger){};
      if (BUTTON_NOT_PRESSED) FLASH_ON;
      DELAY_2US;
      FLASH_OFF;
  break;
  case 3:
    while (PINC == _pinNoTrigger){};
      if (BUTTON_NOT_PRESSED) FLASH_ON;
      DELAY_4US;
      NOP;
      FLASH_OFF;
  break;
  default:
     break;
  }
  FLASH_OFF;
  if (BUTTON_NOT_PRESSED) updateFlashCount();
  soundDone();
  nextVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
}
