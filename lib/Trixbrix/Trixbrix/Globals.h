#ifndef TRIXBRIX_GLOBALS_H
#define TRIXBRIX_GLOBALS_H

#include "Trixbrix/Types.h"

extern volatile enum boom_barrier_position actualBoomBarrierPosition;
extern char activePcbLed;
extern char activeSignalLed;
extern enum mode distanceSensorMode;
extern char ledChangeMode;
extern volatile char buttonPressedOrDistSensWorkedInSwitchMode;
extern volatile char distSensWorkedInSwitchModeInQueue;
extern volatile bool risingOrFallingDistSens1;
extern volatile bool risingOrFallingDistSens2;
extern volatile char distSensWorkedInBoomBarrierMode;
extern volatile char distSensWorkedInBoomBarrierMode_StateToSwitchEnd;
extern volatile char distSens1_SwitchModeCooldownLock;
extern volatile char distSens2_SwitchModeCooldownLock;
extern volatile char whichDistSensWorkedPrev;
extern volatile bool distSens1_PrevState;
extern volatile bool distSens2_PrevState;
extern volatile int iteratorTimer0Loop;
extern volatile int iteratorTimer1Loop;
extern volatile int iteratorTimer2Loop;
extern volatile char switchLedState;
extern volatile enum train_detected newTrainDetectedInBoomBarrierMode;
extern volatile unsigned long currTime;

#endif
