//The only functions which can execute strobes!

void enterFullyArmedState(){
  //Wait for trigger to enter non-active state
    soundDone();
    delay(200); //Waiting for sound to finish
    while (!activeTrigger && TRIGGER_LOW){};
    while (activeTrigger && TRIGGER_HIGH){};

  //Notify user that strobe is armed
    display.clearDisplay();
    display.display();

  //Execute
    runFlash();

  //Notify user that trigger has occurred and wait for trigger to enter non-active state
    soundDone();
    menuPosition = 4 | 0b10000000;
    delay(800);
    while (!activeTrigger && TRIGGER_LOW){};
    while (activeTrigger && TRIGGER_HIGH){};
    printDisplayMain();
}

void runFlash(){
  byte _pinNoTrigger;
  _pinNoTrigger = PINC & 0b00001000;
  unsigned int _t = delayTime[pulseRate];
  if (!flashDelay){
    cli();
    switch (pulseDuration){
    case 0:
      while ((PINC & 0b00001000) == _pinNoTrigger){};
      TCNT1 = 0;
      FLASH_ON;
      DELAY_500_NS;
      FLASH_OFF;
      for (byte i = 1; i < pulseCount; i++){         
        while(TCNT1 > _t){};
        while (TCNT1 < _t){};
        FLASH_ON;
        DELAY_500_NS;
        FLASH_OFF;
        _t += delayTime[pulseRate];
      }
    break;
    case 1:
      while ((PINC & 0b00001000) == _pinNoTrigger){};
          TCNT1 = 0;
      FLASH_ON;
      DELAY_1US;
      FLASH_OFF;
      for (byte i = 1; i < pulseCount; i++){        
        while(TCNT1 > _t){};
        while (TCNT1 < _t){};
        FLASH_ON;
        DELAY_1US;
        FLASH_OFF;
        _t += delayTime[pulseRate];
      }
    break;
    case 2:
      while ((PINC & 0b00001000) == _pinNoTrigger){};
          TCNT1 = 0;
      FLASH_ON;
      DELAY_2US;
      FLASH_OFF;
      for (byte i = 1; i < pulseCount; i++){     
        while(TCNT1 > _t){};
        while (TCNT1 < _t){};
        FLASH_ON;
        DELAY_2US;
        FLASH_OFF;
        _t += delayTime[pulseRate];
      }
    break;
    case 3:
      while ((PINC & 0b00001000) == _pinNoTrigger){};
          TCNT1 = 0;
      FLASH_ON;
      DELAY_4US;
      FLASH_OFF;
      for (byte i = 1; i < pulseCount; i++){      
        while(TCNT1 > _t){};
        while (TCNT1 < _t){};
        FLASH_ON;
        DELAY_4US;
        FLASH_OFF;
        _t += delayTime[pulseRate];
      }
    break;
    default: break;
    }
  }

  else {
    unsigned long _halfMicros = flashDelay;
    _halfMicros *= 16000;
    _halfMicros = _halfMicros >> 16;
    unsigned int _ovf = _halfMicros;
    _halfMicros = flashDelay;
    _halfMicros *= 16000;
    unsigned int _count = _halfMicros;
    Serial.println(_count);
    Serial.println(_ovf);
    while ((PINC & 0b00001000) == _pinNoTrigger){};
    TCNT1 = 0;
    switch (pulseDuration){
    case 0:
    while (_ovf){
      while (TCNT1 < 32000){};
      while (TCNT1 > 30000){};
      _ovf--;
    }
    while (TCNT1 < _count){
      if (TCNT1 >= _ovf) _ovf = TCNT1;
      else _count = 0;
    }
      TCNT1 = 0;
      FLASH_ON;
      DELAY_500_NS;
      FLASH_OFF;
      for (byte i = 1; i < pulseCount; i++){         
        while(TCNT1 > _t){};
        while (TCNT1 < _t){};
        FLASH_ON;
        DELAY_500_NS;
        FLASH_OFF;
        _t += delayTime[pulseRate];
      }
    break;
    case 1:
    while (_ovf){
      while (TCNT1 < 32000){};
      while (TCNT1 > 30000){};
      _ovf--;
    }
    while (TCNT1 < _count){
      if (TCNT1 >= _ovf) _ovf = TCNT1;
      else _count = 0;
    }
          TCNT1 = 0;
      FLASH_ON;
      DELAY_1US;
      FLASH_OFF;
      for (byte i = 1; i < pulseCount; i++){        
        while(TCNT1 > _t){};
        while (TCNT1 < _t){};
        FLASH_ON;
        DELAY_1US;
        FLASH_OFF;
        _t += delayTime[pulseRate];
      }
    break;
    case 2:
    while (_ovf){
      while (TCNT1 < 32000){};
      while (TCNT1 > 30000){};
      _ovf--;
    }
    while (TCNT1 < _count){
      if (TCNT1 >= _ovf) _ovf = TCNT1;
      else _count = 0;
    }
          TCNT1 = 0;
      FLASH_ON;
      DELAY_2US;
      FLASH_OFF;
      for (byte i = 1; i < pulseCount; i++){     
        while(TCNT1 > _t){};
        while (TCNT1 < _t){};
        FLASH_ON;
        DELAY_2US;
        FLASH_OFF;
        _t += delayTime[pulseRate];
      }
    break;
    case 3:
    while (_ovf){
      while (TCNT1 < 32000){};
      while (TCNT1 > 30000){};
      _ovf--;
    }
    while (TCNT1 < _count){
      if (TCNT1 >= _ovf) _ovf = TCNT1;
      else _count = 0;
    }
          TCNT1 = 0;
      FLASH_ON;
      DELAY_4US;
      FLASH_OFF;
      for (byte i = 1; i < pulseCount; i++){      
        while(TCNT1 > _t){};
        while (TCNT1 < _t){};
        FLASH_ON;
        DELAY_4US;
        FLASH_OFF;
        _t += delayTime[pulseRate];
      }
    break;
    default: break;
    }
  }
  FLASH_OFF;
  sei();
  incrementFlashCount();
}
