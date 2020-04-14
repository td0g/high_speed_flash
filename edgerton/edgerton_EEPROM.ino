void recordPerformance(){ //Save flash performance to EEPROM
//Find first record address with differing indexing bit
  byte x = (EEPROM.read(0) >> 2) & 1;
  byte e = 0;
  while (((EEPROM.read(e) >> 2) & 1) == x && (e + 3 < EEPROM_HISTORY_END))e += 3;

//All indexing bits were identical - start at first address
  if (e + 3 >= EEPROM_HISTORY_END) {
    e = 0;
    x = 1 - x;
  }

//Encode performance
  x = x << 2;
  x |= menuPosition;
  x |= (highVolt >> 2) & 0b11000000;
  x |= (nextVolt >> 4) & 0b00110000;

//Save performance to EEPROM
  EEPROM.write(e, x);
  EEPROM.write(e+1, (highVolt & 255));
  EEPROM.write(e+2, (nextVolt & 255));
}

void printEntry(int _index){ //Decode and print performace data @ index to Serial

//get least significant 10 bits
  byte x = EEPROM.read(_index);
  unsigned int _t;
  _t = x & 0b11000000;
  _t = _t << 2;
  _t |= EEPROM.read(_index+1);

//Interpret 10 bits
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
    _t |= EEPROM.read(_index + 2);
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
    Serial.println(_index);
  }
}

void updateFlashCount(){  //Increment total flash count in EEPROM
  flashCount++;
  unsigned int _x;
  _x = flashCount >> 8;
  EEPROM.write(EEPROM_HISTORY_END + 8, flashCount & 255);
  EEPROM.update(EEPROM_HISTORY_END + 9, _x);
}

unsigned int readFlashCount(){  //Read total flash count from EEPROM
  unsigned int _fc = EEPROM.read(EEPROM_HISTORY_END + 8);
  _fc = _fc << 8;
  _fc |= EEPROM.read(EEPROM_HISTORY_END + 9);
  return _fc;
}

void printHistory(){  //Print all recorded performance data and total flash count to Serial

//Find earliest address using indexing bit
  int e = 0;
  byte x;
  x = (EEPROM.read(0) >> 2) & 1;
  while (((EEPROM.read(e) >> 2) & 1) == x && (e + 3 < EEPROM_HISTORY_END))e += 3;
  if (e + 3 >= EEPROM_HISTORY_END) e = 0;

//Print all data records from the earliest address on
  for (int i = e; i + 3 < EEPROM_HISTORY_END; i += 3) printEntry(i);
  
//Print all data records from address 0 to the earliest address
  for (int i = 0; i < e; i += 3) printEntry(i);

//Print total flash count
  Serial.println();
  Serial.print("TOTAL FLASH COUNT ");
  Serial.println(flashCount);
}

void clearHistory(){  //Delete performance data
  for (int i = 0; i < EEPROM_HISTORY_END; i++) EEPROM.update(i, 0);
}

void runSerial(){ //Interacts with serial connection
  if (Serial.available()){
    char _c;
    _c = Serial.read();
    if (_c == 'p') printHistory();
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
