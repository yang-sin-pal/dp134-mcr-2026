//			
// Type 1 method of the trace after cross line
//switch(sensor_mask(get_sensor_mask(&line_sensor), MASK3_3)){			
//				case 0x00://000xx000
//				case 0xe7://11100111
//				case 0xc3:
//				{
//					handle_and_speed(0, CROSS_LINE_FACTOR * robot -> robot_hspeed);
//					robot -> sensor_pos = 0;
//					break;
//				}	
//				
//				case 0x04://0000 0100
//				{		
//					if (robot -> sensor_pos < -3)
//					{
//						handle_and_speed(20,0);
//						break;
//					}
//					handle_and_speed(-2, CROSS_LINE_FACTOR * robot -> robot_hspeed);
//					robot -> sensor_pos = 1;
//					break;
//				}	
//						
//				case 0x0e://0000 1110
//				{
//				 if (robot -> sensor_pos < -3)
//					{
//						handle_and_speed(25,0);
//						break;
//					}
//					handle_and_speed(-3,  CROSS_LINE_FACTOR * robot -> robot_hspeed);
//					robot -> sensor_pos = 2;
//					break;
//				}	
//				
//				case 0x06://0000 0110
//				{
// 					if (robot -> sensor_pos < -3)
//					{
//						handle_and_speed(25,0);
//						break;
//					}
//					handle_and_speed(-5,  CROSS_LINE_FACTOR * robot -> robot_hspeed);
//					robot -> sensor_pos = 3; 
//					break;
//				}
//				
//				case 0x02://0000 0010
//				case 0x07://0000 0111
//				{
//					if (robot -> sensor_pos < -3)
//					{
//						handle_and_speed(25,0);
//						break;
//					}
//					handle_and_speed(-10,  CROSS_LINE_FACTOR * robot -> robot_hspeed);
//					robot -> sensor_pos = 4; 
//					break;
//				}				
//				case 0x03://0000 0011
//				{
//					if (robot -> sensor_pos < -3)
//					{
//						handle_and_speed(25,0);
//						break;
//					}
//					handle_and_speed(-15,  CROSS_LINE_FACTOR * robot -> robot_hspeed);
//					robot -> sensor_pos = 5; 
//					break;
//				}					
//				
//				case 0x01://0000 0001					
//				{
//					if (robot -> sensor_pos < -3)
//					{
//						handle_and_speed(25,0);
//						break;
//					}
//					handle_and_speed(-15,  CROSS_LINE_FACTOR * robot -> robot_hspeed);
//					robot -> sensor_pos = 6; 
//					break;
//				}
//				
//				case 0x10://0010 0000
//				{
//					if (robot -> sensor_pos > 3)
//					{
//						handle_and_speed(-20,0);
//						break;
//					}
//					handle_and_speed(2, CROSS_LINE_FACTOR * robot -> robot_hspeed);
//					robot -> sensor_pos = -1;
//					break;
//				}
//				case 0x78://0111 0000 
//				{
//					if (robot -> sensor_pos > 3)
//					{
//						handle_and_speed(-20,0);
//						break;
//					}
//					handle_and_speed(3, CROSS_LINE_FACTOR * robot -> robot_hspeed);
//					robot -> sensor_pos = -2;
//					break;
//				}
//				case 0x60://0110 0000
//				{
//					if (robot -> sensor_pos > 3)
//					{
//						handle_and_speed(-20,0);
//						break;
//					}
//					handle_and_speed(5, CROSS_LINE_FACTOR * robot -> robot_hspeed);
//					robot -> sensor_pos = -3; 
//					break;
//				}
//				case 0x40://0100 0000
//				case 0xe0://1110 0000
//				{
//					if (robot -> sensor_pos > 3)
//					{
//						handle_and_speed(-20,0);
//						break;
//					}
//					handle_and_speed(10,  CROSS_LINE_FACTOR * robot -> robot_hspeed);
//					robot -> sensor_pos = -4; 
//					break;
//				}
//				case 0xc0://1100 0000
//				{			
//					if (robot -> sensor_pos > 3)
//					{
//						handle_and_speed(-20,0);
//						break;
//					}
//					handle_and_speed(15,  CROSS_LINE_FACTOR * robot -> robot_hspeed);  
//					robot -> sensor_pos = -5; 
//					break;
//				}
//				case 0x80://1000 0000
//				{			
//					if (robot -> sensor_pos > 3)
//					{
//						handle_and_speed(-20,0);
//						break;
//					}
//					handle_and_speed(15,  CROSS_LINE_FACTOR * robot -> robot_hspeed);
//					robot -> sensor_pos = -6; 
//					break;
//				}
//				
//			}			
//-----------------------------------------------------------------------------------------------------
			//Type1 :  Before change right lane is a straight line
