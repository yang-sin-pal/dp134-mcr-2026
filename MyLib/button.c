#include "button.h"

//---------------var button------------
__weak void	btn_pressing_callback(Button *ButtonX){}
__weak void btn_press_short_callback(Button *ButtonX){}
__weak void btn_release_callback(Button *ButtonX){}
__weak void btn_press_timeout_callback(Button *ButtonX){}
	
void button_handle(Button *ButtonX)
{
	//------------------ Loc nhieu ------------------------
	uint8_t sta = HAL_GPIO_ReadPin(ButtonX->GPIOx, ButtonX->GPIO_Pin);
	if(sta != ButtonX->btn_filter)
	{
		ButtonX->btn_filter = sta;
		ButtonX->is_debouncing = 1;
		ButtonX->time_deboune = HAL_GetTick();
	}
	//------------------ Tin hieu da xac lap------------------------
	if(ButtonX->is_debouncing && (HAL_GetTick() - ButtonX->time_deboune >= 15))
	{
		ButtonX->btn_current = ButtonX->btn_filter;
		ButtonX->is_debouncing =0;
	}
	//---------------------Xu li nhan nha------------------------
	if(ButtonX->btn_current != ButtonX->btn_last)
	{
		if(ButtonX->btn_current == 0)//nhan xuong
		{
			ButtonX->is_press_timeout = 1;
			btn_pressing_callback(ButtonX);
			ButtonX->time_start_press = HAL_GetTick();
		}
		else //nha nut
		{
			if(HAL_GetTick() - ButtonX->time_start_press <= 1000)
			{
				btn_press_short_callback(ButtonX);
			}
			btn_release_callback(ButtonX);
			ButtonX->is_press_timeout = 0;
		}
		ButtonX->btn_last = ButtonX->btn_current;
	}
	//-------------Xu li nhan giu----------------
	if(ButtonX->is_press_timeout && (HAL_GetTick() - ButtonX->time_start_press >= 3000))
	{
		btn_press_timeout_callback(ButtonX);
		ButtonX->is_press_timeout =0;
	}
}

void button_multi_handle(Button *ButtonX[], uint8_t num_button)
{
	uint8_t i =0;
	for (i = 0; i < num_button; i++)
	{	
		//------------------ Loc nhieu ------------------------
		uint8_t sta = HAL_GPIO_ReadPin(ButtonX[i]->GPIOx, ButtonX[i]->GPIO_Pin);
		if(sta != ButtonX[i]->btn_filter)
		{
			ButtonX[i]->btn_filter = sta;
			ButtonX[i]->is_debouncing = 1;
			ButtonX[i]->time_deboune = HAL_GetTick();
		}
		//------------------ Tin hieu da xac lap------------------------
		if(ButtonX[i]->is_debouncing && (HAL_GetTick() - ButtonX[i]->time_deboune >= 15))
		{
			ButtonX[i]->btn_current = ButtonX[i]->btn_filter;
			ButtonX[i]->is_debouncing =0;
		}
		//---------------------Xu li nhan nha------------------------
		if(ButtonX[i]->btn_current != ButtonX[i]->btn_last)
		{
			if(ButtonX[i]->btn_current == 0)//nhan xuong
			{
				ButtonX[i]->is_press_timeout = 1;
				ButtonX[i]->state = 1;
				ButtonX[i]->time_start_press = HAL_GetTick();
			}
			else //nha nut
			{
				
				ButtonX[i]-> state = 0;
				ButtonX[i]-> is_press_timeout = 0;
			}
			ButtonX[i]->btn_last = ButtonX[i]->btn_current;
		}
	
	}	
}

void button_init(Button *ButtonX, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	ButtonX	->	GPIOx = GPIOx;
	ButtonX	->	GPIO_Pin = GPIO_Pin;
	ButtonX	-> 	state = 0;
}

uint8_t get_button(Button *ButtonX)
{
	if (ButtonX -> state)  // If button is pressed -> return 1 for future utiliztions 
		return 1;
	return 0;
}

