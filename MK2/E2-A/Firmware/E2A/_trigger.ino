// Handling trigger input while unit is not fully armed

void runTrigger(){
  static byte lastTrigger = 8;
  static unsigned long debounceTimer;
  static byte triggerCount = 0;
  if (millis() > debounceTimer){
    debounceTimer += 10;
    if (menuPosition == 4){
      if (anodeVoltage() >= MIN_ANODE_VOLTAGE){
        if (activeTrigger && TRIGGER_HIGH){
          triggerCount++;
          if (triggerCount == BUTTON_LONGHOLD/10) {
            triggerCount = 0;
            enterFullyArmedState();
          }
        }
        else if (!activeTrigger && TRIGGER_LOW){
          triggerCount++;
          if (triggerCount == BUTTON_LONGHOLD/10) {
            triggerCount = 0;
            enterFullyArmedState();
          }
        }
        else triggerCount = 0;
      }
    }
    else if (TRIGGER_HIGH){
      activeTrigger = 0;
      if (lastTrigger != TRIGGER_HIGH) soundUp();
    }
    else {
      activeTrigger = 1;
      if (lastTrigger != TRIGGER_HIGH) soundDown();
    }
    lastTrigger = TRIGGER_HIGH;
  }
}
