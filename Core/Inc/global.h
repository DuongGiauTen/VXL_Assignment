/*
 * global.h
 *
 *  Created on: Dec 11, 2025
 *      Author: DUONG DEP TRAI
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"

// Định nghĩa các trạng thái
#define INIT        0
#define RED_GREEN   1
#define RED_YELLOW  2
#define GREEN_RED   3
#define YELLOW_RED  4

#define MAN_INIT          10
#define MAN_RED_CONFIG    11
#define MAN_YELLOW_CONFIG 12
#define MAN_GREEN_CONFIG  13

// Các biến toàn cục
extern int status;
extern int time_red;
extern int time_green;
extern int time_yellow;

// Biến đếm thời gian thực
extern int time_counter_red;
extern int time_counter_green;
extern int time_counter_yellow;

#endif /* INC_GLOBAL_H_ */
