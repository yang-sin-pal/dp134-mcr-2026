#include "Servo.h"

#define MIN_PULSE_WIDTH      544     // the shortest pulse sent to a servo  
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo 
#define SERVO_CENTER	       1472
#define HANDLE_STEP          10
#define SERVO_PAGE_ADDRESS   0x0801FC00 // Page 127


uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) 
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


uint8_t servo_read(Servo *sv)
{
	return sv -> angle;
}

static void servo_write(Servo *sv, uint8_t angle)
{
	uint16_t ccr = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
	sv -> angle = angle;
	switch(sv->channel)
	{
		case TIM_CHANNEL_1:
			sv->htim->Instance->CCR1 = ccr;
			break;
		case TIM_CHANNEL_2:
			sv->htim->Instance->CCR2 = ccr;
			break;
		case TIM_CHANNEL_3:
			sv->htim->Instance->CCR3 = ccr;
			break;
		case TIM_CHANNEL_4:
			sv->htim->Instance->CCR4 = ccr;
			break;
	}
	
}

void handle(Servo *sv, int _angle)
{
	int ccr = sv -> servo_center + HANDLE_STEP * _angle;
	switch(sv->channel)
	{
		case TIM_CHANNEL_1:
			sv->htim->Instance->CCR1 = ccr;
			break;
		case TIM_CHANNEL_2:
			sv->htim->Instance->CCR2 = ccr;
			break;
		case TIM_CHANNEL_3:
			sv->htim->Instance->CCR3 = ccr;
			break;
		case TIM_CHANNEL_4:
			sv->htim->Instance->CCR4 = ccr;
			break;
	}
}

void 	update_servo_center(Servo *sv,int8_t calib_value)
{
	sv -> servo_center = sv -> servo_center + calib_value;
}

void servo_init(Servo *sv, TIM_HandleTypeDef *htim, uint16_t channel){
	sv->htim            = htim;
	sv->channel         = channel;
	sv->angle           = 0;
	
	sv->servo_center    = SERVO_CENTER;
	
	htim->Instance->PSC = 71; // 72 -1 
	htim->Instance->ARR = 19999; // 20000 - 1
	HAL_TIM_PWM_Start(htim, channel);
	/// Tim2,3,4 privided clock by AHB1 block (36MHz)
}

void read_servo_center(Servo *sv)
{
	
	flash_read_arr(SERVO_PAGE_ADDRESS,(uint8_t *) &(sv -> servo_center), sizeof(sv -> servo_center));
	if(sv -> servo_center > 2400) sv -> servo_center = SERVO_CENTER;
	if(sv -> servo_center < 544)  sv -> servo_center = SERVO_CENTER;
}

void save_servo_center(Servo *sv)
{
	
	flash_unlock();
	flash_erease(SERVO_PAGE_ADDRESS);
	flash_write_arr(SERVO_PAGE_ADDRESS,(uint8_t *) &(sv -> servo_center) , sizeof(sv -> servo_center));
	flash_lock();
	if(sv -> servo_center > 2400) sv -> servo_center = SERVO_CENTER;
	if(sv -> servo_center < 544)  sv -> servo_center = SERVO_CENTER;
	
}
