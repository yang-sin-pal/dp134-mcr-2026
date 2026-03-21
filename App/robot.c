#include "robot.h" // file dang lam
#include "buzzer.h"
#include "rgb.h"
#include "servo.h"
#include "motor.h"
#include "sonic.h"
#include "button.h"
#include "dip_switch.h" 
#include "adc.h"
#include "flash.h"
#include "leds.h"
#include "params.h"
#include "calib.h" 

Buzzer     buzzer;
RGB        rgb;
Servo      servo;
Motor      motor;
Sonic      sonic;
dip_switch dip1, dip2, dip3;
Button     button1, button2, button3;
ADC        line_sensor;
LEDS       led;

GPIO_TypeDef   *rgb_ports[] = {GPIOB,GPIOA,GPIOA};
uint16_t         rgb_pins[] = {GPIO_PIN_15,GPIO_PIN_8,GPIO_PIN_9};
GPIO_TypeDef *motor_ports[] = {GPIOA,GPIOB};
uint16_t       motor_pins[] = {GPIO_PIN_10,GPIO_PIN_11,GPIO_PIN_1,GPIO_PIN_4,TIM_CHANNEL_3,TIM_CHANNEL_4};
dip_switch           *dip[] = {&dip1,&dip2,&dip3};
Button            *button[] = {&button1,&button2,&button3};
uint16_t         led_pins[] = {GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_14,GPIO_PIN_8, GPIO_PIN_9};//latch,data,clock,oe,sclr

#define SW1 get_button(&button1)
#define SW2 get_button(&button2)
#define SW3 get_button(&button3)
#define DIP get_sw_mask(dip,3)

////// Function to handle Sonic sensor
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){	
	sonic_callback(&sonic);	
}

void robot_init(Robot *robot,ADC_HandleTypeDef* hadc, DMA_HandleTypeDef* hdma_adc,TIM_HandleTypeDef* htim[]){ // ham khoi tao
	
		robot -> robot_tim1  =	htim[0];
	  robot -> robot_tim1 -> Instance -> PSC  = 71; // 1us
		robot -> robot_tim1 -> Instance -> ARR  = 999;// 1000 us = 1ms
		robot -> robot_tim2  =	htim[1];
		robot -> robot_tim3  =	htim[2];
		robot -> robot_tim4  =	htim[3];
		robot -> robot_dma   =  hdma_adc;
		robot -> robot_adc   =  hadc;
		
		buzzer_init(&buzzer,GPIOB, GPIO_PIN_3);
		rgb_init(&rgb, rgb_ports, rgb_pins);
		servo_init(&servo ,robot -> robot_tim3, TIM_CHANNEL_3);
		motors_init(&motor, robot -> robot_tim2, motor_ports, motor_pins);
		sonic_init(&sonic, GPIOA, robot -> robot_tim4, GPIO_PIN_15, GPIO_PIN_12);	
		sw_init(&dip1,GPIOB,GPIO_PIN_5);
		sw_init(&dip2,GPIOB,GPIO_PIN_6);
		sw_init(&dip3,GPIOB,GPIO_PIN_7);
		button_init(&button1,GPIOC,GPIO_PIN_15);
		button_init(&button2,GPIOC,GPIO_PIN_14);
		button_init(&button3,GPIOC,GPIO_PIN_13);
		adc_init(&line_sensor, robot -> robot_adc ,robot -> robot_dma, calib_weight);
		leds_init(&led, GPIOB, led_pins);
		
		//new 
//		debounce_init(&d_buffer);		
		robot -> cnt1        = 0;
		robot -> ready       = false;
		robot -> run         = false;
		robot -> dist        = 0;
		robot -> sw_mask     = 0;
		robot -> sensor_mask = get_sensor_mask(&line_sensor);
		robot -> servo_calib = 0;
		robot -> robot_speed = ROBOT_SPEED;
		robot -> runcase     = STOP;	
		robot -> running     = 0 ;
		read_servo_center(&servo);
		handle(&servo,0);
		
		// Advanced
		robot -> robot_hspeed = ROBOT_HIGH_SPEED;
		robot -> cnt2 = 0;
		robot -> cnt3 = 0;
		robot -> run_time = 0;
		robot -> advanced_on = false;  
		uint16_t offset     = 1000; //In case the time is too small for substraction.
		robot -> brake_time =   BRAKE_TIME;
		robot -> straight_line_time =  STRAIGHT_LINE_TIME  ; // Important value -> Must be tuned!!!
		robot -> max_brake_time     =  MAX_BRAKE_TIME  + offset;
		robot -> min_brake_time     =  MIN_BRAKE_TIME  + offset;
		robot -> brake_time1        =  BRAKE_TIME1     + offset;
		robot -> brake_time2        =  BRAKE_TIME2     + offset;
	  /*  brake_time ---------------------------------------------------------------------------------->
				|------------------------------|--------------------------------|----------------------------|
		min_brake_time                  brake_time2                     brake_time1                max_brake_time
		    |                              |                                |                            |
			   set_brake(&motor, 1, 1);         set_speed(&motor, -20, -20);      handle(&servo, angle);
				 set_speed(&motor, 0, 0);                                           set_brake(&motor, 1, 1);
				 set_brake(&motor, 0, 0);                                           set_speed(&motor, 0, 0);
				 handle_and_speed(angle, speed);	                                                      
		*/
}

