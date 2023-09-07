#ifndef TRIXBRIX_DEFINES_H
#define TRIXBRIX_DEFINES_H

//////////////////////////////////////// PINS ////////////////////////////////////////

#define LED_PCB1 10 //PB2 PCINT2 SS OC1B
#define LED_PCB2 7 //PD7 PCINT23 AIN1

#define LED_SIGNAL1 14 //PC0 PCINT8 ADC0
#define LED_SIGNAL2 15 //PC1 PCINT9 ADC1

#define BUTTON_RIGHT 3 //PD3 PCINT19 OC2B INT1
#define BUTTON_LEFT 2 //PD2 PCINT18 INT0 

#define DIST_SENS1 17 //PC3 PCINT11 ADC3 
#define DIST_SENS2 16 //PC2 PCINT10 ADC2

#define SERVO1 18 //PC4 PCINT12 ADC4 SDA
#define SERVO1_POW 4 //PD4 PCINT20
#define SERVO1_NEG 8 //PB0 PCINT0
#define SERVO1_NEG_POW 9 //PB1 PCINT1 OC1A
#define SERVO2 19 //PC5 PCINT13 ADC5 SCL

//////////////////////////////////////// DEFINES ////////////////////////////////////////

//////// DEBUG MODES //////// to enable debug uncoment at least debug basic mode or more options 
//#define DEBUG_MODE_BASIC
//#define DEBUG_MODE_PCINT
//#define DEBUG_MODE_TIMERSs
//#define DEBUG_MODE_INTERRUPTS
//#define DEBUG_MODE_COOLDOWN_MEASURE

#define EEPROM_ADDRESS 4

#define SERVO1_RIGHT_CLICK 61 //angle for switch servo in position 1
#define SERVO1_LEFT_CLICK 94 //angle for switch servo in position 2
#define SERVO2_RIGHT_CLICK -2 //angle for boom barrier  servo in posiotion down
#define SERVO2_LEFT_CLICK 86 //angle for boom barrier  servo in posiotion up
#define DIFF 8 //angle which switch servo goes back after reaching position defined by values above, leave 0 for function disable

#define COOLDOWN_MS 1100 //time [ms] during which controller is insensitive for signals from distance sensors, used to avoid false reactions caused by gaps between consecutive railroad cars

#define SWITCH_ON_SERVO_DELAY_MS 50 //time [ms] after turning power on the switch servo when command for the servo is going to be sent, after whole switching sequence power is going to be turn off
#define SWITCH_ON_SERVO_DURATION_MS 180 //time [ms] for which switch servo is powered in single sequence and can move
#define SWITCH_ON_SERVO_LATENCY_MS 500 //latency [ms] which indicates how fast consecutive switch sequences can be performed

#define SLOW_MOVE_SERVO_MOVE_DIVIDER 44 //number of steps in which single boom barrier servo move is divided, the more steps the slower move is going to be performed
#define MOVE_SERVO_PERIOD_US 20000 //period [us] of a single steering signal, it's duty depends on angle where servo should stops

#endif