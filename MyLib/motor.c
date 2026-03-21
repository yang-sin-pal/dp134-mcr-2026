#include "motor.h"


#define MAX_SPEED 100 
#define MIN_SPEED 0

void motors_init(Motor *motor, TIM_HandleTypeDef *htim,  GPIO_TypeDef *gpio_ports[], uint16_t channel[]){
	
	motor->htim            = htim;	
	/// f = 5Khz -> 72Mhz/(5000* (450 -1)) = 32 
	htim->Instance->PSC    = 31; // 72 -1 
	htim->Instance->ARR    = 449; // 150 -1 150Hz
	
	motor -> left_motor_channel = channel[4];
	motor -> right_motor_channel = channel[5];
	motor -> brake_left_pin  = channel[0];
	motor -> brake_right_pin = channel[1];
	motor -> dir_left_pin    = channel[2];
	motor -> dir_right_pin   = channel[3];
	
	motor -> GPIO_BRK_PORT   = gpio_ports[0];
	motor -> GPIO_DIR_PORT   = gpio_ports[1];
	
	motor -> left_speed  = 0;
	motor -> right_speed = 0;
	
	HAL_TIM_PWM_Start(htim, motor ->left_motor_channel);
	HAL_TIM_PWM_Start(htim, motor ->right_motor_channel);
}

void set_speed(Motor *motor, int left_speed, int right_speed){
	 if(left_speed >= 0)
	 {
		 // run forward
		 HAL_GPIO_WritePin(motor -> GPIO_DIR_PORT, motor -> dir_left_pin,(GPIO_PinState) 1);
		 //HAL_Delay(1);
	 }
	 else{
		 left_speed = -left_speed;	 
		 HAL_GPIO_WritePin(motor -> GPIO_DIR_PORT, motor -> dir_left_pin,(GPIO_PinState) 0);
		 //HAL_Delay(1);
	 }

	 if(right_speed >= 0)
	 {
		 // run forward
		HAL_GPIO_WritePin(motor -> GPIO_DIR_PORT, motor -> dir_right_pin,(GPIO_PinState) 1);
		// HAL_Delay(1);
	 }
	 else{
		right_speed = -right_speed;
		HAL_GPIO_WritePin(motor -> GPIO_DIR_PORT, motor -> dir_right_pin,(GPIO_PinState) 0);
		//HAL_Delay(1);
	 }

	 if (left_speed > MAX_SPEED) left_speed = MAX_SPEED;
	 if (left_speed < MIN_SPEED ) left_speed = MIN_SPEED;

	 if (right_speed > MAX_SPEED) right_speed = MAX_SPEED;
	 if (right_speed < MIN_SPEED ) right_speed = MIN_SPEED;
	 
	 float left_speed_;
	 float right_speed_;
	 left_speed_   = (float) left_speed  * 449/100;
	 right_speed_  = (float) right_speed * 449/100;
	 	 
	 motor -> left_speed = left_speed_;
	 motor -> right_speed= right_speed_;
	 
	 motor->htim->Instance->CCR3 = (uint16_t) left_speed_;
	 motor->htim->Instance->CCR4 = (uint16_t) right_speed_;
}

void set_brake(Motor *motor, bool brake_left, bool brake_right){
	if(brake_left)
	{
		HAL_GPIO_WritePin(motor -> GPIO_BRK_PORT, motor -> brake_left_pin,(GPIO_PinState) 1); 
	}
	else {
		HAL_GPIO_WritePin(motor -> GPIO_BRK_PORT, motor -> brake_left_pin,(GPIO_PinState) 0); 
	}
	
	if(brake_right)
	{
		HAL_GPIO_WritePin(motor -> GPIO_BRK_PORT, motor -> brake_right_pin,(GPIO_PinState) 1); 
	}
	else {
		HAL_GPIO_WritePin(motor -> GPIO_BRK_PORT, motor -> brake_right_pin,(GPIO_PinState) 0); 
	}
}