void loop(Robot *robot){	           // vong lap
	sw_handle(dip, 3);
	button_multi_handle(button, 3);
	switch( DIP)
	{
		case 0:{ // Calibrate sensor value | 000 | Cam bien hoc mau den va trang
			calib_sensor(&line_sensor,button,&rgb,&buzzer);
			break;
		}
		
		case 1:{ // Save and Load sensor value | 100 | Luu lai gia tri cam bien
			write_read_sensor_value(&line_sensor,button,&rgb,&buzzer);
			break;	
		}
			 
		case 2:{ // Calibrate servo center | 010 | Dieu chinh servo
			calib_servo(&servo,button,&rgb,&buzzer);
			break;
		}
		
		case 3:{ // Check motors | 110 | Kiem tra dong co
			motor_check(&motor,button,&rgb);
			break;
		}

		case 4:{ // Run without Gate bar | 001 | Chay khi an nut
			//robot -> advanced_on = true;	
			robot_high_speed(robot);
			//run(robot);
			break;
		}		
		case 5:{ // Check Sonic sensor| 101 | kiem tra sonic
			check_sonic(&sonic, &rgb,&buzzer);
			break;
		}
		case 6:{ // Run with gate bar  | 011 | Chay voi thanh chan co ultrasonic
			robot -> advanced_on = false;
			if (robot -> running == 0){ 
			RunState state = run_with_gate_bar(&sonic,&rgb, SW1, robot -> ready, robot -> run);
			robot->ready = state.ready;
			robot->run = state.run;}
			robot_high_speed(robot);
			//run(robot);	
			break;
		}
		case 7:{ // Preserved
		  break;
		} 
		
		default:{
			rgb_setcolor(&rgb, OFF);
			break;
		}	
	}
}


void timer_handle(Robot *robot){     // timer
	robot -> cnt1++;	
	robot -> cnt2++;	
	robot -> run_time++;
	if(robot -> curve_flag == 0)
	{
		robot -> cnt3++;
	}
	else if (robot -> curve_flag == 1)
	{
		robot -> cnt3 = 0;
		if(robot -> brake_time > robot -> min_brake_time)
		{
			robot -> brake_time --;
		}
		else //if(robot -> brake_time <= robot -> min_brake_time)
		{
			robot -> brake_time = robot -> min_brake_time - 200;
		}
	}				
	if (robot -> cnt3 > robot -> straight_line_time) robot -> brake_time = robot -> max_brake_time;		
				
		led_out(&led, get_sensor_mask(&line_sensor));
}


//-------- Basic functions --------//
void handle_and_speed(int8_t angle, int8_t speed){// HAM HIEU CHINH TOC DO BANH XE VA GOC QUAY SERVO
	int8_t speed1;
	int8_t speed2;
	// Need to calibrate in real case
	if (angle >  25) angle =  25;
	if (angle < -25) angle = -25;
	uint8_t handle_offset = 10;
	handle(&servo, angle);
	if(angle < 0) // turn right
	{	
		angle = -angle;
		if (angle < -20)
		{
		  speed1 = speed;
			speed2 = speed*coeffR1[angle + 3]/100;
			set_speed(&motor,speed1,speed2);
		}
		else
		{
		  speed1 = speed;
			speed2 = speed*coeffR1[angle + handle_offset]/100;
			set_speed(&motor,speed1,speed2);
		}
	}
	
	else if (angle == 0)
	{
		set_speed(&motor,speed,speed);
	}
	
	else if (angle > 0)// turn left
	{
		if (angle < 20)
		{
			speed2 = speed;
		  speed1 = speed*coeffR1[angle + 3]/100;
		  set_speed(&motor,speed1,speed2);
		}
		else
		{
			speed2 = speed;
		  speed1 = speed*coeffR1[angle + handle_offset]/100;
		  set_speed(&motor,speed1,speed2);
		}		
	}
	
}

uint8_t sensor_mask(uint8_t sensor, uint8_t MASK){// HAM MAT NA CHO CAM BIEN		
	return ( sensor & MASK);
}

