void runAudio(){
  static unsigned long _timer;
  static byte _triggerState = TRIGGER_HIGH;
  if (TRIGGER_HIGH != _triggerState){
    _timer = millis() + 100;
    _triggerState = TRIGGER_HIGH;
  }
  if (_timer && millis() > _timer){
    if (_triggerState) soundUp();
    else soundDown();
    _timer = 0;
  }
}

void soundStartup(){
  #ifdef SOUND
    tone(SOUND_PIN, TONE_A, 20);
    delay(30);
    tone(SOUND_PIN, TONE_C_S, 20);
    delay(30);
    tone(SOUND_PIN, TONE_E, 20);
    delay(30);
    tone(SOUND_PIN, TONE_C_S, 20);
  #endif
}

void soundUp(){
  #ifdef SOUND
    tone(SOUND_PIN, TONE_G, 100);
  #endif
}

void soundDown(){
  #ifdef SOUND
    tone(SOUND_PIN, TONE_A, 100);
  #endif
}

void soundTick(){
  #ifdef SOUND
    tone(SOUND_PIN, 5000, 5);
  #endif
}

void soundCharging(){
  #ifdef SOUND
    tone(SOUND_PIN, TONE_A, 40);
    delay(50);
    tone(SOUND_PIN, TONE_C, 40);
    delay(50);
    tone(SOUND_PIN, TONE_E, 40);
    delay(50);
    tone(SOUND_PIN, TONE_C, 40);
  #endif
}

void soundDone(){
  #ifdef SOUND
    tone(SOUND_PIN, TONE_B, 40);
    delay(60);
    tone(SOUND_PIN, TONE_B, 40);
    delay(60);
    tone(SOUND_PIN, TONE_B, 40);
    delay(60);
    tone(SOUND_PIN, TONE_B, 40);
  #endif
}

void soundError(byte _e){
  #ifdef SOUND
    tone(SOUND_PIN, TONE_D, 500);
    delay(600);
    while (_e){
      tone(SOUND_PIN, TONE_A, 60);
      delay(100);
      _e--;
    }
  #endif
}
