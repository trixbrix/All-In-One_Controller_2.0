#include "Trixbrix/TimersAndInterruptsHandle.h"

//////////////////////////////////////// TIMER 0 COMPARE MATCH INTERRUPT ////////////////////////////////////////

ISR(TIMER0_COMPA_vect) {

  iteratorTimer0Loop++;

  if (distanceSensorMode == boom_barrier_mode && iteratorTimer0Loop > 120) {

    if (iteratorTimer0Loop < 190) {
          digitalWrite(LED_SIGNAL1, HIGH);
          digitalWrite(LED_SIGNAL2, HIGH);
    }

    if (iteratorTimer0Loop == 190) {
          digitalWrite(LED_SIGNAL1, switchLedState);
          digitalWrite(LED_SIGNAL2, !switchLedState);
    }
  }

  if (iteratorTimer0Loop == 190) {
        iteratorTimer0Loop = 0;
        if (buttonPressedOrDistSensWorkedInSwitchMode ||
            distSensWorkedInSwitchModeInQueue ||
            distSensWorkedInBoomBarrierMode_StateToSwitchEnd)
            digitalWrite(activePcbLed, switchLedState);

        switchLedState = !switchLedState;
  }

  if(actualBoomBarrierPosition == closed && !buttonPressedOrDistSensWorkedInSwitchMode &&
    !distSensWorkedInBoomBarrierMode_StateToSwitchEnd)
            timerLedBlinkDeinit(); // to stop signal led blinking after being activated by position 2
}

//////////////////////////////////////// TIMER 2 COMPARE MATCH INTERRUPT ////////////////////////////////////////

ISR(TIMER2_COMPA_vect) {
  
  #ifdef DEBUG_MODE_COOLDOWN_MEASURE
  if(iteratorTimer2Loop == 0)
    currTime = millis();
  #endif

  iteratorTimer2Loop++;

  if(iteratorTimer2Loop == COOLDOWN_MS / 27) { // divider was choosen experimentally
      iteratorTimer2Loop = 0;

      #ifdef DEBUG_MODE_COOLDOWN_MEASURE
      Serial.print("Cooldown finished after ");
      Serial.println(millis() - currTime);
      #endif

      if(distanceSensorMode == switch_mode) {

            if(distSens1_SwitchModeCooldownLock)
                distSens1_SwitchModeCooldownLock = 0;

            if(distSens2_SwitchModeCooldownLock)
                distSens2_SwitchModeCooldownLock = 0;

      } else if(distanceSensorMode == boom_barrier_mode) {

          if((whichDistSensWorkedPrev == 0) ||
            ((whichDistSensWorkedPrev == 1) && (digitalRead(DIST_SENS2) == 1)) ||
            ((whichDistSensWorkedPrev == 2) && (digitalRead(DIST_SENS1) == 1)) )
                  resetStateAfterBoomBarrierSeq();

      }

      timerCooldownDeinit();
  }
}

//////////////////////////////////////// RESET STATE AFTER BOOM BARRIER SEQUENCE ////////////////////////////////////////

void resetStateAfterBoomBarrierSeq(void) {

    slopeDetDistSens1Init(falling);
    slopeDetDistSens2Init(falling);
    distSensWorkedInBoomBarrierMode = distSensWorkedInBoomBarrierMode_StateToSwitchEnd = 1;
    whichDistSensWorkedPrev = 0;
    newTrainDetectedInBoomBarrierMode = not_detected;
}

//////////////////////////////////////// PCINT1 INTERRUPT ////////////////////////////////////////

