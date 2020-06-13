void analogReadAll(){
  static byte adcNumber;
  static const byte adcPins[2] = {INPUT_VOLTAGE_PIN - 14, CAPACITOR_VOLTAGE_PIN - 14};
  if (bit_is_clear(ADCSRA, ADSC)){
    switch (adcNumber){
      case 0:
        lowVoltAdc += ADC;
        lowVoltAdc = lowVoltAdc >> 1;
        break;
      case 1:
        highVoltAdc += ADC;
        highVoltAdc = highVoltAdc >> 1;
        break;
    }
    adcNumber = 1 - adcNumber;
    byte adcPin;
    adcPin = adcPins[adcNumber];
    ADMUX =   bit (REFS0) | (adcPin & 31);  // AVcc  
    ADCSRB = ((adcPin & 0b00100000) >> 2) + 1;
    bitSet(ADCSRA, ADSC);
  }
}

unsigned int anodeVoltage(){
  return (max(10, highVoltAdc) - 10)*20 / calAnode;  //250 = 5.1V, 1,050 = 31V
}

float batteryVoltage(){
  //2V = 82, 3V = 103, 4V = 138, 5V = 173, 6V = 209
  byte voltageValues[5] = {82, 103, 138, 173, 209};
  if (lowVoltAdc <= calBattery2V) return (float) 2*lowVoltAdc/calBattery2V;
  if (lowVoltAdc >= calBattery6V) return (float) 6*lowVoltAdc/calBattery6V;
  float _t = lowVoltAdc - calBattery2V;
  _t = _t / (calBattery6V - calBattery2V);
  _t = _t * 4 + 2;
  return _t;
}
