#ifndef BUZZER_H
#define BUZZER_H
#include "stm32f1xx_hal.h"
#include <stdbool.h>

typedef struct
{
	
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
	uint8_t state;
	uint32_t lastTimeBeeped;
	
} Buzzer; 


void buzzer_init(Buzzer *Buzzer,GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void set_beep(Buzzer *Buzzer, uint16_t beep_time);
void beep_long(Buzzer *Buzzer, uint16_t beep_time);
static void buzzer_set_state(Buzzer *Buzzer, uint8_t state);
static void buzzer_toggle(Buzzer *Buzzer);

#endif
