#ifndef TRIXBRIX_SERVOLEDCONTROL_H
#define TRIXBRIX_SERVOLEDCONTROL_H

#include "Trixbrix/Trixbrix.h"

void bothLedBlink(char blinks);
void signalLedChange(int changeMode);
void moveServo(enum servos servo, unsigned int angle, unsigned int timesOfRepeat);
void slowMoveServo(enum servos servo, int startPosition, int endPosition);
void switchPosition1(void);
void switchPosition2(void);
void positionBoomBarrierClose(void);
void positionBoomBarrierOpen(void);

#endif