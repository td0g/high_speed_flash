
  #define EEPROM_COUNT_LOCATION 512
  #define EEPROM_COUNT_BLOCK_SIZE 64
  #define EEPROM_SYSTEM_INFO_LOCATION 256
  #define EEPROM_CALIBRATION_LOCATION 0

 

void importEEPROM(){
  if (EEPROMReadint(EEPROM_SYSTEM_INFO_LOCATION) != SERIAL_NUMBER || EEPROMReadint(EEPROM_SYSTEM_INFO_LOCATION + 2) != (65535 - SERIAL_NUMBER)){
    for (int i = 0; i < EEPROM.length(); i++)EEPROM.update(i, 0);
    EEPROMWriteint(EEPROM_SYSTEM_INFO_LOCATION, SERIAL_NUMBER);
    EEPROMWriteint(EEPROM_SYSTEM_INFO_LOCATION + 2, (65535 - SERIAL_NUMBER));
    EEPROMWriteint(EEPROM_CALIBRATION_LOCATION, EEPROM_BATTERY_CAL_2V);
    EEPROMWriteint(EEPROM_CALIBRATION_LOCATION+2, EEPROM_BATTERY_CAL_6V);
    EEPROMWriteint(EEPROM_CALIBRATION_LOCATION+4, EEPROM_ANODE_CAL);
  }
  byte _settings = EEPROM.read(EEPROM_CALIBRATION_LOCATION+6);
  reverseEncoder = _settings & 1;
  flipDisplay = (_settings >> 1) & 1;
  soundOn = (_settings >> 2) & 1;
  calBattery2V = EEPROMReadint(EEPROM_CALIBRATION_LOCATION);
  calBattery6V = EEPROMReadint(EEPROM_CALIBRATION_LOCATION+2);
  calAnode = EEPROMReadint(EEPROM_CALIBRATION_LOCATION+4);
}

 

void updateSettingsEEPROM(){
  byte _settings = reverseEncoder | (flipDisplay << 1) | (soundOn << 2);
  EEPROM.update(EEPROM_CALIBRATION_LOCATION+6, _settings);
  EEPROMWriteint(EEPROM_CALIBRATION_LOCATION, calBattery2V);
  EEPROMWriteint(EEPROM_CALIBRATION_LOCATION+2, calBattery6V);
  EEPROMWriteint(EEPROM_CALIBRATION_LOCATION+4, calAnode);
}

 

//Maximum EEPROM write-cycles: 65535
//Maximum flash count: ~2,000,000
void incrementFlashCount(){
  //Update count @ this duration
  unsigned int _index = EEPROM_COUNT_LOCATION + pulseDuration * EEPROM_COUNT_BLOCK_SIZE;
  unsigned long _thisf = EEPROMReadint(_index);
  while (_thisf > 65535 - pulseCount){
    _index += 2;
    _thisf = EEPROMReadint(_index);
  }
  _thisf += pulseCount;
  EEPROMWriteint(_index, _thisf);
  
  //Update total flash executions (multi-strobes will be considered a single execution)
  _index = EEPROM_COUNT_LOCATION + 4 * EEPROM_COUNT_BLOCK_SIZE;
  _thisf = EEPROMReadint(_index);
  while (_thisf == 65535){
    _index += 2;
    _thisf = EEPROMReadint(_index);
  }
  _thisf++;
  EEPROMWriteint(_index, _thisf);
}

 

unsigned long readFlashCount(byte _duration){
  //use 4 for total flash executions (multi-strobes will be considered a single execution)
  unsigned long _f = 0;
  for (int _index = EEPROM_COUNT_LOCATION + _duration * EEPROM_COUNT_BLOCK_SIZE; _index < EEPROM_COUNT_LOCATION + _duration * EEPROM_COUNT_BLOCK_SIZE + EEPROM_COUNT_BLOCK_SIZE; _index += 2){
    _f += EEPROMReadint(_index);
  }
  return _f;
}

 

unsigned int EEPROMReadint(unsigned int _i){
  unsigned int _t;
  _t = EEPROM.read(_i);
  _t *= 256;
  _t += EEPROM.read(_i + 1);
  return (_t);
}

 

void EEPROMWriteint(unsigned int _i, unsigned int _d){
  byte t;
  t = _d;
  EEPROM.update(_i+1, t);
  t = _d >> 8;
  EEPROM.update(_i, t);
}
