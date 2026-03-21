#ifndef MOTOR_H
#define MOTOR_H
#include "stm32f1xx_hal.h"
#include <stdbool.h>

typedef struct
{
	TIM_HandleTypeDef *htim;
	
	GPIO_TypeDef *GPIO_BRK_PORT;
	GPIO_TypeDef *GPIO_DIR_PORT;
	
	uint16_t left_motor_channel;
	uint16_t right_motor_channel;
		
	uint16_t brake_left_pin;
	uint16_t brake_right_pin;
	uint16_t dir_left_pin;
	uint16_t dir_right_pin;
	
	uint16_t left_speed; 
	uint16_t right_speed;
	
	
} Motor;

void motors_init(Motor *motor,TIM_HandleTypeDef *htim,  GPIO_TypeDef *gpio_ports[], uint16_t channel[]);
void set_speed(Motor *motor, int left_speed, int right_speed);
void set_brake(Motor *motor, bool brake_left, bool brake_right);


#endif
