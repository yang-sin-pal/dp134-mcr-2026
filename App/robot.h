#ifndef ROBOT_H
#define ROBOT_H

#include "stm32f1xx_hal.h"
#include <stdbool.h>

typedef enum{
	STOP                    = 0,
	
	BOOST_AFTER_START       = 10,
	NORMAL_TRACE            = 11,               //11
	LARGE_LEFT_TURN         = 12,            //12
	LARGE_RIGHT_TURN        = 13,           //13
	

	CROSS_LINE_ONE          = 21,             //21
	CROSS_LINE_TWO          = 22,             //22
	TRACE_AFTER_CROSS_LINE  = 23,     //23

	PREPARE_TO_TURN_LEFT    = 31,       //31
	TURN_LEFT_90            = 32,               //32 
	END_TURN_LEFT_90        = 33,           //33
//	STABLIZE_90_LEFT        = 34,
//	AFTER_90_LEFT           = 35,
	
	PREPARE_TO_TURN_RIGHT   = 41,      //41
	TURN_RIGHT_90           = 42,              //42   
	END_TURN_RIGHT_90       = 43,          //43 
//	STABLIZE_90_RIGHT       = 44,
//	AFTER_90_RIGHT          = 45,

	RIGHT_HALF_LANE_ONE     = 51,          //51
	RIGHT_HALF_LANE_TWO     = 52,          //52
	CHANGE_RIGHT_LANE       = 53,            //53
	STABLIZE_RIGHT	        = 54,				//54
	END_CHANGE_RIGHT_LANE   = 55,        //55
//  AFTER_CHANGE_RIGHT_LANE = 56,


	LEFT_HALF_LANE_ONE      = 61,           //61
	LEFT_HALF_LANE_TWO      = 62,           //62
	CHANGE_LEFT_LANE        = 63,             //63
	STABLIZE_LEFT           = 64,
	END_CHANGE_LEFT_LANE    = 65,         //65
 // AFTER_CHANGE_LEFT_LANE = 66,   
					
//	BOOST_AFTER_CHANGE_LANE = 70, 
//	BOOST_AFTER_TURN90      = 80,
  	BOOST_AT_BEGIN          = 90,
//	TRACE_AFTER_CROSS_LINE2 = 100, 
	STABLIZE_AFTER_90       = 110,
	//ACCELLERATION  =  81,	
}run_case;


typedef struct{
	uint32_t cnt1,cnt2,cnt3,brake_time,run_time;
	uint32_t min_brake_time, max_brake_time, brake_time1, brake_time2;
	uint16_t straight_line_time;
	int8_t angle1,speed1,angle2,speed2,angle3,speed3,angle4,speed4,angle5,speed5,angle6,speed6,angle7,speed7;
	float dist;
	bool  ready,run, advanced_on;
	uint8_t sw_mask;
	uint16_t sensor_value[8];
	uint8_t sensor_mask;
	int8_t servo_calib;
	int8_t robot_speed;
	int8_t robot_hspeed;
	run_case runcase;
	uint8_t running;
	uint8_t curve_flag, straight_line_flag;			
	int8_t sensor_pos;
	TIM_HandleTypeDef* robot_tim1;
	TIM_HandleTypeDef* robot_tim2;
	TIM_HandleTypeDef* robot_tim3;
	TIM_HandleTypeDef* robot_tim4;
	DMA_HandleTypeDef* robot_dma;
	ADC_HandleTypeDef* robot_adc;
} Robot; 


void robot_init(Robot *robot, ADC_HandleTypeDef* hadc, DMA_HandleTypeDef* hdma_adc, TIM_HandleTypeDef* htim[]);
void timer_handle(Robot *robot);
void loop(Robot *robot);
void run(Robot *robot);
void handle_and_speed(int8_t angle, int8_t speed);
bool check_crossline(void);
bool check_rightline(void);
bool check_leftline(void);
uint8_t sensor_mask(uint8_t sensor, uint8_t MASK);

///-------- advanced function --------///
void robot_high_speed(Robot *robot); 
bool d_check_crossline(void);	
bool d_check_rightline(void);
bool d_check_leftline (void);
uint8_t debounce_check(uint8_t sensor);
void handle_brake(Robot *robot, int8_t handle_angle, int8_t speed);
void handle_brake2(Robot *robot, int8_t _angle, int8_t _speed);
#endif

