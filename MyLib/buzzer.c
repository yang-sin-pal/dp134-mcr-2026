#include "buzzer.h" 
#define LOW  0
#define HIGH 1

void buzzer_init(Buzzer *Buzzer,GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	
	Buzzer -> GPIOx = GPIOx;
	Buzzer -> GPIO_Pin = GPIO_Pin;
	Buzzer -> state = LOW;
	Buzzer -> lastTimeBeeped = HAL_GetTick();
	
}

static void buzzer_set_state(Buzzer *Buzzer, uint8_t state) 
{
    
	Buzzer->state = state;
  HAL_GPIO_WritePin(Buzzer->GPIOx, Buzzer->GPIO_Pin, (GPIO_PinState)state);
	
}

static void buzzer_toggle(Buzzer *Buzzer)
{
	
	buzzer_set_state(Buzzer, (GPIO_PinState) !Buzzer->state);
	
}
void beep_long(Buzzer *Buzzer, uint16_t beep_time)
{
	uint32_t time_ = HAL_GetTick();
	
	while(beep_time > 0)
	{	
		HAL_GPIO_WritePin(Buzzer->GPIOx, Buzzer->GPIO_Pin, (GPIO_PinState)1);
		if (HAL_GetTick() - time_ >= 1) 
		{
				beep_time --;
				time_ = HAL_GetTick();
		}
	}
	HAL_GPIO_WritePin(Buzzer->GPIOx, Buzzer->GPIO_Pin, (GPIO_PinState)0);
}


void set_beep(Buzzer *Buzzer, uint16_t beep_time)
{
	if (HAL_GetTick() - Buzzer->lastTimeBeeped >= beep_time) 
	{
			buzzer_toggle(Buzzer);
			Buzzer->lastTimeBeeped = HAL_GetTick();
	}
}

// End 
