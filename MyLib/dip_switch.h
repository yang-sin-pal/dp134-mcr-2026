#ifndef DIP_SWITCH_H
#define DIP_SWITCH_H

#include "stm32f1xx_hal.h"
#include <stdbool.h>

typedef struct
{
	uint8_t sw_current;
	uint8_t sw_last;
	uint8_t sw_filter;
	uint8_t is_debouncing;
	uint32_t time_deboune;
	uint32_t time_start_press;
	uint8_t is_press_timeout;
	uint8_t state;
	
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
	
}dip_switch; 
	
void sw_init(dip_switch *sw, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void sw_handle(dip_switch *sw[], uint8_t num_sw);
uint8_t get_sw_mask(dip_switch *sw[], uint8_t num_sw);

#endif
