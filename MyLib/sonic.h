#ifndef SONIC_H
#define SONIC_H
#include "stm32f1xx_hal.h"
#include <stdbool.h>

typedef enum
{
	IDLE_STATE,
	WAIT_RISING_STATE,
	WAIT_FALLING_STATE,
	COMPLETE_STATE
}Sonic_State;


typedef struct
{
	
	GPIO_TypeDef *GPIOx;
	TIM_HandleTypeDef *htim;
	Sonic_State state;
	uint16_t echo_pin;
	uint16_t trigger_pin;

} Sonic; 

void sonic_init(Sonic *sonic, GPIO_TypeDef *GPIOx, TIM_HandleTypeDef *htim,
uint16_t echo_pin, uint16_t trigger_pin);

static void sonic_start(Sonic *sonic);
void sonic_callback(Sonic *sonic);
float sonic_dist(Sonic *sonic);

#endif
