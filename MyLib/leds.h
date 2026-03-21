#ifndef LEDS_H
#define LEDS_H
#include "stm32f1xx_hal.h"
#include <stdbool.h>

typedef struct
{
	
	GPIO_TypeDef* GPIOx;
	
	uint8_t  num_of_led;
	uint16_t latch_pin; //SRCLK_PIN
	uint16_t data_pin;  //SER_PIN
	uint16_t clock_pin; //RCLK_PIN
	uint16_t oe_pin;    // Output Enable (OE_PIN)
	uint16_t srclr_pin; //SRCLR
		
}LEDS; 

void leds_init(LEDS *led, GPIO_TypeDef *GPIOx, uint16_t pins[]);
void led_out(LEDS *led, uint8_t data);

#endif
