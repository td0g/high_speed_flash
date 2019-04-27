void runExternal(){
  static unsigned long _lastOn;
  if (digitalRead(TRIGGER_PIN)) {
    unsigned long t;
    t = micros() - _lastOn;
    if (t > 500 && t < 2000){
      chargeFlash();
      runFlash();
      delay(5000);
      REFRESH_DISPLAY;
    }
    _lastOn = micros();
  }
}