//			 switch(sensor_mask(get_sensor_mask(&line_sensor), MASK3_3)){
//				case 0x00://000xx000
//				{
//					handle_and_speed(0, 0.4 * robot -> robot_hspeed);
//					robot -> sensor_pos = 0;
//					break;
//				}	
//				
//				case 0x04://0000 0100
//				{		
//					if (robot -> sensor_pos < -3)
//					{
//						handle_and_speed(20,0);
//						break;
//					}
//					handle_and_speed(-2, 0.4 * robot -> robot_hspeed);
//					robot -> sensor_pos = 1;
//					break;
//				}	
//						
//				case 0x0e://0000 1110
//				{
//				 if (robot -> sensor_pos < -3)
//					{
//						handle_and_speed(25,0);
//						break;
//					}
//					handle_and_speed(-3,  0.4 * robot -> robot_hspeed);
//					robot -> sensor_pos = 2;
//					break;
//				}	
//				
//				case 0x06://0000 0110
//				{
// 					if (robot -> sensor_pos < -3)
//					{
//						handle_and_speed(25,0);
//						break;
//					}
//					handle_and_speed(-5,  0.4 * robot -> robot_hspeed);
//					robot -> sensor_pos = 3; 
//					break;
//				}
//				
//				case 0x02://0000 0010
//				case 0x07://0000 0111
//				{
//					if (robot -> sensor_pos < -3)
//					{
//						handle_and_speed(25,0);
//						break;
//					}
//					handle_and_speed(-10,  0.2 * robot -> robot_hspeed);
//					robot -> sensor_pos = 4; 
//					break;
//				}				
//				case 0x03://0000 0011
//				{
//					if (robot -> sensor_pos < -3)
//					{
//						handle_and_speed(25,0);
//						break;
//					}
//					handle_and_speed(-15,  0.2 * robot -> robot_hspeed);
//					robot -> sensor_pos = 5; 
//					break;
//				}					
//				
//				case 0x01://0000 0001					
//				{
//					if (robot -> sensor_pos < -3)
//					{
//						handle_and_speed(25,0);
//						break;
//					}
//					handle_and_speed(-15,  0.2 * robot -> robot_hspeed);
//					robot -> sensor_pos = 6; 
//					break;
//				}
//				
//				case 0x10://0010 0000
//				{
//					if (robot -> sensor_pos > 3)
//					{
//						handle_and_speed(-20,0);
//						break;
//					}
//					handle_and_speed(2, 0.2 * robot -> robot_hspeed);
//					robot -> sensor_pos = -1;
//					break;
//				}
//				case 0x78://0111 0000 
//				{
//					if (robot -> sensor_pos > 3)
//					{
//						handle_and_speed(-20,0);
//						break;
//					}
//					handle_and_speed(3, 0.2 * robot -> robot_hspeed);
//					robot -> sensor_pos = -2;
//					break;
//				}
//				case 0x60://0110 0000
//				{
//					if (robot -> sensor_pos > 3)
//					{
//						handle_and_speed(-20,0);
//						break;
//					}
//					handle_and_speed(5,  0.2 * robot -> robot_hspeed);
//					robot -> sensor_pos = -3; 
//					break;
//				}
//				case 0x40://0100 0000
//				case 0xe0://1110 0000
//				{
//					if (robot -> sensor_pos > 3)
//					{
//						handle_and_speed(-20,0);
//						break;
//					}
//					handle_and_speed(10,  0.2 * robot -> robot_hspeed);
//					robot -> sensor_pos = -4; 
//					break;
//				}
//				case 0xc0://1100 0000
//				{			
//					if (robot -> sensor_pos > 3)
//					{
//						handle_and_speed(-20,0);
//						break;
//					}
//					handle_and_speed(15,  0.2 * robot -> robot_hspeed);
//					robot -> sensor_pos = -5; 
//					break;
//				}
//				case 0x80://1000 0000
//				{			
//					if (robot -> sensor_pos > 3)
//					{
//						handle_and_speed(-20,0);
//						break;
//					}
//					handle_and_speed(15,  0.2 * robot -> robot_hspeed);
//					robot -> sensor_pos = -6; 
//					break;
//				}	
//				case 0xe7://1110 0111
//				case 0xc3://1100 0011
//				{
//					if(d_check_crossline()){robot -> runcase = CROSS_LINE_ONE;break;}
//				}
//				default:
//					break;
//				}		
//			
//--------------------------------------------------------------------------------------------
// Change left lane saved method
//			 switch(sensor_mask(get_sensor_mask(&line_sensor), MASK4_4)){
//					case 0x18:{//0001 1000			
//						handle_and_speed(0, robot -> robot_hspeed);
//						robot -> curve_flag = 0;
//						robot -> sensor_pos = 0;
//						robot -> straight_line_flag = 1;
//						break;
//					}	
//					case 0x1c://0001 1100
//					case 0x08:{//0000 1000
//					
//						if (robot -> sensor_pos < -3)
//						{
//							handle_and_speed(-25, 0.7 * robot -> robot_hspeed);
//							break;
//						}
//						handle_and_speed(-3, 0.9 * robot -> robot_hspeed);
//						robot -> curve_flag = 0;
//						robot -> sensor_pos = 1;
//						robot -> straight_line_flag = 1;
//						break;
//					}
//					
//					case 0x0c:{//0000 1100 
//						if (robot -> sensor_pos < -3)
//						{
//							handle_and_speed(-25, 0.7 * robot -> robot_hspeed);
//							break;
//						}
//						handle_and_speed(-5,  robot -> robot_hspeed);
//						robot -> curve_flag = 0;
//						robot -> sensor_pos = 2;
//						robot -> straight_line_flag = 1;
//						break;
//					}		
//					
//					case 0x04://0000 0100
//					case 0x0e://0000 1110
//					{
//					 if (robot -> sensor_pos < -3)
//						{
//							handle_and_speed(-25, 0.7 * robot -> robot_hspeed);
//							break;
//						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{						
//							handle_brake(robot,-7, 0.9 * robot -> robot_hspeed);
//						}
//							handle_and_speed(-7, 0.9 * robot -> robot_hspeed);
//							robot -> curve_flag = 1;
//							robot -> sensor_pos = 3;
//							robot -> straight_line_flag = 0;			
//							break;
//					}	
//								
//					case 0x06://0000 0110
//					{
//						if (robot -> sensor_pos < -3)
//						{
//							handle_and_speed(-25, 0.7 * robot -> robot_hspeed);
//							break;
//						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{							
//							handle_brake(robot, -7, 0.85 * robot -> robot_hspeed);
//						}
//							handle_and_speed(-7, 0.85 * robot -> robot_hspeed);
//							robot -> curve_flag = 1;
//							robot -> sensor_pos = 4; 
//							robot -> straight_line_flag = 0;
//							break;	
//					}		
//					
//					case 0x02://0000 0010
//					case 0x07://0000 0111
//					{
//						if (robot -> sensor_pos < -3)
//						{
//							handle_and_speed(-25,0.7 * robot -> robot_hspeed);
//							break;
//						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{	
//							handle_brake(robot,-15,0.8 * robot -> robot_hspeed);
//						}
//							handle_and_speed(-15, 0.8 * robot -> robot_hspeed);
//							robot -> curve_flag = 1;
//							robot -> sensor_pos = 5; 
//							robot -> straight_line_flag = 0;
//							break;	
//					}	
//				
//					case 0x03://0000 0011
//					{
//						if (robot -> sensor_pos < -3)
//						{
//							handle_and_speed(-25, 0.7 * robot -> robot_hspeed);
//							break;
//						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{
//						 handle_brake(robot,-20, 0.75 * robot -> robot_hspeed);
//						}
//							handle_and_speed(-20, 0.75 * robot -> robot_hspeed);
//							robot -> curve_flag = 1;
//							robot -> sensor_pos = 6; 
//							robot -> straight_line_flag = 0;
//							break;		
//					}					
//					
//					case 0x01://0000 0001					
//					{
//						if (robot -> sensor_pos < -3)
//						{
//							handle_and_speed(25, 0.7 * robot -> robot_hspeed);
//							break;
//						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{
//						 handle_brake(robot,-25, 0.7 * robot -> robot_hspeed);
//						}
//							handle_and_speed(-25,  0.7 * robot -> robot_hspeed);
//							robot -> curve_flag = 1;
//							robot -> sensor_pos = 6; 
//							robot -> straight_line_flag = 0;
//							break;			
//					}				
//					
//					case 0x00://0000 0000
//					{					
//						if (robot -> sensor_pos < -4)
//						{
//							handle_and_speed(25, 0.7 * robot -> robot_hspeed);												
//							break;
//						}
//						else if (robot -> sensor_pos > 4)
//						{
//							handle_and_speed(-25, 0.7 * robot -> robot_hspeed);
//							break;
//						}
//						break;
//					}
//								
//					case 0x10://0010 0000
//					case 0x38://0011 1000
//					{
//						if (robot -> sensor_pos > 3)
//						{
//							handle_and_speed(25, 0.7 * robot -> robot_hspeed);
//							break;
//						}
//						handle_and_speed(3,  robot -> robot_hspeed);
//						robot -> curve_flag = 0;
//						robot -> sensor_pos = -1;
//						robot -> straight_line_flag = 1;
//						break;
//					}
//									
//					case 0x30://0011 0000							
//					{
//						if (robot -> sensor_pos > 3)
//						{
//							handle_and_speed(25, 0.7 * robot -> robot_hspeed);
//							break;
//						}
//						handle_and_speed(5, 0.9 * robot -> robot_hspeed);
//						robot -> curve_flag = 0;
//						robot -> sensor_pos = -2;
//						robot -> straight_line_flag = 1;
//						break;
//					}
//					
//					case 0x20://0010 0000
//					case 0x70://0111 0000 
//					{
//						if (robot -> sensor_pos > 3)
//						{
//							handle_and_speed(25, 0.7 * robot -> robot_hspeed);
//							break;
//						}
//						handle_and_speed(7, 0.9 * robot -> robot_hspeed);
//						robot -> curve_flag = 0;
//						robot -> sensor_pos = -3;
//						robot -> straight_line_flag = 1;
//						break;		
//					}
//					
//					case 0x60://0110 0000
//					{
//						if (robot -> sensor_pos > 3)
//						{
//							handle_and_speed(25, 0.7 * robot -> robot_hspeed);
//							break;
//						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{
//							handle_brake(robot, 15, 0.85 * robot -> robot_hspeed);
//						}					
//							handle_and_speed(15, 0.85 * robot -> robot_hspeed);
//							robot -> curve_flag =  1;
//							robot -> sensor_pos = -4; 
//							robot -> straight_line_flag = 0;	
//							break;								
//					}
//							
//					case 0x40://0100 0000
//					case 0xe0://1110 0000
//					{
//						if (robot -> sensor_pos > 3)
//						{
//							handle_and_speed(25, 0.7 * robot -> robot_hspeed);
//							break;
//						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{
//							handle_brake(robot, 15, 0.8 * robot -> robot_hspeed);
//						}
//							handle_and_speed(15, 0.8 * robot -> robot_hspeed);
//							robot -> curve_flag = 1;
//							robot -> sensor_pos = -5; 
//							robot -> straight_line_flag = 0;
//				   		break;
//					}
//					
//					case 0xc0://1100 0000
//					{					
//						if (robot -> sensor_pos > 3)
//						{
//							handle_and_speed(25, 0.7 * robot -> robot_hspeed);
//							break;
//						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{
//						 handle_brake(robot, 20, 0.7 * robot -> robot_hspeed);		
//						}
//						handle_and_speed(20, 0.7 * robot -> robot_hspeed);
//						robot -> curve_flag = 1;
//						robot -> sensor_pos = -6; 
//						robot -> straight_line_flag = 0;
//						break;		
//					}	
//				
//					case 0x80://1000 0000
//					{	
//						if (robot -> sensor_pos > 3)
//						{
//							handle_and_speed(25 , 0.7 * robot -> robot_hspeed);
//							break;
//						}
//						else if( robot -> brake_time  > robot -> min_brake_time)
//						{					
//							handle_brake(robot, 25, 0.7 * robot -> robot_hspeed);		
//						}

