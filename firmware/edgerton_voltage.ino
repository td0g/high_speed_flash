unsigned int batteryVoltage(){
  static unsigned int _a = analogRead(INPUT_VOLT_ANALOG);
  _a += analogRead(INPUT_VOLT_ANALOG);
  _a = _a >> 1;
  return _a * batteryVoltCal;
}

byte capacitorVoltage(){
  unsigned long _v;
  _v = analogRead(HIGH_VOLT_OUT_ANALOG);
  _v *= highVoltCal;
  return _v;
}
