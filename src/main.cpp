#include "Trixbrix/Trixbrix.h"
#include <EEPROM.h>

volatile enum boom_barrier_position actualBoomBarrierPosition;
char activePcbLed;
char activeSignalLed;
enum mode distanceSensorMode;
char ledChangeMode;
volatile char buttonPressedOrDistSensWorkedInSwitchMode;
volatile char distSensWorkedInSwitchModeInQueue;
volatile bool risingOrFallingDistSens1;
volatile bool risingOrFallingDistSens2;
volatile char distSensWorkedInBoomBarrierMode;
volatile char distSensWorkedInBoomBarrierMode_StateToSwitchEnd;
volatile char distSens1_SwitchModeCooldownLock;
volatile char distSens2_SwitchModeCooldownLock;
volatile char whichDistSensWorkedPrev;
volatile bool distSens1_PrevState;
volatile bool distSens2_PrevState;
volatile int iteratorTimer0Loop;
volatile int iteratorTimer1Loop;
volatile int iteratorTimer2Loop;
volatile char switchLedState;
volatile enum train_detected newTrainDetectedInBoomBarrierMode;
volatile unsigned long currTime;

//////////////////////////////////////// SETUP ////////////////////////////////////////

void setup() {

  pinMode(LED_PCB1, OUTPUT);
  pinMode(LED_PCB2, OUTPUT);
  pinMode(LED_SIGNAL1, OUTPUT);
  pinMode(LED_SIGNAL2, OUTPUT);
  pinMode(BUTTON_RIGHT, INPUT);
  pinMode(BUTTON_LEFT, INPUT);
  pinMode(SERVO1, OUTPUT);
  pinMode(SERVO1_POW, OUTPUT);
  pinMode(SERVO1_NEG, OUTPUT);
  pinMode(SERVO1_NEG_POW, OUTPUT);
  pinMode(SERVO2, OUTPUT);
  pinMode(DIST_SENS1, INPUT);
  pinMode(DIST_SENS2, INPUT);

  digitalWrite(LED_PCB1, LOW);
  digitalWrite(LED_PCB2, LOW);

  digitalWrite(LED_SIGNAL1, LOW);
  digitalWrite(LED_SIGNAL2, LOW);
  #ifdef DEBUG_MODE_BASIC
  Serial.begin(9600);
  Serial.println("Communication init baudrate 9600");
  #endif

  actualBoomBarrierPosition = closed;
  activePcbLed = 0;
  activeSignalLed = 0;
  ledChangeMode = 0;
  buttonPressedOrDistSensWorkedInSwitchMode = 0;
  distSensWorkedInSwitchModeInQueue = 0;
  risingOrFallingDistSens1 = 0;
  risingOrFallingDistSens2 = 0;
  distSensWorkedInBoomBarrierMode = 0;
  distSensWorkedInBoomBarrierMode_StateToSwitchEnd = 0;
  distSens1_SwitchModeCooldownLock = 0;
  distSens2_SwitchModeCooldownLock = 0;
  whichDistSensWorkedPrev = 0;
  iteratorTimer0Loop = 0;
  iteratorTimer1Loop = 0;
  iteratorTimer2Loop = 0;
  switchLedState = 0;
  newTrainDetectedInBoomBarrierMode = not_detected;
  distSens1_PrevState = digitalRead(DIST_SENS1);
  distSens2_PrevState = digitalRead(DIST_SENS2);

  distanceSensorMode = (mode)EEPROM.read(EEPROM_ADDRESS);

  switchPosition1();
  moveServo(servo2, SERVO2_RIGHT_CLICK, 30);

  if((distanceSensorMode != boom_barrier_mode)) {
    
        distanceSensorMode = switch_mode;
        slopeDetDistSens1Init(rising);
        slopeDetDistSens2Init(rising);
        digitalWrite(LED_SIGNAL2, HIGH);
        
        if(EEPROM.read(EEPROM_ADDRESS) != (char)switch_mode) EEPROM.write(EEPROM_ADDRESS, (char)switch_mode);
        #ifdef DEBUG_MODE_BASIC
        Serial.println("Enter switch mode");
        #endif
   }
   
  else if(distanceSensorMode == boom_barrier_mode) {
    
        slopeDetDistSens1Init(falling);
        slopeDetDistSens2Init(falling);
        digitalWrite(LED_SIGNAL1, HIGH);
        digitalWrite(LED_SIGNAL2, HIGH);
        
        if(EEPROM.read(EEPROM_ADDRESS) != (char)boom_barrier_mode) EEPROM.write(EEPROM_ADDRESS, (char)boom_barrier_mode);
        #ifdef DEBUG_MODE_BASIC
        Serial.println("Enter boom barriers mode");
        #endif
   }
   
  timerCooldownDeinit();
  sei(); 
  
  digitalWrite(LED_PCB2, HIGH);
  activePcbLed = LED_PCB1;
  activeSignalLed = LED_SIGNAL1;
}

