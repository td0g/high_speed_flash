void runUI(){
  
  byte _b = readBtn();
  int8_t _e = readEncoder();
  if (_b){
    Serial.println(menuPosition);
    soundTick();
    switch (menuPosition){
      case 0:
        if (_b == 1) menuPosition = 1;
        #if (SUBMODEL == E2A)
          else if (batteryVoltage() > MIN_BATTERY_VOLTAGE) {
            menuPosition = 4;
            HIGH_VOLT_ON;
          }
        #else
          else menuPosition = 8;
        #endif
      break;
      case 1:
        if (_b == 1) {
          if (pulseCount == 1) menuPosition = 3;
          else menuPosition = 2;
        }
        #if (SUBMODEL == E2A)
          else if (batteryVoltage() > MIN_BATTERY_VOLTAGE) {
            menuPosition = 4;
            HIGH_VOLT_ON;
          }
        #else
          else menuPosition = 8;
        #endif
      break;
      case 2:
        if (_b == 1) menuPosition = 3;
        #if (SUBMODEL == E2A)
          else if (batteryVoltage() > MIN_BATTERY_VOLTAGE) {
          menuPosition = 4;
          HIGH_VOLT_ON;
        }
        #else
          else menuPosition = 8;
        #endif
      break;
      case 3:
        #if (SUBMODEL == E2A)
          if (_b == 1) {
            menuPosition = 0;
          }
          else if (batteryVoltage() > MIN_BATTERY_VOLTAGE) {
            menuPosition = 4;
            HIGH_VOLT_ON;
          }
        #else
          if (_b == 1) {
            menuPosition = 4;
            HIGH_VOLT_ON;
          }
          else menuPosition = 8;
        #endif
      break;
      case 4:
        if (_b == 1) {
          menuPosition = 0;
          HIGH_VOLT_OFF;
        }
        else if (anodeVoltage() > MIN_ANODE_VOLTAGE) {
          enterFullyArmedState();
        }
      break;
      case 8:
        if (_b == 1) {
          if (modellingLamp) modellingLamp = 0;
          else modellingLamp = modellingLampLast;
          menuPosition |= 0b10000000;
        }
        else {
          modellingLamp = 0;
          modellingLampLast = 1;
          menuPosition = 16;
        }
      break;
      case 16: //Duty Cycle
        if (_b == 1) menuPosition = 17;
        else {
          modellingLamp = 0;
          menuPosition = 0;
        }
      break;
      case 17: //Strobe Rate
        if (_b == 1) menuPosition = 16;
        else {
          modellingLamp = 0;
          menuPosition = 0;
        }
      break;
    }
  }
  if (_e != 0){
    soundTick();
    switch (menuPosition){
      case 0:
        if (pulseDuration > 0 || _e == 1){
          if (pulseDuration < 3 || _e == -1){
            pulseDuration += _e;
            pulseDuration = min(pulseDuration, maxPulseDuration());
            menuPosition |= 0b10000000;
          }
        }
      break;
      case 1:
        if (pulseCount > 1 || _e == 1){
          if (pulseCount < MAX_PULSES || _e == -1){
            if (pulseCount == 1){
              pulseRate = max(pulseRate, maxPulseRate());
            }
            pulseCount += _e;
            pulseCount = min(pulseCount, maxPulseCount());
            menuPosition |= 0b10000000;
          }
        }
      break;
      case 2:
        if (pulseRate > 0 || _e == 1){
          if (pulseRate < MAX_PULSE_RATE || _e == -1){
            pulseRate += _e;
            pulseRate = max(pulseRate, maxPulseRate());
            menuPosition |= 0b10000000;
          }
        }
      break;
      case 3:
        if (_e == 1 && flashDelay < 9999){
          if (flashDelay == 9000) flashDelay = 9999;
          else if (flashDelay >= 1000) flashDelay += 500;
          else if (flashDelay >= 100) flashDelay += 50;
          else if (flashDelay >= 20) flashDelay += 5;
          else flashDelay += 1;
          menuPosition |= 0b10000000;
        }
        else if (_e == -1 && flashDelay) {
          if (flashDelay == 9999) flashDelay = 9000;
          else if (flashDelay > 1000) flashDelay -= 500;
          else if (flashDelay > 100) flashDelay -= 50;
          else if (flashDelay > 20) flashDelay -= 5;
          else flashDelay--;
          menuPosition |= 0b10000000;
        }
      break;
      case 8:
        if (!modellingLamp){
          if (pulseRate > 1 || _e == 1){
            if (pulseRate < 100 || _e == -1){
              pulseRate += _e;
              menuPosition |= 0b10000000;
            }
          }
        }
        else if (modellingLamp > 1 || _e == 1){
          if (modellingLamp < 100 || _e == -1){
            modellingLamp += _e;
            modellingLampLast = max(1, modellingLamp);
            menuPosition |= 0b10000000;
          }
        }
      break;
      case 16:
        if (modellingLamp > 0 || _e == 1){
          if (modellingLamp < 100 || _e == -1){
            modellingLamp += _e;
            modellingLampLast = modellingLamp;
            menuPosition |= 0b10000000;
          }
        }
      break;
      case 17:
        if (pulseRate > 1 || _e == 1){
          if (pulseRate < 100 || _e == -1){
            pulseRate += _e;
            menuPosition |= 0b10000000;
          }
        }
      break;
    }
  }
}




 

/*Limitations:

PulseRate (MAX):

    0   1   2   3 pulseDuration  

2   0   0   0   3

3   0   0   3   3

4   0   0   3  

5   0   3  

6   0   3  

pulseCount

*/

 

byte maxPulseRate(){
  switch (pulseDuration){
    case 0: return 0; break;
    case 1:
      if (pulseCount < 5) return 0;
      return 3;
    break;
    case 2:
      if (pulseCount < 3) return 0;
      return 3;
    break;
    default: return 3; break;
  }
}

 

byte maxPulseCount(){
  switch (pulseDuration){
    case 0: return 6; break;
    case 1:
      if (pulseRate < 3) return 4;
      return 6;
    break;
    case 2:
      if (pulseRate < 3) return 2;
      return 4;
    break;
    case 3:
      if (pulseRate < 3) return 1;
      return 2;
    break;
  }
}

 

byte maxPulseDuration(){
  switch (pulseRate){
    case 0: case 1: case 2:
      if (pulseCount == 1) return 3;
      if (pulseCount < 3) return 2;
      if (pulseCount < 5) return 1;
      return 0;
    break;
    default:
      if (pulseCount < 4) return 3;
      if (pulseCount < 5) return 2;
      return 1;
    break;
  }
}
