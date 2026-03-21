#include "leds.h" 

void leds_init(LEDS *led, GPIO_TypeDef *GPIOx, uint16_t pins[])
{
	
	led -> GPIOx = GPIOx;
	
	led -> latch_pin = pins[0];
	led -> data_pin  = pins[1];	
	led -> clock_pin = pins[2];
	led -> oe_pin    = pins[3];
	led -> srclr_pin = pins[4];
	
	HAL_GPIO_WritePin(led -> GPIOx, led -> latch_pin,   GPIO_PIN_RESET);
	HAL_GPIO_WritePin(led -> GPIOx, led -> data_pin ,   GPIO_PIN_RESET);
	HAL_GPIO_WritePin(led -> GPIOx, led -> clock_pin,   GPIO_PIN_RESET);
	HAL_GPIO_WritePin(led -> GPIOx, led -> srclr_pin,     GPIO_PIN_SET);	
	HAL_GPIO_WritePin(led -> GPIOx, led -> oe_pin   ,     GPIO_PIN_SET);	
}


//{GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_14,GPIO_PIN_8, GPIO_PIN_9};//latch,clock,data,oe,sclr
void led_out(LEDS *led, uint8_t data)
{
	
		HAL_GPIO_WritePin(led -> GPIOx, led -> srclr_pin, GPIO_PIN_SET);// SRCLR Pin 
		HAL_GPIO_WritePin(led -> GPIOx, led -> oe_pin   , GPIO_PIN_SET); // OE pin

		uint8_t i =0;
		for(i=0;i < 8;i++){
		if(data&(1<<i)){
		//SER_Pin,1
		HAL_GPIO_WritePin(led -> GPIOx, led -> data_pin, GPIO_PIN_SET); // Data pin - SER 
		}else{
		//SER_Pin,0
		HAL_GPIO_WritePin(led -> GPIOx, led -> data_pin, GPIO_PIN_RESET);// Data pin - SER 
		}
		HAL_GPIO_WritePin(led -> GPIOx, led -> clock_pin,   GPIO_PIN_SET);// CLOCK Pin
		HAL_GPIO_WritePin(led -> GPIOx, led -> clock_pin, GPIO_PIN_RESET);// CLOCK Pin
		}	
		HAL_GPIO_WritePin(led -> GPIOx, led -> latch_pin,   GPIO_PIN_SET);  // LATCH 
		HAL_GPIO_WritePin(led -> GPIOx, led -> latch_pin, GPIO_PIN_RESET);  // LATCH
		HAL_GPIO_WritePin(led -> GPIOx, led -> oe_pin   , GPIO_PIN_RESET);  // OE pin
		HAL_GPIO_WritePin(led -> GPIOx, led -> srclr_pin, GPIO_PIN_RESET);  // SRCLR Pin
	  //HAL_Delay(1);
}