ISR(PCINT1_vect) {

  volatile bool distSens1_Rising = 0;
  volatile bool distSens1_Falling = 0;
  volatile bool distSens2_Rising = 0;
  volatile bool distSens2_Falling = 0;
  
  if(distSens1_PrevState == 0 && digitalRead(DIST_SENS1) == 1) {
    distSens1_Rising = 1; distSens1_Falling = 0; distSens1_PrevState = 1;
    #ifdef DEBUG_MODE_PCINT
    Serial.println("PCINT DIST_SENS1 RISING");
    #endif
  }
  else if(distSens1_PrevState == 1 && digitalRead(DIST_SENS1) == 0) {
    distSens1_Rising = 0; distSens1_Falling = 1; distSens1_PrevState = 0;
    #ifdef DEBUG_MODE_PCINT
    Serial.println("PCINT DIST_SENS1 FALLING");
    #endif
  }
  else {
    distSens1_Rising = 0; distSens1_Falling = 0;
  }

  if(distSens2_PrevState == 0 && digitalRead(DIST_SENS2) == 1) {
    distSens2_Rising = 1; distSens2_Falling = 0; distSens2_PrevState = 1;
    #ifdef DEBUG_MODE_PCINT
    Serial.println("PCINT DIST_SENS2 RISING");
    #endif
  }
  else if(distSens2_PrevState == 1 && digitalRead(DIST_SENS2) == 0) {
    distSens2_Rising = 0; distSens2_Falling = 1; distSens2_PrevState = 0;
    #ifdef DEBUG_MODE_PCINT
    Serial.println("PCINT DIST_SENS2 FALLING");
    #endif
  }
  else {
    distSens2_Rising = 0; distSens2_Falling = 0;
  }

//////////////////////////////////////// SWITCH MODE ////////////////////////////////////////

    if (distanceSensorMode == switch_mode){
      
        if(distSens1_Falling && !distSens1_SwitchModeCooldownLock && 
          !buttonPressedOrDistSensWorkedInSwitchMode && !distSensWorkedInSwitchModeInQueue) {
      
                  buttonPressedOrDistSensWorkedInSwitchMode = 1;
                  
                  #ifdef DEBUG_MODE_BASIC
                  Serial.println("DIST_SENS1 in switch mode worked");
                  #endif
          
        } else if(distSens2_Falling && !distSens2_SwitchModeCooldownLock &&
                  !buttonPressedOrDistSensWorkedInSwitchMode && !distSensWorkedInSwitchModeInQueue) {
      
                  buttonPressedOrDistSensWorkedInSwitchMode = 1;
                  
                  #ifdef DEBUG_MODE_BASIC
                  Serial.println("DIST_SENS2 in switch mode worked");
                  #endif
        
        } else if(distSens1_Falling && !distSens1_SwitchModeCooldownLock &&
                  buttonPressedOrDistSensWorkedInSwitchMode && !distSensWorkedInSwitchModeInQueue) {

                  distSensWorkedInSwitchModeInQueue = 1;

                  #ifdef DEBUG_MODE_BASIC
                  Serial.println("DIST_SENS1 worked in switch mode, occurence add to queue");
                  #endif
        
        } else if(distSens2_Falling && !distSens2_SwitchModeCooldownLock &&
                  buttonPressedOrDistSensWorkedInSwitchMode && !distSensWorkedInSwitchModeInQueue) {

                  distSensWorkedInSwitchModeInQueue = 1;

                  #ifdef DEBUG_MODE_BASIC
                  Serial.println("DIST_SENS2 worked in switch mode, occurence add to queue");
                  #endif

        } else if(distSens1_Rising && !distSens1_SwitchModeCooldownLock) {
                   
                  distSens1_SwitchModeCooldownLock = 1;
                  timerCooldownInit(COOLDOWN_MS);
        
        } else if(distSens2_Rising && !distSens2_SwitchModeCooldownLock) {

                  distSens2_SwitchModeCooldownLock = 1;
                  timerCooldownInit(COOLDOWN_MS);
        }
    }

//////////////////////////////////////// BOOM BARRIER MODE ////////////////////////////////////////

    if(distanceSensorMode == boom_barrier_mode) {

        if((risingOrFallingDistSens1 == 0 && distSens1_Rising == 1 && digitalRead(DIST_SENS2)) ||
          (risingOrFallingDistSens1 == 1 && distSens1_Falling == 1 && digitalRead(DIST_SENS2))) {


                  if(newTrainDetectedInBoomBarrierMode == not_detected) {
                      cli();
                      slopeDetDistSens2Init(rising);
                      sei();
                      distSensWorkedInBoomBarrierMode = distSensWorkedInBoomBarrierMode_StateToSwitchEnd = 1;
                      whichDistSensWorkedPrev = 1;
                      newTrainDetectedInBoomBarrierMode = detected;  
                      #ifdef DEBUG_MODE_BASIC
                      Serial.println("DIST_SENS1 in boom barrier mode worked");
                      #endif
                  
                  } else if(whichDistSensWorkedPrev != 1) {
                      timerCooldownInit(COOLDOWN_MS);
                  }

        } else if((risingOrFallingDistSens2 == 0 && distSens2_Rising == 1 && digitalRead(DIST_SENS1)) ||
                  (risingOrFallingDistSens2 == 1 && distSens2_Falling == 1 && digitalRead(DIST_SENS1))) {

                  if(newTrainDetectedInBoomBarrierMode == not_detected) {
                      cli();
                      slopeDetDistSens1Init(rising);
                      sei();
                      distSensWorkedInBoomBarrierMode = distSensWorkedInBoomBarrierMode_StateToSwitchEnd = 1;
                      whichDistSensWorkedPrev = 2;
                      newTrainDetectedInBoomBarrierMode = detected;
                      #ifdef DEBUG_MODE_BASIC
                      Serial.println("DIST_SENS2 in boom barrier mode worked");
                      #endif
                  
                  } else if(whichDistSensWorkedPrev != 2) {
                      timerCooldownInit(COOLDOWN_MS);
                  }

          }
    }
}

