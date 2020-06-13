/*

void testUnit(){
  while (TRIGGER_HIGH){
    soundDone();
    delay(250);
  }
  for (int i = 0; i < 1000; i++){
    pulseDuration = 0;
    pulseCount = 1;
    forceFlash();
    delay(125);
  }
  while (TRIGGER_HIGH){
    soundDone();
    delay(250);
  }
  for (int i = 0; i < 1000; i++){
    pulseDuration = 1;
    pulseCount = 1;
    forceFlash();
    delay(125);
  }
  while (TRIGGER_HIGH){
    soundDone();
    delay(250);
  }
  for (int i = 0; i < 1000; i++){
    pulseDuration = 2;
    pulseCount = 1;
    forceFlash();
    delay(125);
  }
  while (TRIGGER_HIGH){
    soundDone();
    delay(250);
  }
  for (int i = 0; i < 1000; i++){
    pulseDuration = 3;
    pulseCount = 1;
    forceFlash();
    delay(125);
  }

  
  while (TRIGGER_HIGH){
    soundDone();
    delay(250);
  }
  for (int i = 0; i < 250; i++){
    pulseDuration = 0;
    pulseCount = 6;
    pulseRate = 0;
    forceFlash();
    delay(500);
  }
  
  while (TRIGGER_HIGH){
    soundDone();
    delay(250);
  }
  for (int i = 0; i < 250; i++){
    pulseDuration = 1;
    pulseCount = 4;
    pulseRate = 0;
    forceFlash();
    delay(500);
  }
  
  while (TRIGGER_HIGH){
    soundDone();
    delay(250);
  }
  for (int i = 0; i < 250; i++){
    pulseDuration = 2;
    pulseCount = 4;
    pulseRate = 3;
    forceFlash();
    delay(500);
  }
  
  while (TRIGGER_HIGH){
    soundDone();
    delay(250);
  }
  for (int i = 0; i < 250; i++){
    pulseDuration = 3;
    pulseCount = 3;
    pulseRate = 3;
    forceFlash();
    delay(500);
  }
  
  for (byte i = 0; i < 16; i++){
    soundDone();
    delay(250);
  }
}

void forceFlash(){
  unsigned long _trigTime;
  _trigTime = micros();
    unsigned int _t = delayTime[pulseRate] - 10;
    switch (pulseDuration){
    case 0:
        while (micros() < _trigTime){};
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
        while (micros() < _trigTime){};
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
        while (micros() < _trigTime){};
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
        while (micros() < _trigTime){};
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
      default:
      break;
    }
  FLASH_OFF;
}
*/
