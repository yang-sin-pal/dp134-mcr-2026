#ifndef SERVO_H
#define SERVO_H
#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include "flash.h" 


typedef struct
{
	
	TIM_HandleTypeDef *htim;
	uint16_t channel;
	uint8_t angle;
	uint16_t servo_center;
	
}Servo;


void servo_init(Servo *sv, TIM_HandleTypeDef *htim, uint16_t channel);
static void servo_write(Servo *sv, uint8_t angle);
void read_servo_center(Servo *sv);
void save_servo_center(Servo *sv);
void handle(Servo *sv, int _angle);
void update_servo_center(Servo *sv,int8_t calib_value);

#endif
