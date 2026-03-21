#ifndef BUTTON_H
#define BUTTON_H
#include "stm32f1xx_hal.h"
#include <stdbool.h>

typedef struct
{
	uint8_t btn_current;
	uint8_t btn_last;
	uint8_t btn_filter;
	uint8_t is_debouncing;
	uint32_t time_deboune;
	uint32_t time_start_press;
	uint8_t is_press_timeout;	
	uint8_t state;
	
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
	
}Button; 
	
void button_handle(Button *ButtonX);
void button_init(Button *ButtonX,GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void button_multi_handle(Button *ButtonX[], uint8_t num_button);

uint8_t get_button(Button *ButtonX);

#endif