//////////////////////////////////////// LOOP ////////////////////////////////////////

void loop() {

//////////////////////////////////////// BUTTON IS PRESSED ////////////////////////////////////////

  if(!digitalRead(BUTTON_RIGHT) && digitalRead(BUTTON_LEFT) && actualBoomBarrierPosition == opened) {

        delay(100);
        if(!digitalRead(BUTTON_RIGHT) && digitalRead(BUTTON_LEFT) && actualBoomBarrierPosition == opened) {

            buttonPressedOrDistSensWorkedInSwitchMode = 1;

            if(distanceSensorMode == boom_barrier_mode)
              resetStateAfterBoomBarrierSeq();

            #ifdef DEBUG_MODE_BASIC
            Serial.println("RIGHT pressed");
            #endif
        }
  }

  if(!digitalRead(BUTTON_LEFT) && digitalRead(BUTTON_RIGHT) && actualBoomBarrierPosition == closed) {
      
      delay(100);
      if(!digitalRead(BUTTON_LEFT) && digitalRead(BUTTON_RIGHT) && actualBoomBarrierPosition == closed) {

            buttonPressedOrDistSensWorkedInSwitchMode = 1;

            #ifdef DEBUG_MODE_BASIC
            Serial.println("LEFT pressed");
            #endif
      }
  }

//////////////////////////////////////// SWITCH POSITION ////////////////////////////////////////

  if(buttonPressedOrDistSensWorkedInSwitchMode ||
    (distanceSensorMode == switch_mode && (distSensWorkedInSwitchModeInQueue)) ||
    (distanceSensorMode == boom_barrier_mode && (distSensWorkedInBoomBarrierMode || distSensWorkedInBoomBarrierMode_StateToSwitchEnd))) {
    
        if(distanceSensorMode == boom_barrier_mode)
          distSensWorkedInBoomBarrierMode = 0;

        actualBoomBarrierPosition == opened ? positionBoomBarrierClose() : positionBoomBarrierOpen();

        if(distanceSensorMode == boom_barrier_mode)
          distSensWorkedInBoomBarrierMode_StateToSwitchEnd = 0;
        
        else if(distanceSensorMode == switch_mode &&
          !buttonPressedOrDistSensWorkedInSwitchMode)  
              distSensWorkedInSwitchModeInQueue = 0;

        if(buttonPressedOrDistSensWorkedInSwitchMode)
              buttonPressedOrDistSensWorkedInSwitchMode = 0;
  }

//////////////////////////////////////// SWITCH/BOOM BARRIERS MODE CHANGE ////////////////////////////////////////

  if ((digitalRead(BUTTON_RIGHT) == 0) && (digitalRead(BUTTON_LEFT) == 0)) {

    delay(1500);
    if ((digitalRead(BUTTON_RIGHT) == 0) && (digitalRead(BUTTON_LEFT) == 0)) {

      bothLedBlink(3);

      if (actualBoomBarrierPosition == closed) {
        newTrainDetectedInBoomBarrierMode = not_detected;
        slopeDetDistSens1Init(falling);
        slopeDetDistSens2Init(falling);
      }
      else {
        newTrainDetectedInBoomBarrierMode = detected;
        slopeDetDistSens1Init(rising);
        slopeDetDistSens2Init(rising);
      }

      whichDistSensWorkedPrev = 0;

      distSens1_SwitchModeCooldownLock = 0;
      distSens2_SwitchModeCooldownLock = 0;
      distSensWorkedInSwitchModeInQueue = 0;

      timerCooldownDeinit();

      if (distanceSensorMode == boom_barrier_mode) {
        distanceSensorMode = switch_mode;
        if (EEPROM.read(EEPROM_ADDRESS) != (char)switch_mode) EEPROM.write(EEPROM_ADDRESS, (char)switch_mode);
        slopeDetDistSens1Init(rising);
        slopeDetDistSens2Init(rising);

        bothLedBlink(1);
        digitalWrite(LED_SIGNAL1, HIGH);
        digitalWrite(LED_SIGNAL2, HIGH);
        digitalWrite(activeSignalLed, LOW);
        #ifdef DEBUG_MODE_BASIC
        Serial.println("Enter switch mode");
        #endif
      }
      else {
        distanceSensorMode = boom_barrier_mode;
        if (EEPROM.read(EEPROM_ADDRESS) != (char)boom_barrier_mode) EEPROM.write(EEPROM_ADDRESS, (char)boom_barrier_mode);
        bothLedBlink(2);
        digitalWrite(LED_SIGNAL1, HIGH);
        digitalWrite(LED_SIGNAL2, HIGH);
        #ifdef DEBUG_MODE_BASIC
        Serial.println("Enter boom barriers mode");
        #endif
      }
    }

    digitalWrite(LED_PCB1, HIGH);
    digitalWrite(LED_PCB2, HIGH);
    digitalWrite(activePcbLed, LOW);
  }

}