//							handle_and_speed(25, 0.7 * robot -> robot_hspeed);
//							robot -> curve_flag = 1;
//							robot -> sensor_pos = -6; 
//							robot -> straight_line_flag = 0;
//							break;	
//				}
//				case 0xe7://1110 0111
//				case 0xff://1111 1111
//				case 0xc3://1100 0011
//				{
//					if(d_check_crossline()){robot -> runcase = CROSS_LINE_ONE;break;}
//				}
//				default:
//				break;
//				}		
//-----------------------------------------------------------------------------------------------------------------------
			//Type2 :  Before change right lane is a straight line
//			 switch(sensor_mask(get_sensor_mask(&line_sensor), MASK3_3)){
//				case 0xe7://1110 0111
//				case 0xc3://1100 0011
//				{
//					robot -> runcase = CROSS_LINE_ONE;
//					break;
//				} 
//				case 0x00://000xx000
//				{
//					handle_and_speed(0, CHANGE_LANE_FACTOR* robot -> robot_hspeed);
//					break;
//				}		
//				case 0x04://0000 0100
//			  case 0x0e://0000 1110
//				case 0x06://0000 0110
//				case 0x02://0000 0010
//				case 0x07://0000 0111
//				case 0x03://0000 0011
//				case 0x01://0000 0001
//				{		