//////////////////////////////////////// TIMER 0 INIT ////////////////////////////////////////

void timerLedBlinkInit(void) {
  
  OCR0A = 100;
  TCCR0A |= (1 << WGM01); // CTC mode
  TCCR0B |= ((1 << CS00) | (1 << CS01)); // 64 prescaler
  TIMSK0 |= (1 << OCIE0A); // interrupt from overflow of timer1 is enabled
  #ifdef DEBUG_MODE_TIMERS
  Serial.println("Led blink timer init");
  #endif
}

//////////////////////////////////////// TIMER 0 DEINIT ////////////////////////////////////////

void timerLedBlinkDeinit(void) {
  
  TIMSK0 &= ~(1 << OCIE0A); // interrupt from compare match of timer1 is enabled
  iteratorTimer0Loop = 0;

  #ifdef DEBUG_MODE_TIMERS
  Serial.println("Led blink timer deinit");
  #endif
}

//////////////////////////////////////// TIMER 2 INIT ////////////////////////////////////////

void timerCooldownInit(int timerInput) {
  
  if(!(TIMSK2 & OCIE2A)) {
      OCR2A = 100;
      TCCR2A |= (1 << WGM21); // CTC mode
      TCCR2B |= ((1 << CS20) | (1 << CS21)); // 64 prescaler
      TIMSK2 |= (1 << OCIE2A); // interrupt from overflow of timer1 is enabled
      
      #ifdef DEBUG_MODE_TIMERS
      Serial.println("Cooldown timer init");
      #endif  
  }
}

//////////////////////////////////////// TIMER 2 DEINIT ////////////////////////////////////////

void timerCooldownDeinit(void) {

  TIMSK2 &= ~(1 << OCIE2A); // interrupt from compare match of timer1 is enabled
  iteratorTimer2Loop = 0;
  
  #ifdef DEBUG_MODE_TIMERS
  Serial.println("Cooldown timer deinit");
  #endif 
}

//////////////////////////////////////// PCINT11 INTERRUPT INIT ////////////////////////////////////////

void slopeDetDistSens1Init(enum edge_detection risingOrFalling) {

  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT11);
  
  if(risingOrFalling == falling) { //1 -> reacts on a falling edge, ISC01 = 1 and ISC00 = 0
    risingOrFallingDistSens1 = 1;
    #ifdef DEBUG_MODE_INTERRUPTS
    Serial.println("DIST_SENS1 Falling init");
    #endif
  }
  else {                           //0 -> reacts on a rising edge, ISC01 = 1 and ISC00 = 1
    risingOrFallingDistSens1 = 0;
    #ifdef DEBUG_MODE_INTERRUPTS
    Serial.println("DIST_SENS1 Rising init");
    #endif
  }
}

//////////////////////////////////////// PCINT11 INTERRUPT DEINIT ////////////////////////////////////////

void slopeDetDistSens1Deinit(void) {

  PCICR &= ~(1 << PCIE1);
  PCMSK1 &= ~(1 << PCINT11);
  #ifdef DEBUG_MODE_INTERRUPTS
  Serial.println("DIST_SENS1 deinit");
  #endif
}

//////////////////////////////////////// PCINT10 INTERRUPT INIT ////////////////////////////////////////

void slopeDetDistSens2Init(enum edge_detection risingOrFalling) {

  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT10);
  
  if(risingOrFalling == falling) { //1 -> reacts on a falling edge, ISC11 = 1 and ISC10 = 0
    risingOrFallingDistSens2 = 1;
    #ifdef DEBUG_MODE_INTERRUPTS
    Serial.println("DIST_SENS2 Falling init");
    #endif
  }
  else {                           //0 -> reacts on a rising edge, ISC11 = 1 and ISC10 = 1
    risingOrFallingDistSens2 = 0;
    #ifdef DEBUG_MODE_INTERRUPTS
    Serial.println("DIST_SENS2 Rising init");
    #endif
  }
}

//////////////////////////////////////// PCINT10 INTERRUPT DEINIT ////////////////////////////////////////

void slopeDetDistSens2Deinit(void) {

  PCICR &= ~(1 << PCIE1);
  PCMSK1 &= ~(1 << PCINT10);
  #ifdef DEBUG_MODE_INTERRUPTS
  Serial.println("DIST_SENS2 deinit");
  #endif
}
