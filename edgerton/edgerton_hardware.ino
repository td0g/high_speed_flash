//Functions to interact with hardware

void flashRoutine(){  //Handles flashing routine (charge, delay, flash, display current, save to EEPROM)

//If battery voltage is low, don't even try flashing
  if (batteryVoltage() < MIN_BATTERY_VOLTAGE * 10)soundError(1);

//Otherwise, attempt to charge flash
  else if (chargeFlash()) {

//If delay is set, flash after delay.  Otherwise, flash immediately on trigger
    if (!flashDelay) runFlash();
    else forceFlash();

//Turn off capacitor charger
    HIGH_VOLT_OFF;

//If button is not pressed, record to EEPROM
    if (BUTTON_NOT_PRESSED) {
      recordPerformance();
      unsigned long _t;
      _t = millis() + 3000;
      while (millis() < _t && BUTTON_NOT_PRESSED){};
      float _c;
      _c = (nextVolt - highVolt) / 8;
      _c *= CAPACITANCE_UF;
      _c *= 2;
      byte x[4] = {1, 2, 4, 8};
      _c /= x[menuPosition];
      _c *= 100;
    
      nextVolt *= 10000;
      nextVolt /= highVolt;

//If button is not pressed, display current output
      display.forceLineState(2, 0, 0b01111111);
      display.forceLineState(3, 0b01110011, 0b00001100);
      display.printInt(nextVolt);
      _t = millis() + 1000;
      while (millis() < _t && BUTTON_NOT_PRESSED){};
      display.setSegments(TXT_nothing);
      display.clearLineState();
      _t = millis() + 250;
      while (millis() < _t && BUTTON_NOT_PRESSED){};
      nextVolt = _c;
      display.forceLineState(2, 0, 0b01111111);
      display.forceLineState(3, 0b01110111, 0b00001000);
      display.printInt(nextVolt);
      _t = millis() + 1000;
      while (millis() < _t && BUTTON_NOT_PRESSED){};
      display.setSegments(TXT_nothing);
      display.clearLineState();
      _t = millis() + 250;
      while (millis() < _t && BUTTON_NOT_PRESSED){};
      REFRESH_DISPLAY;

//Wait for trigger to reset and button to release
        while (!TRIGGER_HIGH || BUTTON_PRESSED){}; 
      }
  }
}

bool chargeFlash(){ //Handles capacitor charging, returns true if charged / false if not charged

//Display voltage
  display.clearLineState();
  display.printInt(analogRead(HIGH_VOLT_OUT_ANALOG) * capacitorVoltCal);
  highVolt = 0;

//Turn on high-voltage converter and begin charging capacitors
  HIGH_VOLT_ON;

//Audio notification
  soundCharging();

//Charge for 500 milliseconds and display capacitor voltage
  unsigned long _t = millis() + 500;
  while (millis() < _t) display.printInt(capacitorVoltage());

//Charge for up to 4 seconds and wait for voltage to normalize, display capacitor voltage
  byte i = 10;
  while (i && analogRead(HIGH_VOLT_OUT_ANALOG) > highVolt + 1){
    readBtn();
    i--;
    highVolt = analogRead(HIGH_VOLT_OUT_ANALOG);
    _t = millis() + 400;
    soundTick();
    while (millis() < _t || BUTTON_PRESSED) {
      display.printInt(capacitorVoltage());
      delay(50);
    }
  }

//Measure capacitor voltage
  highVolt = 0;
  for (byte i = 0; i < 8; i++) highVolt += analogRead(HIGH_VOLT_OUT_ANALOG);
  highVolt = highVolt >> 3;

//If capacitor voltage exceeds safety limit, stop charging and return false signal
  if (capacitorVoltage() > MAX_CAPACITOR_VOLTAGE){
    soundError();
    HIGH_VOLT_OFF;
    return false;
  }

//Audio notification capacitors are charged
  soundDone();

//Return charged signal
  return true;
}
