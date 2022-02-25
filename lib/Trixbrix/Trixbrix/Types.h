#ifndef TRIXBRIX_TYPES_H
#define TRIXBRIX_TYPES_H

enum edge_detection {rising, falling}; //rising or falling
enum train_detected {not_detected, detected}; //not detected or detected
enum mode {switch_mode, boom_barrier_mode}; //switch mode or boom barriers mode(1 blik - switch_mode, 2 bliks - boom_barrier_mode while changing mode with both buttons pressed)
enum servos {servo1, servo1_neg, servo2}; //servo1 or servo1 negative or servo2
enum boom_barrier_position {closed, opened}; //closed or opened

#endif