//-------- advanced functions --------//	
void robot_high_speed(Robot *robot){ // cac truong hop cua xe	
	if (SW1){ 
	  robot ->  cnt1 = 0;
		robot ->  cnt2 = 0;
		robot ->  cnt3 = 0;
		robot ->  run_time = 0;
		robot -> advanced_on = true;
	  robot ->	runcase = STOP;	
	}	
	if (robot -> run_time >= 120000) {
		robot -> runcase = STOP;
	}
	switch(robot -> runcase){
		case STOP:{
			handle_and_speed(0,0);
			if(SW3 || robot -> run)
			{
			
				robot -> runcase = NORMAL_TRACE;
				robot -> running = 1;
			}	
			break;
		}
 		case BOOST_AT_BEGIN:{
			if (robot -> cnt2  < TIME_TO_BOOST)
			{
				switch(sensor_mask(get_sensor_mask(&line_sensor), MASK3_3)){
					case 0x00://000xx000
					{
						handle_and_speed(0, 45);
						break;
					}				
					case 0x04://0000 0100
					{							
						handle_and_speed(-2, 45);
						break;
					}	
					case 0x07://0000 0111
					{							
						handle_and_speed(-3, 45);
						break;
					}
					case 0x10://0010 0000
					{
						handle_and_speed(2, 45);
						break;
					}
					case 0x78://0111 0000 
					{
						handle_and_speed(3, 45);
						break;
					}
				 }			
			}
			else {
			robot -> runcase = NORMAL_TRACE;
			robot -> brake_time = 0;
			break;
			}
		}
		
  	case NORMAL_TRACE:{           //case 11: CHAY BINH THUONG	
				if(d_check_crossline()){robot -> runcase = CROSS_LINE_ONE;     break;}
  			if(d_check_leftline()) {robot -> runcase = LEFT_HALF_LANE_ONE; break;}
				if(d_check_rightline()){robot -> runcase = RIGHT_HALF_LANE_ONE;break;}
				rgb_setcolor(&rgb, PURPLE);
			  switch(sensor_mask(get_sensor_mask(&line_sensor), MASK4_4)){
					case 0x08://0000 1000
					case 0x10://0001 0000
					case 0x18:{//0001 1000			
						handle_and_speed(0, robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = 0;
						robot -> straight_line_flag = 1;
						break;
					}	
					
					case 0x1c:{//0001 1100					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(0 , 0.8 * robot -> robot_hspeed);
							break;
						} 
						handle_and_speed(0, 0.9 * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = 1;
						robot -> straight_line_flag = 1;
						break;
					}
					
					case 0x0c:{//0000 1100 
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(1, 0.75 * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(1, 0.9 * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = 2;
						robot -> straight_line_flag = 1;
						break;
					}		
					
					case 0x04://0000 0100
					case 0x0e:{//0000 1110
					 if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-1, 0.75 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{						
							handle_brake2(robot,-1 , 0.8 * robot -> robot_hspeed);
						}
							handle_and_speed(-2, 0.85 * robot -> robot_hspeed);
					     		robot -> curve_flag = 1;
							robot -> sensor_pos = 3;
							robot -> straight_line_flag = 0;			
							break;
					}	
								
					case 0x06:{//0000 0110
 					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-1, 0.7 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{							
							handle_brake2(robot, -1, 0.7 * robot -> robot_hspeed);
						}
							handle_and_speed(-3, 0.75 * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 4; 
							robot -> straight_line_flag = 0;
							break;	
					}		
					
					case 0x02://0000 0010
					case 0x07:{//0000 0111
					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-2,0.7 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{	
							handle_brake2(robot,-2,0.5 * robot -> robot_hspeed);
						}
							handle_and_speed(-4, 0.68 * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 5; 
							robot -> straight_line_flag = 0;
							break;	
					}	
				
					case 0x03:{//0000 0011
					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-2, 0.75 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{
						 handle_brake2(robot,-2, 0.5 * robot -> robot_hspeed);
						}
							handle_and_speed(-5, 0.65 * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 6; 
							robot -> straight_line_flag = 0;
							break;		
					}					
					
					case 0x01:{//0000 0001					
					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-2, 0.75 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{
						 handle_brake2(robot,-2, 0.5 * robot -> robot_hspeed);
						}
							handle_and_speed(-5,  0.65 * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 6; 
							robot -> straight_line_flag = 0;
							break;			
					}				
					
 					case 0x00:{//0000 0000										
						if (robot -> sensor_pos < -4)
						{
							handle_and_speed(15, 0 * robot -> robot_hspeed);												
							break;
						}
						else if (robot -> sensor_pos > 4)
						{
							handle_and_speed(-15, 0 * robot -> robot_hspeed);
							break;
						}
						break;
					}
								
					
					case 0x38:{//0011 1000
					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(0, 0.8 * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(1, 0.9 * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = -1;
						robot -> straight_line_flag = 1;
						break;
					}
									 
					case 0x30:{//0011 0000							
					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(1, 0.75 * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(0, 0.85 * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = -2;
						robot -> straight_line_flag = 1;
						break;
					}
					
					case 0x20://0010 0000
					case 0x50://0101 0000
					case 0x70:{//0111 0000 			
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(1, 0.7 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{						
							handle_brake2(robot,1, 0.8 * robot -> robot_hspeed);
						}
						handle_and_speed(1, 0.8* robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = -3;
						robot -> straight_line_flag = 1;
						break;		
					}
					
 					case 0x60:{//0110 0000					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(1, 0.7 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{
							handle_brake2(robot, 1, 0.7 * robot -> robot_hspeed);
						}					
							handle_and_speed(2, 0.75 * robot -> robot_hspeed);
							robot -> curve_flag =  1;
							robot -> sensor_pos = -4; 
							robot -> straight_line_flag = 0;	
							break;								
					}
							
					case 0x40://0100 0000
					case 0xa0://1010 0000
					case 0xe0:{//1110 0000					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(1, 0.7 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{
							handle_brake2(robot, 1, 0.5 * robot -> robot_hspeed);
						}
							handle_and_speed(3, 0.68 * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = -5; 
							robot -> straight_line_flag = 0;
				   		break;
					}
					
					case 0xc0:{//1100 0000										
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(2, 0.75 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{
						 handle_brake2(robot, 2, 0.5 * robot -> robot_hspeed);		
						}
						handle_and_speed(5, 0.65 * robot -> robot_hspeed);
						robot -> curve_flag = 1;
						robot -> sensor_pos = -6; 
						robot -> straight_line_flag = 0;
						break;		
					}	
				
					case 0x80:{//1000 0000					   
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(2, 0.75 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{					
							handle_brake2(robot, 2, 0.5 * robot -> robot_hspeed);		
						}
							handle_and_speed(5, 0.65 * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = -6; 
							robot -> straight_line_flag = 0;
							break;	 
					}			
					default: 
						break;			
				}			
			break;
		}		
		case CROSS_LINE_ONE:{         //case 21: DUONG CHEO	
			//set_brake(&motor, 1,1);
			//set_speed(&motor, 0,0);	
			robot -> runcase = CROSS_LINE_TWO;	
			robot -> cnt1 = 0;
			break;
		}
		
 		case CROSS_LINE_TWO:{         //case 22: doi 5ms
				while(robot -> cnt1 < 100)
				{
					set_speed(&motor, 0,0);
					set_brake(&motor, 1,1);
		    	
			switch(sensor_mask(get_sensor_mask(&line_sensor), MASK4_4)){
					case 0x18:{//0001 1000			
						handle_and_speed(0, 0 * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = 0;
						robot -> straight_line_flag = 1;
						break;
					}	
					case 0x1c://0001 1100
					case 0x08:{//0000 1000					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(0, 0* robot -> robot_hspeed);
							break;
						}
						handle_and_speed(0, 0 * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = 1;
						robot -> straight_line_flag = 1;
						break;
					}
					
					case 0x0c:{//0000 1100 
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-1, 0 * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(-1, 0 * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = 2;
						robot -> straight_line_flag = 1;
						break;
					}		
					
					case 0x04://0000 0100
					case 0x0e:{//0000 1110
					 if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-1, 0 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{						
							handle_brake2(robot,-1, 0 * robot -> robot_hspeed);
						}
							handle_and_speed(-1, 0* robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 3;
							robot -> straight_line_flag = 0;			
							break;
					}	
								
					case 0x06:{//0000 0110
 					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-2, 0 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{							
							handle_brake2(robot, -2, 0 * robot -> robot_hspeed);
						}
							handle_and_speed(-2,0 * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 4; 
							robot -> straight_line_flag = 0;
							break;	
					}		
					
					case 0x02://0000 0010
					case 0x07:{//0000 0111
					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-2,0 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{	
							handle_brake2(robot,-2,0 * robot -> robot_hspeed);
						}
							handle_and_speed(-2, 0 * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 5; 
							robot -> straight_line_flag = 0;
							break;	
					}	
				
					case 0x03:{//0000 0011
					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-3,0 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{
						 handle_brake2(robot,-3, 0 * robot -> robot_hspeed);
						}
							handle_and_speed(-3, 0 * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 6; 
							robot -> straight_line_flag = 0;
							break;		
					}					
					
					case 0x01:{//0000 0001					
					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-3, 0* robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{
						 handle_brake2(robot,-3, 0 * robot -> robot_hspeed);
						}
							handle_and_speed(-3,  0 * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 6; 
							robot -> straight_line_flag = 0;
							break;			
					}				
					
					case 0x00:{//0000 0000										
						if (robot -> sensor_pos < -4)
						{
							handle_and_speed(15, 0 * robot -> robot_hspeed);												
							break;
						}
						else if (robot -> sensor_pos > 4)
						{
							handle_and_speed(-15, 0 * robot -> robot_hspeed);
							break;
						}
						break;
					}
								
					case 0x10://0010 0000
					case 0x38:{//0011 1000
					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(0, 0 * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(0, 0 * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = -1;
						robot -> straight_line_flag = 1;
						break;
					}
									
					case 0x30:{//0011 0000							
					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(1, 0 * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(1, 0 * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = -2;
						robot -> straight_line_flag = 1;
						break;
					}
					
					case 0x20://0010 0000
					case 0x70:{//0111 0000 			
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(1, 0 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{						
							handle_brake2(robot,1, 0* robot -> robot_hspeed);
						}
						handle_and_speed(1, 0* robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = -3;
						robot -> straight_line_flag = 1;
						break;		
					}
					
					case 0x60:{//0110 0000					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(2, 0 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{
							handle_brake2(robot, 2, 0 * robot -> robot_hspeed);
						}					
							handle_and_speed(2, 0 * robot -> robot_hspeed);
							robot -> curve_flag =  1;
							robot -> sensor_pos = -4; 
							robot -> straight_line_flag = 0;	
							break;								
					}
							
					case 0x40://0100 0000
					case 0xe0:{//1110 0000					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(2, 0 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{
							handle_brake2(robot, 2, 0* robot -> robot_hspeed);
						}
							handle_and_speed(2, 0 * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = -5; 
							robot -> straight_line_flag = 0;
				   		break;
					}
					
					case 0xc0:{//1100 0000										
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(3, 0 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{
						 handle_brake2(robot, 3, 0 * robot -> robot_hspeed);		
						}
						handle_and_speed(3,0 * robot -> robot_hspeed);
						robot -> curve_flag = 1;
						robot -> sensor_pos = -6; 
						robot -> straight_line_flag = 0;
						break;		
					}	
				
					case 0x80:{//1000 0000					   
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(3 , 0 * robot -> robot_hspeed);
							break;
						}
						else if( robot -> brake_time  > robot -> min_brake_time)
						{					
							handle_brake2(robot, 3, 0 * robot -> robot_hspeed);		
						}

							handle_and_speed(3, 0 * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = -6; 
							robot -> straight_line_flag = 0;
							break;	
					}			
					default: 
						break;			
				}			
				}
				if (robot -> cnt1 >= 100 && robot -> cnt1 <= 130)
				{
					set_speed(&motor, -20,-20);
				}
				else if (robot -> cnt1 > 130)
				{
					//handle(&servo,-10);
					set_brake(&motor, 0,0);
					set_speed(&motor, 20,20);
					if(d_check_crossline() == 0)
					{
						if(d_check_leftline()== 0 && d_check_rightline() ==0)
						{
							set_brake(&motor, 0,0);
							//handle_and_speed(0, 0 * robot -> robot_hspeed);
							robot -> runcase = TRACE_AFTER_CROSS_LINE;
							robot -> cnt1 = 0;
							break;
						}
					}
				}
				break;					
		}	
		
		case TRACE_AFTER_CROSS_LINE:{ //case 23: kiem tra 90 trai hay phai	
			uint8_t mask = get_sensor_mask(&line_sensor);	
			if(  
				   sensor_mask(mask, 0xf0) == 0xf0  //1111 0000
			  || sensor_mask(mask, 0xf8) == 0xf8  //1111 1000
				|| sensor_mask(mask, 0xfc) == 0xfc  //1111 1100
				|| sensor_mask(mask, 0xe0) == 0xe0  //1110 0000
			  ){			
					//robot -> runcase = PREPARE_TO_TURN_LEFT;
						handle(&servo, 18);
						set_brake(&motor, 0,0);
						set_speed(&motor, 0, 55);
						robot -> cnt1 = 0;	
						robot -> runcase  = TURN_LEFT_90;	
				break;
			}		
			if(  
					 sensor_mask(mask, 0x07) == 0x07 //0000 0111
				 ||sensor_mask(mask, 0x0f) == 0x0f //0000 1111
				 ||sensor_mask(mask, 0x1f) == 0x1f //0001 1111
				 ||sensor_mask(mask, 0x3f) == 0x3f //0011 1111
			  ){
					handle(&servo, -18 );
					set_brake(&motor, 0,0);
					set_speed(&motor, 55 , 0);
					robot -> cnt1  = 0;
					robot -> runcase  = TURN_RIGHT_90;
				//robot -> runcase = PREPARE_TO_TURN_RIGHT;
				break;
			}
		  // Type 1 when approaching the strainght line before turning.		
			switch(sensor_mask(get_sensor_mask(&line_sensor), MASK4_4)){
					case 0x18:{//0001 1000			
						handle_and_speed(0, CROSS_LINE_FACTOR * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = 0;
						robot -> straight_line_flag = 1;
						break;
					}	
					case 0x1c://0001 1100
					case 0x08:{//0000 1000					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(0, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(0, CROSS_LINE_FACTOR * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = 1;
						robot -> straight_line_flag = 1;
						break;
					}
					
					case 0x0c:{//0000 1100 
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-1, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(-1, CROSS_LINE_FACTOR * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = 2;
						robot -> straight_line_flag = 1;
						break;
					}		
					
					case 0x04://0000 0100
					case 0x0e:{//0000 1110
					 if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-1, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							break;
						}
							handle_and_speed(-1, CROSS_LINE_FACTOR* robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 3;
							robot -> straight_line_flag = 0;			
							break;
					}	
								
					case 0x06:{//0000 0110
 					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-1,CROSS_LINE_FACTOR * robot -> robot_hspeed);
							break;
						}
							handle_and_speed(-1, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 4; 
							robot -> straight_line_flag = 0;
							break;	
					}		
					
					case 0x02://0000 0010
					case 0x07:{//0000 0111
					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-2,CROSS_LINE_FACTOR * robot -> robot_hspeed);
							break;
						}
							handle_and_speed(-2, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 5; 
							robot -> straight_line_flag = 0;
							break;	
					}	
				
					case 0x03:{//0000 0011
					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-2, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							break;
						}
							handle_and_speed(-2, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 6; 
							robot -> straight_line_flag = 0;
							break;		
					}					
					
					case 0x01:{//0000 0001					
					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-3, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							break;
						}
							handle_and_speed(-3,  CROSS_LINE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 6; 
							robot -> straight_line_flag = 0;
							break;			
					}				
					
					case 0x00:{//0000 0000										
						if (robot -> sensor_pos < -4)
						{
							handle_and_speed(15, CROSS_LINE_FACTOR* robot -> robot_hspeed);												
							break;
						}
						else if (robot -> sensor_pos > 4)
						{
							handle_and_speed(-15, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							break;
						}
						break;
					}
								
					case 0x10://0010 0000
					case 0x38:{//0011 1000
					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(0, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(0, CROSS_LINE_FACTOR * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = -1;
						robot -> straight_line_flag = 1;
						break;
					}
									
					case 0x30:{//0011 0000												
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(1, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(1, CROSS_LINE_FACTOR * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = -2;
						robot -> straight_line_flag = 1;
						break;
					}
					
					case 0x20://0010 0000
					case 0x70:{//0111 0000 			
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(1, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(1, CROSS_LINE_FACTOR * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = -3;
						robot -> straight_line_flag = 1;
						break;		
					}
					
					case 0x60:{//0110 0000					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(2, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							break;
						}					
							handle_and_speed(2, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag =  1;
							robot -> sensor_pos = -4; 
							robot -> straight_line_flag = 0;	
							break;								
					}
							
					case 0x40://0100 0000
					case 0xe0:{//1110 0000					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(2, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							break;
						}
							handle_and_speed(2, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = -5; 
							robot -> straight_line_flag = 0;
				   		break;
					}
					
					case 0xc0:{//1100 0000										
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(3, 0.7 * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(3, CROSS_LINE_FACTOR * robot -> robot_hspeed);
						robot -> curve_flag = 1;
						robot -> sensor_pos = -6; 
						robot -> straight_line_flag = 0;
						break;		
					}	
				
					case 0x80:{//1000 0000					   
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(3, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							break;
						}
							handle_and_speed(3, CROSS_LINE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = -6; 
							robot -> straight_line_flag = 0;
							break;	
					}			
					default: 
						break;			
				}			
			break;
		}	
		
		case PREPARE_TO_TURN_LEFT:{   //CHUAN BI RE TRAI
			if (sensor_mask(get_sensor_mask(&line_sensor), MASK4_4) == 0x00)  // condition to turn left
			{
					handle(&servo, 25);
					set_brake(&motor, 0,0);
					set_speed(&motor, 0, 1 * robot -> robot_hspeed);
					robot -> cnt1 = 0;	
					robot -> runcase  = TURN_LEFT_90;					
			}			
				break;
		}
		
		case TURN_LEFT_90:{           //case 31: doi 50ms
			if (robot -> cnt1 > 80 )
			{
			//HAL_Delay(100);
			robot -> runcase = END_TURN_LEFT_90;
			robot -> cnt1 = 0;		
			}
			break;
		}
		
		case END_TURN_LEFT_90:{       //case 32: ket thuc xoay trai
			uint8_t mask = get_sensor_mask(&line_sensor);
			if(sensor_mask(mask, MASK4_4) == 0x3c //0011 1100
				||  sensor_mask(mask, MASK4_4) == 0xe0     //0000 1110
				||sensor_mask(mask, MASK4_4) == 0x1c	//0001 1100
				||sensor_mask(mask, MASK4_4) == 0x18 	//0001 1000
			) 	
			{	
				robot -> runcase  = NORMAL_TRACE;
				robot -> cnt3 = robot ->straight_line_time;
				break;
			}
			break;
		}

		case PREPARE_TO_TURN_RIGHT:{  //CHUAN BI RE PHAI
			if (sensor_mask(get_sensor_mask(&line_sensor), MASK4_4) == 0x00)  // condition to turn right
			{
			 // Max angle for turning right ==> Imidiately turn cause => cannot reach 0x60
				handle(&servo, -25);
				set_speed(&motor, robot -> robot_hspeed*0.9, -10);
				robot -> cnt1  = 0;
				robot -> runcase  = TURN_RIGHT_90;	
			}
			break;
		
		}
		
		case TURN_RIGHT_90:{          //case 41: doi 50ms
			if (robot -> cnt1  > 80)
			{
				//HAL_Delay(100);
				robot -> runcase = END_TURN_RIGHT_90;
				robot -> cnt1 = 0;
			}
			break;
		}
		
		case END_TURN_RIGHT_90:{      //case 42: ket thuc xoay phai
			uint8_t mask = get_sensor_mask(&line_sensor);
			if(sensor_mask(mask, MASK4_4) == 0x06 //0011 1100
			|| sensor_mask(mask, MASK4_4) == 0x18 //0001 1000
			|| sensor_mask(mask, MASK4_4) == 0x18 //0001 1000
			|| sensor_mask(mask, MASK4_4) == 0x18 //0001 1000
			|| sensor_mask(mask, MASK4_4) == 0x30 //0011 0000
			|| sensor_mask(mask, MASK4_4) == 0x38 //0011 1000
			|| sensor_mask(mask, MASK4_4) == 0x70 //0111 0000
			|| sensor_mask(mask, MASK4_4) == 0x60 //0110 0000
			)
			{	
				robot -> runcase  = NORMAL_TRACE;
				robot -> cnt3 = robot ->straight_line_time;
				break;
			}
			break;
		}
		
 		case RIGHT_HALF_LANE_ONE:{    //case 51: chuyen lan phai		
			// type 1 : Change without brake
			robot -> runcase  = RIGHT_HALF_LANE_TWO;
			// Type 2: Change with brake
//			set_brake(&motor, 1, 1);
//   		set_speed(&motor, 0,0);
//			robot -> runcase  = RIGHT_HALF_LANE_TWO;
//			robot -> cnt1 = 0;
			break;
		}
		
		case RIGHT_HALF_LANE_TWO:{    
			if (robot -> cnt1 > 100)
			{
				set_brake(&motor,0,0);
				handle_and_speed(0, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
				robot -> runcase  = CHANGE_RIGHT_LANE;					
			}
			break;
		}
		
		case CHANGE_RIGHT_LANE:{      //case 53: chuyen lan phai	
			rgb_setcolor(&rgb, OFF);
			if(sensor_mask(get_sensor_mask(&line_sensor), MASK4_4) == 0x00){
				//beep_long(&buzzer,100);
				handle_and_speed(RIGHT_CHANGE_LANE_ANGLE, 0.55	* robot -> robot_hspeed);
				robot -> runcase  = STABLIZE_RIGHT;
				robot -> cnt2 = 0;
				break;
			}				
			// type 3 - change lane 		
       switch(sensor_mask(get_sensor_mask(&line_sensor), MASK4_4)){
					case 0x18:{//0001 1000			
						handle_and_speed(0, CHANGE_LANE_FACTOR* robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = 0;
						robot -> straight_line_flag = 1;
						break;
					}	
					case 0x1c://0001 1100
					case 0x08:{//0000 1000					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(0, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(0, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = 1;
						robot -> straight_line_flag = 1;
						break;
					}
					
					case 0x0c:{//0000 1100 
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-1, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(-1, CHANGE_LANE_FACTOR* robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = 2;
						robot -> straight_line_flag = 1;
						break;
					}		
					
					case 0x04://0000 0100
					case 0x0e:{//0000 1110
					 if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-1, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{						
//							handle_brake2(robot,-5, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
//						}
							handle_and_speed(-1, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 3;
							robot -> straight_line_flag = 0;			
							break;
					}	
								
					case 0x06:{//0000 0110
 					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-2, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{							
//							handle_brake2(robot, -7, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
//						}
							handle_and_speed(-2, 0.7 * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 4; 
							robot -> straight_line_flag = 0;
							break;	
					}		
					
					case 0x02://0000 0010
					case 0x07:{//0000 0111
					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-2,CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{	
//							handle_brake2(robot,-14,CHANGE_LANE_FACTOR * robot -> robot_hspeed);
//						}
							handle_and_speed(-2, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 5; 
							robot -> straight_line_flag = 0;
							break;	
					}	
				
					case 0x03:{//0000 0011
					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-3, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{
//						 handle_brake2(robot,-17, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
//						}
							handle_and_speed(-3, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 6; 
							robot -> straight_line_flag = 0;
							break;		
					}					
					
					case 0x01:{//0000 0001					
					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-3, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{
//						 handle_brake2(robot,-18, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
//						}
							handle_and_speed(-3,  CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 6; 
							robot -> straight_line_flag = 0;
							break;			
					}				
					
					case 0x00:{//0000 0000										
						if (robot -> sensor_pos < -4)
						{
							handle_and_speed(15, CHANGE_LANE_FACTOR * robot -> robot_hspeed);												
							break;
						}
						else if (robot -> sensor_pos > 4)
						{
							handle_and_speed(-15, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
						break;
					}
								
					case 0x10://0010 0000
					case 0x38:{//0011 1000
					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(0, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(0, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = -1;
						robot -> straight_line_flag = 1;
						break;
					}
									
					case 0x30:{//0011 0000							
					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(1, CHANGE_LANE_FACTOR* robot -> robot_hspeed);
							break;
						}
						handle_and_speed(1, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = -2;
						robot -> straight_line_flag = 1;
						break;
					}
					
					case 0x20://0010 0000
					case 0x70:{//0111 0000 			
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(1, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{						
//							handle_brake2(robot,5, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
//						}
						handle_and_speed(1, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = -3;
						robot -> straight_line_flag = 1;
						break;		
					}
					
					case 0x60:{//0110 0000					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(2, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{
//							handle_brake2(robot, 7, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
//						}					
							handle_and_speed(2, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag =  1;
							robot -> sensor_pos = -4; 
							robot -> straight_line_flag = 0;	
							break;								
					}
							
					case 0x40://0100 0000
					case 0xe0:{//1110 0000					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(2, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{
//							handle_brake2(robot, 14, CHANGE_LANE_FACTOR* robot -> robot_hspeed);
//						}
							handle_and_speed(2, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = -5; 
							robot -> straight_line_flag = 0;
				   		break;
					}
					
					case 0xc0:{//1100 0000										
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(3, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{
//						 handle_brake2(robot, 17, CHANGE_LANE_FACTOR * robot -> robot_hspeed);		
//						}
						handle_and_speed(3, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
						robot -> curve_flag = 1;
						robot -> sensor_pos = -6; 
						robot -> straight_line_flag = 0;
						break;		
					}	
				
					case 0x80:{//1000 0000					   
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(3, CHANGE_LANE_FACTOR* robot -> robot_hspeed);
							break;
						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{					
//							handle_brake2(robot, 18, CHANGE_LANE_FACTOR* robot -> robot_hspeed);		
//						}

							handle_and_speed(3, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = -6; 
							robot -> straight_line_flag = 0;
							break;	
					}		
					case 0xe7:
					case 0xff:
					{
						if(d_check_crossline()){robot -> runcase = CROSS_LINE_ONE;break;}
					}					
					default: 
						break;			
				}			
 
			break;
		}
		
		case STABLIZE_RIGHT:{         //ON DINH VAO LANE PHAI
			uint8_t mask = get_sensor_mask(&line_sensor);
			if(sensor_mask(mask, 0x07) != 0
				||sensor_mask(mask, MASK0_4) != 0 )
			{
				handle_and_speed(0, 0.5*robot -> robot_hspeed);
				robot -> runcase  = END_CHANGE_RIGHT_LANE;
				break;
			}
			break;
	}
		
		case END_CHANGE_RIGHT_LANE:{  //case 54: end chuyen lan phai
			uint8_t mask = get_sensor_mask(&line_sensor);
			if (sensor_mask(mask, MASK4_0)  != 0x00
			 || sensor_mask(mask, 0xc3)     != 0xc3 //1100 0011
			 || sensor_mask(mask, 0x60)     == 0x60 //0110 0000
			 || sensor_mask(mask, 0x06)     == 0x06 //0000 0110
			) 
			{
				robot -> cnt3 = robot ->straight_line_time;
				set_brake(&motor, 1,1);
				set_speed(&motor,0,0);
				set_speed(&motor,-20,-20);
				robot -> runcase  = NORMAL_TRACE;	
			}
			break;
		}
		
		case LEFT_HALF_LANE_ONE:{     //case 61: chuyen lan trai
      // type 1 : Change without brake
			robot -> runcase  = LEFT_HALF_LANE_TWO;
			// type 2: Change with brake
//			set_brake(&motor, true, true);
//			set_speed(&motor, 0,0);
//			robot -> runcase = LEFT_HALF_LANE_TWO;
//			robot -> cnt1 = 0;	
			break;
		}
		
		case LEFT_HALF_LANE_TWO:{     //case 62: doi 50ms
		
			if (robot -> cnt1   > 100)
			{
        set_brake(&motor, 0,0);
				handle_and_speed(0, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
				robot -> runcase  = CHANGE_LEFT_LANE;
			}
			break;
		}
		
		case CHANGE_LEFT_LANE:{
				// Trace after right half lane
				if(sensor_mask(get_sensor_mask(&line_sensor), MASK4_4) == 0x00)
				{
					handle_and_speed(LEFT_CHANGE_LANE_ANGLE, 0.5*robot -> robot_hspeed);
					robot -> runcase  = STABLIZE_LEFT;
					robot -> cnt2 = 0;
					break;
				}

		    switch(sensor_mask(get_sensor_mask(&line_sensor), MASK4_4)){
					case 0x18:{//0001 1000			
						handle_and_speed(0, CHANGE_LANE_FACTOR* robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = 0;
						robot -> straight_line_flag = 1;
						break;
					}	
					case 0x1c://0001 1100
					case 0x08:{//0000 1000					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(0, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(0, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = 1;
						robot -> straight_line_flag = 1;
						break;
					}
					
					case 0x0c:{//0000 1100 
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-1, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(-1, CHANGE_LANE_FACTOR* robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = 2;
						robot -> straight_line_flag = 1;
						break;
					}		
					
					case 0x04://0000 0100
					case 0x0e:{//0000 1110
					 if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-1, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
							handle_and_speed(-1, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 3;
							robot -> straight_line_flag = 0;			
							break;
					}	
								
					case 0x06:{//0000 0110
 					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-2, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
							handle_and_speed(-2, 0.7 * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 4; 
							robot -> straight_line_flag = 0;
							break;	
					}		
					
					case 0x02://0000 0010
					case 0x07:{//0000 0111
					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-2,CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
							handle_and_speed(-2, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 5; 
							robot -> straight_line_flag = 0;
							break;	
					}	
				
					case 0x03:{//0000 0011
					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-3, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
							handle_and_speed(-3, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 6; 
							robot -> straight_line_flag = 0;
							break;		
					}					
					
					case 0x01:{//0000 0001					
					
						if (robot -> sensor_pos < -3)
						{
							handle_and_speed(-3, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
							handle_and_speed(-3,  CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = 6; 
							robot -> straight_line_flag = 0;
							break;			
					}				
					
					case 0x00:{//0000 0000										
						if (robot -> sensor_pos < -4)
						{
							handle_and_speed(15, CHANGE_LANE_FACTOR * robot -> robot_hspeed);												
							break;
						}
						else if (robot -> sensor_pos > 4)
						{
							handle_and_speed(-15, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
						break;
					}
								
					case 0x10://0010 0000
					case 0x38:{//0011 1000
					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(0, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
						handle_and_speed(0, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = -1;
						robot -> straight_line_flag = 1;
						break;
					}
									
					case 0x30:{//0011 0000							
					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(1, CHANGE_LANE_FACTOR* robot -> robot_hspeed);
							break;
						}
						handle_and_speed(1, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = -2;
						robot -> straight_line_flag = 1;
						break;
					}
					
					case 0x20://0010 0000
					case 0x70:{//0111 0000 			
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(1, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{						
//							handle_brake2(robot,5, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
//						}
						handle_and_speed(1, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
						robot -> curve_flag = 0;
						robot -> sensor_pos = -3;
						robot -> straight_line_flag = 1;
						break;		
					}
					
					case 0x60:{//0110 0000					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(2, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{
//							handle_brake2(robot, 7, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
//						}					
							handle_and_speed(2, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag =  1;
							robot -> sensor_pos = -4; 
							robot -> straight_line_flag = 0;	
							break;								
					}
							
					case 0x40://0100 0000
					case 0xe0:{//1110 0000					
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(2, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{
//							handle_brake2(robot, 14, CHANGE_LANE_FACTOR* robot -> robot_hspeed);
//						}
							handle_and_speed(2, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = -5; 
							robot -> straight_line_flag = 0;
				   		break;
					}
					
					case 0xc0:{//1100 0000										
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(3, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							break;
						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{
//						 handle_brake2(robot, 17, CHANGE_LANE_FACTOR * robot -> robot_hspeed);		
//						}
						handle_and_speed(3, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
						robot -> curve_flag = 1;
						robot -> sensor_pos = -6; 
						robot -> straight_line_flag = 0;
						break;		
					}	
				
					case 0x80:{//1000 0000					   
						if (robot -> sensor_pos > 3)
						{
							handle_and_speed(3 , CHANGE_LANE_FACTOR* robot -> robot_hspeed);
							break;
						}

							handle_and_speed(3, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
							robot -> curve_flag = 1;
							robot -> sensor_pos = -6; 
							robot -> straight_line_flag = 0;
							break;	
					}		
					case 0xe7:
					case 0xff:
					{
						if(d_check_crossline()){robot -> runcase = CROSS_LINE_ONE;break;}
					}					
					default: 
						break;			
				}			
 
			break;
		}
		
		
		case STABLIZE_LEFT:{
			uint8_t mask = get_sensor_mask(&line_sensor);
				if( sensor_mask(mask, 0x80) != 0
//					||sensor_mask(get_sensor_mask(&line_sensor), 0x70) != 0
//					||sensor_mask(get_sensor_mask(&line_sensor), 0x60) != 0
//					||sensor_mask(get_sensor_mask(&line_sensor), 0xe0) != 0
//					||sensor_mask(get_sensor_mask(&line_sensor), MASK4_0) != 0
					){
						handle_and_speed(0, 0.6 * robot-> robot_hspeed);
						robot -> runcase  = END_CHANGE_LEFT_LANE;
					}
					break;
		}
		
		case END_CHANGE_LEFT_LANE:{   //case 64: end chuyen lan trai
				uint8_t mask = get_sensor_mask(&line_sensor);
			if (sensor_mask(mask, MASK0_4)  != 0
			 || sensor_mask(mask, 0xc3)     != 0xc3
			 || sensor_mask(mask, 0x60)     == 0x60
			 || sensor_mask(mask, 0x06)     == 0x06)
			{
				robot -> cnt3 = robot ->straight_line_time;
				robot -> runcase  = NORMAL_TRACE;	
			}
			break;
		}
		default:{	
			rgb_setcolor(&rgb, OFF);
			set_brake(&motor,1,1);
			set_speed(&motor,0,0);
			break;
		}
		
	}
}
bool d_check_crossline(){
	uint8_t mask = get_sensor_mask(&line_sensor);
	if( 	sensor_mask(mask, 0xe7)   == 0xe7    //1110 0111
	  ||  sensor_mask(mask, 0xc3)   == 0xc3    //1100 0011
		||  sensor_mask(mask, 0xff)   == 0xff    //1111 1111
	  ||  sensor_mask(mask, 0x81)   == 0x81    //1000 0001
	 )
	{
		rgb_setcolor(&rgb, RED);
//beep_long(&buzzer,100);
		return true;
	}
	else return false;
}
bool d_check_rightline(){
		if (d_check_crossline() == false) {
		uint8_t mask = get_sensor_mask(&line_sensor);
	  if(sensor_mask(mask, 0x1f) == 0x1f //0001 1111
	   ||sensor_mask(mask, 0x0f) == 0x0f //0000 1111
		 ||sensor_mask(mask, 0x19) == 0x19 //0001 1001
		 ||sensor_mask(mask, 0x1d) == 0x1d //0001 1101
		 ||sensor_mask(mask, 0x1b) == 0x1b //0001 1011
		 ||sensor_mask(mask, 0x17) == 0x17 //0001 0111
		 ||sensor_mask(mask, 0x11) == 0x11 //0001 0001
		 ||sensor_mask(mask, 0x3f) == 0x3f //0011 1111
		 ||sensor_mask(mask, 0x2f) == 0x2f //0010 1111
		 ||sensor_mask(mask, 0x37) == 0x37 //0011 0111
		 ||sensor_mask(mask, 0x3b) == 0x3b //0011 1011
		)
		{		
			rgb_setcolor(&rgb, GREEN);
			return true;
		}  
	}
	return false;
}
bool d_check_leftline(){
	if(d_check_crossline() == false) {
	if(sensor_mask(get_sensor_mask(&line_sensor), 0xf8) == 0xf8		//11111000
	 ||sensor_mask(get_sensor_mask(&line_sensor), 0xf0) == 0xf0		//11110000
	)
	{
		rgb_setcolor(&rgb, BLUE);
		return true;
	}
	}
	return false;
}
	
void handle_brake(Robot *robot, int8_t handle_angle,int8_t speed){
	
	if(robot -> brake_time >= robot -> brake_time1 ) // Stage 1 
	{
		handle(&servo, handle_angle);
		set_brake(&motor, 1, 1);
		set_speed(&motor, 0, 0);
	}	
	else if ((robot -> brake_time  < robot -> brake_time1) && (robot -> brake_time  >= robot -> brake_time2)) // State 2 
	{
		set_speed(&motor, -20, -20);
		set_brake(&motor, 1, 1);
		set_speed(&motor, 0, 0);
	}	
	else // Stage 3
	{		
		set_brake(&motor, 0, 0);		
		handle_and_speed(handle_angle, speed);
	}	
	
}
void handle_brake2(Robot *robot, int8_t _angle, int8_t _speed ){
	 /*  brake_time ---------------------------------------------------------------------------------->
				|-----------------------------------------|-------------------------------------------------|
		min_brake_time                         brake_time1                                        max_brake_time
		    |                                                                                           |
             Boost after braking                          handle(&servo, angle);
			       set_brake(&motor, 0, 0);                     set_brake(&motor, 1, 1);
				     handle_and_speed(angle, speed);	            set_speed(&motor, 0, 0);                                 
		*/    				 
	if(robot -> brake_time >= robot -> brake_time1 )
	{
		handle(&servo, _angle);
		set_brake(&motor, 1, 1);
		set_speed(&motor, 0, 0);
	//	set_speed(&motor, -5, -5);
	}		
	else if ( robot -> brake_time < robot -> brake_time1 && robot -> brake_time >= robot -> min_brake_time)
	{	
		set_brake(&motor, 0, 0);		
		handle_and_speed(_angle, _speed + 15 );
	}		
	
}


// END
