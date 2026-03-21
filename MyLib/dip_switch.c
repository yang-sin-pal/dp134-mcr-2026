#include "dip_switch.h"

void sw_handle(dip_switch *sw[], uint8_t num_sw){
		uint8_t i = 0;
		for (i = 0; i < num_sw ; i++)
		{
		//------------------ Loc nhieu ------------------------
		uint8_t sta = HAL_GPIO_ReadPin(sw[i]->GPIOx, sw[i]->GPIO_Pin);
		if(sta != sw[i]->sw_filter)
		{
			sw[i]->sw_filter = sta;
			sw[i]->is_debouncing = 1;
			sw[i]->time_deboune = HAL_GetTick();
		}
		//------------------ Tin hieu da xac lap------------------------
		if(sw[i]->is_debouncing && (HAL_GetTick() - sw[i] -> time_deboune >= 15))
		{
			sw[i]->sw_current    = sw[i]->sw_filter;
			sw[i]->is_debouncing = 0;
		}
		//---------------------Xu li nhan nha------------------------
		if(sw[i]->sw_current != sw[i]->sw_last)
		{
			if(sw[i]->sw_current == 0)//nhan xuong
			{
				sw[i] -> is_press_timeout = 1;
				sw[i] -> state = 0;
				sw[i] -> time_start_press = HAL_GetTick();
			}
			else //nha nut
			{
				sw[i] -> state = 1;
				sw[i] -> is_press_timeout = 0;
			}
			sw[i]->sw_last = sw[i]->sw_current;
		}
	}
}
	
void sw_init(dip_switch *sw,GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	
	sw -> GPIOx    = GPIOx;
	sw -> GPIO_Pin = GPIO_Pin;
	sw -> state    = HAL_GPIO_ReadPin(sw ->GPIOx, sw ->GPIO_Pin);
	
}

uint8_t get_sw_mask(dip_switch *sw[], uint8_t num_sw){
	uint8_t temp = 0;
	uint8_t i    = 0;
	for (i = 0; i < num_sw ; i++)
	{
		temp <<= 1; // Shift left by 1 bit
    temp |= (sw[i]->state == 0) ? 0x01 : 0x00; // Set LSB based on switch state
	}
	return temp;
}
