#include "sonic.h" 

void sonic_init(Sonic *sonic, GPIO_TypeDef *GPIOx, TIM_HandleTypeDef *htim,
uint16_t echo_pin, uint16_t trigger_pin)
{
	sonic -> GPIOx       = GPIOx ;
	sonic -> htim  			 = htim ;
	sonic -> state       = IDLE_STATE;
	sonic -> echo_pin    = echo_pin;
	sonic -> trigger_pin = trigger_pin;
	sonic -> state       = IDLE_STATE;
}

static void sonic_start(Sonic *sonic)
{
	if(sonic -> state == IDLE_STATE)
	{
		HAL_GPIO_WritePin(sonic -> GPIOx ,sonic -> trigger_pin,(GPIO_PinState)0);
		HAL_Delay(1); 		
		HAL_GPIO_WritePin(sonic -> GPIOx ,sonic -> trigger_pin,(GPIO_PinState)1);
		HAL_Delay(1); // If can, should change to delay_us()
		HAL_GPIO_WritePin(sonic -> GPIOx ,sonic -> trigger_pin,(GPIO_PinState)0);		
		sonic -> state = WAIT_RISING_STATE;
	}
}

float sonic_dist(Sonic *sonic)
{
	
	static float kc = 0;
	sonic_start(sonic);
	if(sonic -> state == COMPLETE_STATE)
	{		
		//Compute the distance
		kc = 0.017 * sonic -> htim -> Instance -> CNT;
		sonic -> state = IDLE_STATE;
	}
	return kc;
	
}

void sonic_callback(Sonic *sonic)
{
	switch(sonic -> state)
	{
		case WAIT_RISING_STATE:
			if(HAL_GPIO_ReadPin(sonic -> GPIOx, sonic -> echo_pin) == 1)
			{
				sonic -> htim -> Instance ->CNT = 0;
				sonic -> state = WAIT_FALLING_STATE;
				HAL_TIM_Base_Start(sonic -> htim);
			}
			else
			{
				sonic -> state = IDLE_STATE;
			}
			break;
		case WAIT_FALLING_STATE:
			if(HAL_GPIO_ReadPin(sonic -> GPIOx,sonic -> echo_pin) == 0)
			{
				HAL_TIM_Base_Stop(sonic -> htim);
				sonic -> state = COMPLETE_STATE;
			}
			else
			{
				sonic -> state = IDLE_STATE;
			}
			break;
		default:
			break;
	}	
}


