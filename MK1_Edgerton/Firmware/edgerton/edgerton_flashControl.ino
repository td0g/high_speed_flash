//Functions to control flash

void forceFlash(){    //flashes after delay
  unsigned long _trigTime;
  
//Clear display
  display.setSegments(TXT_nothing);

//Get current state of PINC
  byte _pinNoTrigger;
  _pinNoTrigger = PINC;

//Wait for change in pin state
  while (PINC == _pinNoTrigger){};

//Start countdown timer
  _trigTime = micros();
  _trigTime += flashDelay;

//If button is not pressed, then wait for countdown timer to expire then flash
  if (BUTTON_NOT_PRESSED){
    switch (flashDuration){
    case 0:
        while (micros() < _trigTime){};
        cli();
        FLASH_ON;
        DELAY_500NS;
        FLASH_OFF;
        sei();
    break;
    case 1:
        while (micros() < _trigTime){};
        cli();
        FLASH_ON;
        DELAY_1US; 
        FLASH_OFF;
        sei();
    break;
    case 2:
        while (micros() < _trigTime){};
        cli();
        FLASH_ON;
        DELAY_2US;
        FLASH_OFF;
        sei();
    break;
    case 3:
        while (micros() < _trigTime){};
        cli();
        FLASH_ON;
        DELAY_4US;
        FLASH_OFF;
        sei();
    break;
      default:
      break;
    }
  }
  FLASH_OFF;
  
//Signal flash is done with audio
  soundDone();

//Export data to EEPROM
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
  
//Clear display
  display.setSegments(TXT_nothing);

//Get current state of PINC
  byte _pinNoTrigger;
  _pinNoTrigger = PINC;

//Disable all interrupts
  cli();
  
//Wait for trigger (PINC state changes), then fire flash immediately if button is not pressed
  switch (flashDuration){
  case 0:
    while (PINC == _pinNoTrigger){};
      if (BUTTON_NOT_PRESSED) FLASH_ON;
      DELAY_500NS;
      FLASH_OFF;
      sei(); //Enable interrupts
  break;
  case 1:
    while (PINC == _pinNoTrigger){};
      if (BUTTON_NOT_PRESSED) FLASH_ON;
      DELAY_1US; 
      FLASH_OFF;
      sei(); //Enable interrupts
  break;
  case 2:
    while (PINC == _pinNoTrigger){};
      if (BUTTON_NOT_PRESSED) FLASH_ON;
      DELAY_2US;
      FLASH_OFF;
      sei(); //Enable interrupts
  break;
  case 3:
    while (PINC == _pinNoTrigger){};
      if (BUTTON_NOT_PRESSED) FLASH_ON;
      DELAY_4US;
      FLASH_OFF;
      sei(); //Enable interrupts
  break;
  default:
     break;
  }
  FLASH_OFF;



//Signal flash is done with audio
  soundDone();
  
//Export data to EEPROM
  if (BUTTON_NOT_PRESSED) updateFlashCount();
  nextVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
}
