#include "Trixbrix/ServoLedControl.h"


//////////////////////////////////////// BOTH LED BLINK ////////////////////////////////////////

void bothLedBlink(char blinks) {
  char blink;
            for(blink = 0; blink < blinks; blink++) {
              
                digitalWrite(LED_PCB1, LOW);
                digitalWrite(LED_PCB2, LOW);
                delay(200);
                digitalWrite(LED_PCB1, HIGH);
                digitalWrite(LED_PCB2, HIGH);
                delay(200);
            }
  delay(500);
}

//////////////////////////////////////// SIGNAL LED CHANGE ////////////////////////////////////////

void signalLedChange(int changeMode) {

  digitalWrite(LED_SIGNAL1, HIGH);
  digitalWrite(LED_SIGNAL2, HIGH);

  ledChangeMode = changeMode;
}

//////////////////////////////////////// MOVE SERVO ////////////////////////////////////////

void moveServo(enum servos servo, unsigned int angle, unsigned int timesOfRepeat) {

  unsigned int pin;
  unsigned int i;
  unsigned int microSeconds = map(angle, 0, 180, 500, 2400); // max 2400 (180 degree), min 500 (0 degree)

  switch(servo) {
    case servo1: //turns servo 1
        pin = SERVO1;
        digitalWrite(SERVO1_POW, HIGH);
        break;
    
    case servo1_neg: //turns servo 1 (negated)
        pin = SERVO1_NEG;
        digitalWrite(SERVO1_NEG_POW, HIGH);
        break;

    case servo2: //turns servo no 2
        pin = SERVO2;
        break;

    default:
        break;
  }
          if(servo != servo2)  
            delay(SWITCH_ON_SERVO_DELAY_MS);

          for(i = 0; i < timesOfRepeat; i++) {
              digitalWrite(pin, HIGH);
              delayMicroseconds(microSeconds);
              digitalWrite(pin, LOW);
              delayMicroseconds(MOVE_SERVO_PERIOD_US - microSeconds);
          }
}

//////////////////////////////////////// SLOW MOVE SERVO ////////////////////////////////////////


void slowMoveServo(enum servos servo, int startPosition, int endPosition) {

  int noOfStep;
  int rotationStep;

  if (endPosition > startPosition) {
      rotationStep = (endPosition - startPosition) / SLOW_MOVE_SERVO_MOVE_DIVIDER;

      for (noOfStep = 0; noOfStep <= SLOW_MOVE_SERVO_MOVE_DIVIDER; noOfStep++) {
          moveServo(servo, noOfStep * rotationStep + startPosition, 1);
      }

  } else {
      rotationStep = (startPosition - endPosition) / SLOW_MOVE_SERVO_MOVE_DIVIDER;

      for (noOfStep = SLOW_MOVE_SERVO_MOVE_DIVIDER; noOfStep >= 0 ; noOfStep--) {
          moveServo(servo, noOfStep * rotationStep + endPosition, 1);
      }
  }
  
}

//////////////////////////////////////// CUT POWER TO SERVO ////////////////////////////////////////

void disableServo(enum servos servo) {
  switch(servo) {
    case servo1: //turns servo 1
        digitalWrite(SERVO1_POW, LOW);
        break;
    
    case servo1_neg: //turns servo 1 (negated)
        digitalWrite(SERVO1_NEG_POW, LOW);
        break;

    case servo2:
      return;

    default:
      return;
  }

  delay(SWITCH_ON_SERVO_LATENCY_MS);
}

//////////////////////////////////////// STEERING POSITION 1 ////////////////////////////////////////

void switchPosition1(void) {
  moveServo(servo1, SERVO1_RIGHT_CLICK, 30);
    if(DIFF) {
      moveServo(servo1, SERVO1_RIGHT_CLICK + DIFF, 10);
    }
  disableServo(servo1);

  moveServo(servo1_neg, SERVO1_LEFT_CLICK, 30);
    if(DIFF) {
      moveServo(servo1_neg, SERVO1_LEFT_CLICK - DIFF, 10);
    }
  disableServo(servo1_neg);
}

//////////////////////////////////////// STEERING POSITION 2 ////////////////////////////////////////

void switchPosition2(void) {
  moveServo(servo1, SERVO1_LEFT_CLICK, 30);
    if(DIFF) {
      moveServo(servo1, SERVO1_LEFT_CLICK - DIFF, 10);
    }
  disableServo(servo1);

  moveServo(servo1_neg, SERVO1_RIGHT_CLICK, 30);
    if(DIFF) {
      moveServo(servo1_neg, SERVO1_RIGHT_CLICK + DIFF, 10);
    }
  disableServo(servo1_neg);
}

//////////////////////////////////////// POSITION 1 ////////////////////////////////////////

void positionBoomBarrierClose(void) {
    
    digitalWrite(LED_PCB1, LOW);
    digitalWrite(LED_PCB2, HIGH);  
    
    activePcbLed = LED_PCB1;
    switchLedState = 0;

    if(distanceSensorMode == boom_barrier_mode) {
      digitalWrite(LED_SIGNAL1, HIGH);
      digitalWrite(LED_SIGNAL2, HIGH);
    }
    else {
      digitalWrite(LED_SIGNAL1, LOW);
      digitalWrite(LED_SIGNAL2, HIGH);    
    }
    activeSignalLed = LED_SIGNAL1;

    if(distanceSensorMode == boom_barrier_mode){
      slowMoveServo(servo2, SERVO2_LEFT_CLICK, SERVO2_RIGHT_CLICK);
      switchPosition1();
    } 
  
    if(distanceSensorMode == switch_mode) {
      switchPosition1();
      slowMoveServo(servo2, SERVO2_LEFT_CLICK, SERVO2_RIGHT_CLICK);
    } 

    if(distanceSensorMode == boom_barrier_mode) {
      digitalWrite(LED_SIGNAL1, HIGH);
      digitalWrite(LED_SIGNAL2, HIGH);
    }
    
    digitalWrite(activePcbLed, LOW);

    actualBoomBarrierPosition = closed;
}

//////////////////////////////////////// POSITION 2 ////////////////////////////////////////

void positionBoomBarrierOpen(void) {
  
    digitalWrite(LED_PCB1, HIGH);
    digitalWrite(LED_PCB2, LOW);      
    
    activePcbLed = LED_PCB2;
    switchLedState = 0;
    
    timerLedBlinkInit();

    activeSignalLed = LED_SIGNAL2;

    if(distanceSensorMode == switch_mode) {
      digitalWrite(LED_SIGNAL1, HIGH);
      digitalWrite(LED_SIGNAL2, LOW);
    }

    if(distanceSensorMode == boom_barrier_mode) {
      slowMoveServo(servo2, SERVO2_RIGHT_CLICK, SERVO2_LEFT_CLICK);
      switchPosition2();
    } 
    
    if(distanceSensorMode == switch_mode) {
      switchPosition2();
      slowMoveServo(servo2, SERVO2_RIGHT_CLICK, SERVO2_LEFT_CLICK);
    } 
    
    digitalWrite(activePcbLed, LOW);

    actualBoomBarrierPosition = opened;
}