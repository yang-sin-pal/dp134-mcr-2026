#ifndef ADC_H
#define ADC_H
#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include "flash.h" 


typedef struct
{
	
	ADC_HandleTypeDef *hadc;
	DMA_HandleTypeDef *hdma_adc;
	
	uint16_t sensor_value[8];
	
	uint16_t maxOfMinSensorValue[8];
	uint16_t minOfMaxSensorValue[8];
	uint16_t v_compare[8];

	uint16_t weight[8];
	uint8_t  sensor;
	
} ADC;

void adc_init(ADC *adc, ADC_HandleTypeDef *hadc, DMA_HandleTypeDef *hdma_adc, uint16_t weight[]);
uint8_t get_sensor_mask(ADC *adc);
void update_black(ADC *adc);
void update_white(ADC *adc);
void update_vcompare(ADC *adc);
void write_vcompare(ADC *adc);
void read_vcompare(ADC *adc);


#endif
