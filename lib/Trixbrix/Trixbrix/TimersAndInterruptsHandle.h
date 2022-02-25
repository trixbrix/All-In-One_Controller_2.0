#ifndef TRIXBRIX_TIMERS_AND_INTERRUPTS_HANDLE
#define TRIXBRIX_TIMERS_AND_INTERRUPTS_HANDLE

#include "Trixbrix/Trixbrix.h"

void timerLedBlinkInit(void);
void timerLedBlinkDeinit(void);
void timerCooldownInit(int timerInput);
void timerCooldownDeinit(void);
void slopeDetDistSens1Init(enum edge_detection risingOrFalling);
void slopeDetDistSens1Deinit(void);
void slopeDetDistSens2Init(enum edge_detection risingOrFalling);
void slopeDetDistSens2Deinit(void);
void resetStateAfterBoomBarrierSeq(void);

ISR(TIMER0_COMPA_vect);
ISR(TIMER2_COMPA_vect);
ISR(PCINT1_vect);

#endif