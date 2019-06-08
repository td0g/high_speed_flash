void macro(){
  HIGH_VOLT_ON;
  unsigned int j;
  j = 0;
  while (1){
    while (!Serial.available()){};
    delay(100);
    while (Serial.available()) Serial.read();
    for (unsigned int i = 0; i < 5000; i++){
      float _c;
      float _v;
      digitalWrite(TRIGGER_PIN, 0);
      if (!(j % 50)) pinMode(TRIGGER_PIN, OUTPUT);
      //delay(150);
      _v = analogRead(HIGH_VOLT_OUT_ANALOG);
      //_v += analogRead(HIGH_VOLT_OUT_ANALOG);
      //_v /= 2;
      _c = _v;
      _v /= 8;
      menuPosition = 1;
      forceFlash();
      pinMode(TRIGGER_PIN, INPUT);
      //Serial.print(j);
      //Serial.print("  ");
      //Serial.print(_v);
      //Serial.print(" v --> ");
      _v = nextVolt2;
      _v /= 8;
      //Serial.print(_v);
      
      _c -= _v;
      _c /= 8;
      _c *= 10;
      //Serial.print(" v, current: ");
      //Serial.println(_c);
      j++;
      display.printInt(j);
      //delay(10);
      display.colon(1);
      //delay(750);
      display.colon(0);
      if (_v < 65){
        while (1){};
      }
    }
  }
}
