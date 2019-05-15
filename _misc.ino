void recordPerformance(){
 int e;
  e = 0;
  while (e != -1 && EEPROM.read(e) && EEPROM.read(e) != 255){
    if (e + 6 > EEPROM.length()) e = -1;
    else e += 5;
  }

  if (e != -1) {
    EEPROM.write(e, menuPosition + 1);
    EEPROM.write(e+1, (highVolt & 255));
    EEPROM.write(e+2, ((highVolt >> 8) & 255));
    EEPROM.write(e+3, (nextVolt & 255));
    EEPROM.write(e+4, ((nextVolt >> 8) & 255));
  }
}

void printHistory(){
  int e;
  e = 0;
  while (EEPROM.read(e) && EEPROM.read(e) != 255){
    switch(EEPROM.read(e)){
      case 1:
        Serial.print("0.5 us");
      break;
      case 2:
        Serial.print("1.0 us");
      break;
      case 3:
        Serial.print("2.0 us");
      break;
      case 4:
      Serial.print("4.0 us");
      break;
    }
    Serial.println(" Flash");
    Serial.print(F("   Voltage: "));
    unsigned int _t = EEPROM.read(e+2);
    _t = _t << 8;
    _t |= EEPROM.read(e+1);
    _t *= 4;
    _t /= 9;
    float _c;
    _c = _t;
    Serial.print(_t);
    Serial.print(F(" -> "));
    _t = EEPROM.read(e+4);
    _t = _t << 8;
    _t |= EEPROM.read(e+3);
    _t /= 9;
    _c -= _t;
    Serial.print(_t);
    Serial.print(F("   Current: "));
    _c *= CAPACITANCE_UF;
    _c *= 2;
    byte x[4] = {1, 2, 4, 8};
    _c /= x[EEPROM.read(e)-1];
    Serial.println(_c);
    e += 5;
  }
}

void clearHistory(){
  for (int i = 0; i < EEPROM.length(); i++){
    EEPROM.update(i, 0);
  }
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