//					handle_and_speed(-5, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
//					robot -> sensor_pos = 1;
//					break;
//				}		
//				case 0x10://0010 0000
//			  case 0x78://0111 0000
//				case 0x60://0110 0000
//				case 0x40://0100 0000
//				case 0xe0://1110 0000
//				case 0xc0://1100 0000
//				case 0x80://1000 0000
//				{		
//					handle_and_speed(5, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
//					break;
//				}				
//				default:
//					break;
//				}

//---------------------------------------------------------------------------------------------------------------------------
	//Type2 :  Before change left lane is a straight line
//			 switch(sensor_mask(get_sensor_mask(&line_sensor), MASK3_3)){
//				case 0xe7://1110 0111
//				case 0xc3://1100 0011
//				{
//					robot -> runcase = CROSS_LINE_ONE;
//					break;
//				} 
//				 case 0x00://000xx000
//				{
//					handle_and_speed(0, CHANGE_LANE_FACTOR* robot -> robot_hspeed);
//					break;
//				}		
//				case 0x04://0000 0100
//			  case 0x0e://0000 1110
//				case 0x06://0000 0110
//				case 0x02://0000 0010
//				case 0x07://0000 0111
//				case 0x03://0000 0011
//				case 0x01://0000 0001
//				{		

//					handle_and_speed(-5, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
//					robot -> sensor_pos = 1;
//					break;
//				}		
//				case 0x10://0010 0000
//			  case 0x78://0111 0000
//				case 0x60://0110 0000
//				case 0x40://0100 0000
//				case 0xe0://1110 0000
//				case 0xc0://1100 0000
//				case 0x80://1000 0000
//				{		
//					handle_and_speed(5, CHANGE_LANE_FACTOR * robot -> robot_hspeed);
//					break;
//				}				
//				default:
//					break;
//				}	



