#include "adc.h"

#define ADC_PAGE_ADDRESS 0x0801F800  // Page 126

void adc_init(ADC *adc, ADC_HandleTypeDef *hadc, DMA_HandleTypeDef *hdma_adc, uint16_t weight[])
{
	
	adc -> hadc            = hadc ;
	adc -> hdma_adc  			 = hdma_adc ;
	HAL_ADC_Start_DMA(hadc, (uint32_t *) adc -> sensor_value, 8);
//	uint16_t weight[8] = {500,500,500,500,500,500,500,500};
	for(int i =0; i < 8; i++)
	{
		adc -> weight[i] = weight[i];
	}
}

void update_black(ADC *adc)
{
	uint8_t i;
	for (i=0; i<8; i++)
	{
		adc -> maxOfMinSensorValue[i] = adc -> sensor_value[i];
	}
	for (i=0; i<8; i++)
	{
		adc -> v_compare[i] = ((adc ->maxOfMinSensorValue[i] + adc ->minOfMaxSensorValue[i])/2) + adc -> weight[i];
	}
}

void update_white(ADC *adc)
{
	uint8_t i;
	for (i=0; i<8; i++)
	{
		adc -> minOfMaxSensorValue[i] = adc ->sensor_value[i];
	}
	for (i=0; i<8; i++)
	{
		adc -> v_compare[i] = ((adc ->maxOfMinSensorValue[i] + adc ->minOfMaxSensorValue[i])/2) + adc -> weight[i];
	}
}

uint8_t get_sensor_mask(ADC *adc)
{
	uint8_t temp = 0;
	uint8_t i = 0;
	for (i = 0; i < 8; i++)
	{
		temp = temp << 1;
		if (adc -> sensor_value[i] > adc -> v_compare[i]) // black
		{
			temp |= 0x01; //0b00000001
		}
		else
		{
			temp &= 0xfe; //0b11111110
		}
	}
	
	return temp;
}

void write_vcompare(ADC *adc)
{
	
	uint8_t vcompare_save[16];
	uint8_t i;
	for (i =0; i< 8; i++)
	{
		adc -> v_compare[i] &= 0xFFF;
		vcompare_save[2 * i]   =  (adc -> v_compare[i] >> 8) & 0xFF; // high byte
		vcompare_save[2 * i + 1] =   adc -> v_compare[i] & 0xFF; // low byte
	}
	
	flash_unlock();
	flash_erease(ADC_PAGE_ADDRESS);
	flash_write_arr(ADC_PAGE_ADDRESS,(uint8_t *) &vcompare_save , sizeof(vcompare_save));
	flash_lock();
	
}

void read_vcompare(ADC *adc)
{
	
	uint8_t i;
	uint8_t vcompare_read[16];
	flash_read_arr(ADC_PAGE_ADDRESS,(uint8_t *) &vcompare_read, sizeof(vcompare_read));
	
	for (i = 0; i < 8; i++)
	{
		adc -> v_compare [i] = (vcompare_read[2 * i] << 8) | vcompare_read[2 * i + 1]; // combine high byte and low byte to 10 bit value
	}
	
}

