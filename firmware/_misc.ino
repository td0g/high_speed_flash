void recordPerformance(){
  int e;
  byte x;
  x = (EEPROM.read(0) >> 2) & 1;
  e = 0;
  while (((EEPROM.read(e) >> 2) & 1) == x && (e + 3 < EEPROM_HISTORY_END)){
    e += 3;
  }
  if (e + 3 >= EEPROM_HISTORY_END) {
    e = 0;
    x = 1 - x;
  }
  x = x << 2;
  x |= menuPosition;
  x |= (highVolt >> 2) & 0b11000000;
  x |= (nextVolt >> 4) & 0b00110000;

  EEPROM.write(e, x);
  EEPROM.write(e+1, (highVolt & 255));
  EEPROM.write(e+2, (nextVolt & 255));
}

void printEntry(int i){
  byte x;
  unsigned int _t;
  x = EEPROM.read(i);
  _t = x & 0b11000000;
  _t = _t << 2;
  _t |= EEPROM.read(i+1);
  if (_t != 0){
    switch(x & 0b00000011){
      case 0:
        Serial.print("0.5 us");
      break;
      case 1:
        Serial.print("1.0 us");
      break;
      case 2:
        Serial.print("2.0 us");
      break;
      case 3:
      Serial.print("4.0 us");
      break;
    }
    Serial.println(" Flash");
    Serial.print(F("   Voltage: "));

    float _c;
    _c = _t;
    _t /= 8;
    Serial.print(_t);
    Serial.print(F(" -> "));
    _t = x & 0b00110000;
    _t = _t << 4;
    _t |= EEPROM.read(i+2);
    _c = _c - _t;
    _t /= 8;
    Serial.print(_t);
    Serial.print(F("   Current: "));
    _c /= 8;
    _c *= CAPACITANCE_UF;
    _c *= 2;
    byte d[4] = {1, 2, 4, 8};
    _c /= d[x & 0b00000011];
    Serial.print(_c);
    Serial.print("   EEPROM Location ");
    Serial.println(i);
  }
}

void printHistory(){
  int e;
  byte x;
  e = 0;
  x = (EEPROM.read(0) >> 2) & 1;
  while (((EEPROM.read(e) >> 2) & 1) == x && (e + 3 < EEPROM_HISTORY_END))e += 3;
  if (e + 3 >= EEPROM_HISTORY_END) e = 0;
  
  for (int i = e; i + 3 < EEPROM_HISTORY_END; i += 3) printEntry(i);
  for (int i = 0; i < e; i += 3) printEntry(i);
  Serial.println();
  Serial.print("TOTAL FLASH COUNT ");
  Serial.println(flashCount);
}

void clearHistory(){
  for (int i = 0; i < EEPROM_HISTORY_END; i++) EEPROM.update(i, 0);
}

void runSerial(){
  if (Serial.available()){
    char _c;
    _c = Serial.read();
    if (_c == 'p'){
      printHistory();
    }
    else if (_c == 'e') {
      clearHistory();
      Serial.println(F("History Cleared"));
    }
    else {
      Serial.println(F("Type p to print history"));
      Serial.println(F("Type e to erase history"));
    }
    Serial.println();
  }
}

void updateFlashCount(){
  flashCount++;
  byte x;
  x = flashCount >> 8;
  EEPROM.write(EEPROM_HISTORY_END, flashCount & 255);
  EEPROM.update(EEPROM_HISTORY_END + 1, x);
}
