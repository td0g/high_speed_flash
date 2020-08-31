//Functions to read battery and capacitor voltages

unsigned int batteryVoltage(){
//Takes an average of two analog readings for battery voltage
  static unsigned int _a = analogRead(INPUT_VOLT_ANALOG);
  _a += analogRead(INPUT_VOLT_ANALOG);
  _a = _a >> 1;
  return _a * batteryVoltCal;
}

byte capacitorVoltage(){
//Takes four capacitor voltage readings and returns actual integer voltage
  float _v;
  _v = analogRead(HIGH_VOLT_OUT_ANALOG);
  _v += analogRead(HIGH_VOLT_OUT_ANALOG);
  _v += analogRead(HIGH_VOLT_OUT_ANALOG);
  _v += analogRead(HIGH_VOLT_OUT_ANALOG);
  _v /= 4;
  if (_v > 1020) _v = 9999;
  else _v *= capacitorVoltCal;
  return _v;
}
