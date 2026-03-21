#ifndef PARAMS_H
#define PARAMS_H
#include "stm32f1xx_hal.h"
#include <stdbool.h>

#define         MASK2_2         0x66    /* xooxxoox   01100110                  */
#define         MASK2_0         0xc0    /* ooxxxxxx   01100000                  */
#define         MASK0_2         0x03    /* xxxxxxoo   00000110                  */
#define         MASK3_3         0xe7    /* oooxxooo   11100111                  */
#define         MASK0_3         0x07    /* xxxxxooo   00000111                  */
#define         MASK3_0         0xe0    /* oooxxxxx   11100000                  */
#define         MASK4_0         0xf0    /* ooooxxxx   11110000                  */
#define         MASK0_4         0x0f    /* xxxxoooo   00001111                  */
#define         MASK1_1         0x81    /* oxxxxxxo   01111110                  */
#define					MASK4_4					0xff    /* oooooooo	  11111111									*/

#define ROBOT_SPEED 40               // TOC DO CUA XE
#define ROBOT_HIGH_SPEED 65         // HIGH SPEED -> Advanced Functions
#define TIME_TO_BOOST  100          //ms 
#define TIME_AFTER_TURN_90 100     //ms 
#define CHANGE_LANE_TIME   100     //ms
#define CHANGE_LANE_FACTOR 0.35 
// Percent of speed remains after change lane detection
#define CROSS_LINE_FACTOR  0.35    // Percent of speed remains after cross line detection
#define RIGHT_CHANGE_LANE_ANGLE -12
#define LEFT_CHANGE_LANE_ANGLE  12


#define BRAKE_TIME 0   //ms
#define BRAKE_TIME1 180 //ms
#define BRAKE_TIME2 50 //ms
#define MAX_BRAKE_TIME 1000 //ms 
#define MIN_BRAKE_TIME 0 //ms  
#define STRAIGHT_LINE_TIME 5  // ms
// Some methods to increase the speed of the car after the change lane or turn 90.
#define BOOST_TIME_AFTER90   100 //ms 
#define BOSST_TIME_AFTER_CHANGE_LANE 100 // ms
#define SPEED_AFTER_CHANGE_LANE  80 
#define SPEED_AFTER_TURN_90 80 
uint16_t calib_weight[8] = {400,350,340,330,330,330,385,250};  // HIEU CHINH GIA TRI CHO TUNG CAM BIEN
/////----------- Differential values -------------------//////
const char coeffR1[50] = {
	100,	98, 	96, 	94, 	93, 	91, 	90, 	88, 	86, 	85,	
	83, 	82, 	80, 	79, 	77, 	76, 	74, 	73, 	72, 	70,
	69, 	67, 	66, 	64, 	63, 	62, 	60, 	59, 	58, 	56,
	55, 	53, 	52, 	50, 	49, 	48, 	46, 	45, 	43, 	42,
	40, 	39, 	37, 	36, 	34, 	33, 	31, 	30, 	28, 	27
};
// TINH VI SAI


#endif
