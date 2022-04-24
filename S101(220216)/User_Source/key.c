#include <p24FJ64GA306.h>
#include "adc.h"
#include "define.h"
#include "display.h"
#include "eepr.h"
#include "ex_var.h"
#include "flash.h"
#include "key.h"
#include "sub_function.h"
#include "test.h"
#include "uart.h"
#include "rtc.h"




//unsigned short up_key_count=0;
//unsigned char x=0;


/*********************************************************************************************/
// 4ms key scan routine 
void Key_Scan(void)
{
	static unsigned char key_scan_count;
	static unsigned int key_data_temp;
	volatile unsigned short key_scan_value=0, key_scan_value1, key_scan_value2, key_scan_value2_tmp=0;
	volatile unsigned char key_scan_count_temp=key_scan_count, key_status_temp=key_status;
	




	if(!tbi(key_status_temp,key_detect_f))
	{
		key_scan_value1=PORTE;
		key_scan_value1=~key_scan_value1;
		key_scan_value1&=0x00FF;									// Key Port Masking

		#if 0
		key_scan_value2=PORTB;
		key_scan_value2=~key_scan_value2;
		key_scan_value2&=0xF000;									// Key Port Masking
		#endif 

		key_scan_value2=PORTB;
		key_scan_value2=~key_scan_value2;
		if(ex_main_first_key_f==1)	key_scan_value2&=0xF000;									// Key Port Masking
		else						key_scan_value2&=0xF004;

		if( (key_scan_value2&(1<<2))==(1<<2) )
		{
			key_scan_value2 &= 0xF400;									// Key Port Masking
			key_scan_value2 |= (1<<10);
			
		}
			


		key_scan_value = key_scan_value1 | key_scan_value2;
			

		
		if(key_scan_value==0) 									//만약, 어떤 key도 눌리지 않은 경우
		{	
			if(tbi(key_status_temp,key_press_f)) 
			{
				if(++key_scan_count_temp>=KEY_RELEASE_TIME) 	// KEY_RELEASE_TIME
				{
					key_scan_count_temp=key_status_temp=0;
				}
			}
			else 
			{
				key_scan_count_temp=key_status_temp=0;
			}
		}
		else if(!tbi(key_status_temp,key_press_f)) 				
		{
	
			if(key_data_temp!=key_scan_value) 				
			{			
				key_data_temp=key_scan_value;
				key_scan_count_temp=0;
			}
			else if(++key_scan_count_temp>=KEY_DEBOUNCE_TIME) 	// KEY_DEBOUNCE_TIME
			{
				key_scan_code=key_data_temp;

				key_scan_count_temp=0;
				key_status_temp=(_key_press_f+_key_detect_f);
			}
		}

			
		
		key_scan_count=key_scan_count_temp;
		key_status=key_status_temp;

	}
}


void Key_Task(void)
{
	unsigned char 	x_key;
	unsigned char 	key_cnt=0;
	unsigned short 	i=0;

	if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
	{
		x_key=Key_Data_Read();
		switch(x_key) 
		{
			case ZERO_KEY:							//	1번 키, 영점 키 
				tone_type=SHORT_TONE_PLAY;
				if(key_task_key_f==0)
				{
					while((key_code == ZERO_KEY) && tbi(key_status,key_press_f)) 
					{
						if(++key_cnt > ep_long_key_time) 
						{	
							//tone_type=SHORT_TONE_PLAY;
							ex_weight_zero_flag=1;
							return;
						}
						DelayMs(100);
					}
					key_cnt=0;
				}
				/////////////////////////////////////////////////////////////////////////////////
				key_task_key_f=1;
				if(key_task_first_input_f==1)
				{
					key_task_first_input_f=0;
					key_task_number=x_key;
				
					break;
				}
				key_task_number = key_task_number*10+x_key;
				
				if(key_task_number > 99999) 
				{
					key_task_number = 0;
					key_task_number = 0+x_key;
				}
				key_task_number_tmp=key_task_number;
				key_task_keyless_tim=3000;
				break;
				
			case HOLD_KEY:							//	2번 키, 홀드 키 
				tone_type=SHORT_TONE_PLAY;
				if(key_task_key_f==0)
				{
					while((key_code == HOLD_KEY) && tbi(key_status,key_press_f)) 
					{
						if(++key_cnt > ep_long_key_time) 
						{	
							tone_type=SHORT_TONE_PLAY;
							ex_weight_hold_flag=1;
							return;
						}
						DelayMs(100);
					}
					key_cnt=0;
				}
				//////////////////////////////////////////////////////////////////////////////////
				key_task_key_f=1;
				if(key_task_first_input_f==1)
				{
					key_task_first_input_f=0;
					key_task_number=x_key;
					break;
				}
				key_task_number = key_task_number*10+x_key;
				
				if(key_task_number > 99999) 
				{
					key_task_number = 0;
					key_task_number = 0+x_key;
				}

				key_task_number_tmp=key_task_number;
				key_task_keyless_tim=3000;
				break;

			case ADD_KEY:							//	3번 키, 가산 키 
				tone_type=SHORT_TONE_PLAY;
				if(key_task_key_f==0)
				{
					while((key_code == ADD_KEY) && tbi(key_status,key_press_f)) 
					{
						if(++key_cnt > ep_long_key_time) 
						{	
							tone_type=SHORT_TONE_PLAY;
							ex_weight_add_flag=1;
							return;
						}
						DelayMs(100);
					}
					key_cnt=0;
				}
				//////////////////////////////////////////////////////////////////////////////////
				key_task_key_f=1;
				if(key_task_first_input_f==1)
				{
					key_task_first_input_f=0;
					key_task_number=x_key;
					break;
				}
				key_task_number = key_task_number*10+x_key;
				
				if(key_task_number > 99999) 
				{
					key_task_number = 0;
					key_task_number = 0+x_key;
				}
				key_task_number_tmp=key_task_number;
				key_task_keyless_tim=3000;
				
				break;

			case SUM_KEY:								//	4번 키, 합산 키 
				tone_type=SHORT_TONE_PLAY;
				if(key_task_key_f==0)
				{
					while((key_code == SUM_KEY) && tbi(key_status,key_press_f)) 
					{
						if(++key_cnt > ep_long_key_time && !ex_weight_sum_lamp_flag) 
						{	
							tone_type=SHORT_TONE_PLAY;
							ex_weight_sum_flag=1;
							return;
						}
						DelayMs(100);
					}
					key_cnt=0;
				}
				//////////////////////////////////////////////////////////////////////////////////
				key_task_key_f=1;
				if(key_task_first_input_f==1)
				{
					key_task_first_input_f=0;
					key_task_number=x_key;
					break;
				}
				key_task_number = key_task_number*10+x_key;
				
				if(key_task_number > 99999) 
				{
					key_task_number = 0;
					key_task_number = 0+x_key;
				}
				key_task_number_tmp=key_task_number;
				key_task_keyless_tim=3000;
				break;

			case SAVE_KEY:								//	5번 키, 저장 키 
				tone_type=SHORT_TONE_PLAY;
				if(key_task_key_f==0)
				{
					while((key_code == SAVE_KEY) && tbi(key_status,key_press_f)) 
					{
						if(++key_cnt > ep_long_key_time) 
						{	
							tone_type=SHORT_TONE_PLAY;
							ex_unit_price_input_flag=1;
							return;
						}
						DelayMs(100);
					}
					key_cnt=0;
				}
				//////////////////////////////////////////////////////////////////////////////////
				key_task_key_f=1;
				if(key_task_first_input_f==1)
				{
					key_task_first_input_f=0;
					key_task_number=x_key;
					break;
				}
				key_task_number = key_task_number*10+x_key;
				
				if(key_task_number > 99999) 
				{
					key_task_number = 0;
					key_task_number = 0+x_key;
				}
				key_task_number_tmp=key_task_number;
				key_task_keyless_tim=3000;
				
				#if 0
				x2=0;
				while((key_code == SAVE_KEY) && tbi(key_status,key_press_f)) 
				{
				
					if(++x2 > ep_long_key_time) 
					{	
						tone_type=SHORT_TONE_PLAY;	
						ex_weight_total_sum_flag=1;
						return;
					}
				
					DelayMs(100);
				}
				#endif 

				break;


			case MULTI_KEY: 								// 	6번 키, 곱셈 키 
				tone_type=SHORT_TONE_PLAY;
				if(key_task_key_f==0)
				{
					while((key_code == MULTI_KEY) && tbi(key_status,key_press_f)) 
					{
						if(++key_cnt > ep_long_key_time) 
						{	
							tone_type=SHORT_TONE_PLAY;
							ex_same_product_flag=1;
							return;
						}
						DelayMs(100);
					}
					key_cnt=0;
				}
				//////////////////////////////////////////////////////////////////////////////////
				key_task_key_f=1;
				if(key_task_first_input_f==1)
				{
					key_task_first_input_f=0;
					key_task_number=x_key;
					break;
				}
				key_task_number = key_task_number*10+x_key;
				
				if(key_task_number > 99999) 
				{
					key_task_number = 0;
					key_task_number = 0+x_key;
				}
				key_task_number_tmp=key_task_number;
				key_task_keyless_tim=3000;
				break;

			case F1_KEY:							//	7번 키, F1 키 
				tone_type=SHORT_TONE_PLAY;
			
				//////////////////////////////////////////////////////////////////////////////////
                if(!KeyTareEnable)
                {
                    key_task_key_f=1;
                    if(key_task_first_input_f==1)
                    {
                        key_task_first_input_f=0;
                        key_task_number=x_key;
                        break;
                    }
                    key_task_number = key_task_number*10+x_key;

                    if(key_task_number > 99999) 
                    {
                        key_task_number = 0;
                        key_task_number = 0+x_key;
                    }
                    key_task_number_tmp=key_task_number;
                    key_task_keyless_tim=3000;
                    break;
                }
                else
                {
                    if(key_task_key_f==0)
                    {
                        while((key_code == F1_KEY) && tbi(key_status,key_press_f)) 
                        {
                            if(++key_cnt > ep_long_key_time) 
                            {	
                                tone_type=SHORT_TONE_PLAY;
                                ex_weight_key_tare_flag=1;
                                return;
                            }

                            DelayMs(100);
                        }
                        key_cnt=0;
                    }
                    //////////////////////////////////////////////////////////////////////////////////
                    key_task_key_f=1;
                    if(key_task_first_input_f==1)
                    {
                        key_task_first_input_f=0;
                        key_task_number=x_key;

                        break;
                    }


                    key_task_number = key_task_number*10+x_key;

                    if(key_task_number > 99999) 
                    {
                        key_task_number = 0;
                        key_task_number = 0+x_key;
                    }

                    key_task_number_tmp=key_task_number;
                    key_task_keyless_tim=3000;

                    break;
                }

			case F2_KEY:								//	8번 키 , F2 키 
				tone_type=SHORT_TONE_PLAY;
				
				//////////////////////////////////////////////////////////////////////////////////
				key_task_key_f=1;
				if(key_task_first_input_f==1)
				{
					key_task_first_input_f=0;
					key_task_number=x_key;
					break;
				}
				key_task_number = key_task_number*10+x_key;
				
				if(key_task_number > 99999) 
				{
					key_task_number = 0;
					key_task_number = 0+x_key;
				}
				key_task_number_tmp=key_task_number;
				key_task_keyless_tim=3000;
				break;

			case F3_KEY:								//	9번 키 , F3 키 			
				tone_type=SHORT_TONE_PLAY;
				//tf200_send2(ex_e_value); 
				
				//////////////////////////////////////////////////////////////////////////////////
				key_task_key_f=1;
				if(key_task_first_input_f==1)
				{
					key_task_first_input_f=0;
					key_task_number=x_key;
				
					break;
				}
				
				key_task_number = key_task_number*10+x_key;
				
				if(key_task_number > 99999) 
				{
					key_task_number = 0;
					key_task_number = 0+x_key;
				}
				key_task_number_tmp=key_task_number;
				key_task_keyless_tim=3000;
				break;

			case CLR_K:								//	0번 키, 클리어 키 
				tone_type=SHORT_TONE_PLAY;
				if(key_task_key_f==0)
				{
					while((key_code == CLR_K) && tbi(key_status,key_press_f)) 
					{
						if(++key_cnt > ep_long_key_time) 
						{	
							tone_type=SHORT_TONE_PLAY;
							if(ex_weight_sum_lamp_flag==1)	
							{
								if(ex_weight_add_count==0)	break;
								else
								{
									ex_weight_sum_value -= ex_weight_add_value[ex_weight_add_count-1];
									ex_weight_add_count--;
								}
							}
							return;
						}
						DelayMs(100);
					}
					key_cnt=0;
				}
				//////////////////////////////////////////////////////////////////////////////////
				key_task_key_f=1;
				if(key_task_first_input_f==1)
				{
					key_task_first_input_f=0;
					key_task_number=x_key;
				
					break;
				}
				
				key_task_number = key_task_number*10+x_key;
				
				if(key_task_number > 99999) 
				{
					key_task_number = 0;
					key_task_number = 0+x_key;
				}

				key_task_number_tmp=key_task_number;
				key_task_keyless_tim=3000;

				if(key_task_key_f==1)
				{
					while((key_code == CLR_K) && tbi(key_status,key_press_f)) 
					{
                        unit_price_display(key_task_number);
						if(++key_cnt > (ep_long_key_time *10)) 
						{	
							tone_type=SHORT_TONE_PLAY;
							key_task_number=0;
							return;
						}
						DelayMs(10);
					}
					key_cnt=0;
				}
				
				break;

			case ENT_K:								//	ENTER 키 
				tone_type=SHORT_TONE_PLAY;

				if(key_task_key_f==0)
				{
					if(ex_weight_sum_lamp_flag==1)	
					{
						ex_weight_sum_lamp_flag=0;
						
						ex_weight_add_count=0;

						for(i=0; i<500; i++)	
						{
							ex_weight_add_value[i]=0;
						}
						////////////////////////////////////////////////////////////////////////////////
						if( ex_serial_no>10 )	
						{
							ex_serial_no=0;
							
							Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no), (unsigned long)0);		//	1
							Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+4), (unsigned long)0);		//	2
							Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+8), (unsigned long)0);		//	3
							Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+12), (unsigned long)0);		//	4
							Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+16), (unsigned long)0);		//	5
							Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+20), (unsigned long)0);		//	6
							Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+24), (unsigned long)0);		//	7
							Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+28), (unsigned long)0);		//	8
							Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+32), (unsigned long)0);		//	9
							Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+36), (unsigned long)0);		//	10
							break;
						}
						
						Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no*4), (unsigned long)ex_weight_sum_value);
					}
					else
					{
						if(!ep_enter_key_set)
                        {
                            if( (ep_backlight==0) )
                            {
                                if(ex_back_light_flag)	{	BackLight_Off;	 ex_back_light_flag=0;		}
                                else					{	BackLight_On;	 ex_back_light_flag=1;		}
                            }
                        }
                        
						else    print_flag=1;
					}
				}
				else
				{	
					short_call_input();
				}
				
				break;

			case POWER_K:							//	파워, 제로, 임시단가 키
				tone_type=SHORT_TONE_PLAY;
				while((key_code == POWER_K) && tbi(key_status,key_press_f)) 
				{
					if(++key_cnt > ep_long_key_time) 
					{	
						tone_type=SHORT_TONE_PLAY;
						poff_sig=1;
						return;
					}
					DelayMs(100);
				}
				key_cnt=0;

				if(key_task_key_f==1)	short_u_p_input();
				else					ex_weight_tare_flag=1;
				break;
                
            case CAL_K:
                tone_type=SHORT_TONE_PLAY;
                DelayMs(100);
                Single_Calibration_Mode(); 
                break;

			default:
				break;
		}
	}
}

/*********************************************************************************************/
// normal function key data read
unsigned char Key_Data_Read(void)
{
	if(tbi(key_status, key_detect_f))
	{
		cbi(key_status, key_detect_f);
		
		switch(key_scan_code)
		{
			case 0x0001:	key_code=ZERO_KEY;	break;
			case 0x0002:	key_code=HOLD_KEY;	break;
			case 0x0004:	key_code=ADD_KEY;	break;
			case 0x0008:	key_code=SUM_KEY;	break;
			case 0x0010:	key_code=SAVE_KEY;	break;
			case 0x0020:	key_code=MULTI_KEY;	break;
			case 0x0040:	key_code=F1_KEY;	break;
			case 0x0080:	key_code=F2_KEY;	break;
			case 0x0400: 	key_code=POWER_K;	break;
			case 0x1000:	key_code=F3_KEY;	break;
			case 0x2000:	key_code=CLR_K;		break;
			case 0x4000:	key_code=ENT_K;		break;
			case 0x8000:	key_code=CAL_K;		break;
			default:							break;
		}
	}
	else if(tbi(ir_key_status, ir_key_detect_f))
	{
		cbi(ir_key_status,ir_key_detect_f);
		key_code=ir_key_scan_code;
	}

	

	//lcd_display_once_time=0;
	return(key_code);
}


unsigned char Input_Number_Weight_ONE(unsigned char init_value)
{
	unsigned char 	i=0;
	unsigned char 	x_key;
	unsigned char 	number=0;
	unsigned char	key_cnt=0;

	number=init_value;

	weight_display(number);


	while(1)
	{
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			
			switch(x_key) 
			{
				case ZERO_KEY:/* 1 */
				case HOLD_KEY:/* 2 */
				case ADD_KEY:/* 3 */
				case SUM_KEY:/* 4 */
				case SAVE_KEY:/* 5 */	
                case MULTI_KEY:/* 5 */
				case F1_KEY:/* 7 */
				case F2_KEY:/* 8 */	
				case F3_KEY:/* 9 */	
				case CLR_K:/* 0 */	
					tone_type=0x01;

					number=x_key;


					if(number > 9) number = 0;


					break;

					
				case ENT_K:
					tone_type=0x01;
					return(number);

					break;

				case POWER_K:							//	파워, 제로, 임시단가 키
					tone_type=SHORT_TONE_PLAY;
					while((key_code == POWER_K) && tbi(key_status,key_press_f)) 
					{
						if(++key_cnt > ep_long_key_time) 
						{	
							tone_type=SHORT_TONE_PLAY;
							poff_sig=1;
							return;
						}
						DelayMs(100);
					}
					key_cnt=0;
					break;
					
				default:	
					break;
				
			}
		}

		weight_display(number);
	}

	return;
}

unsigned char two_byte(unsigned char init_value)
{
	unsigned char 	x_key;
	unsigned char	key_cnt=0;

 	unsigned char first_value; 
 	unsigned char second_value; 
	unsigned char  first_input_flag=1;

	first_value =(init_value/10)%10;
	second_value=(init_value/1)%10;


	number_two_input_step=0;
	number_two_input_step_timer=0;


  	while(1)
  	{
		switch(number_two_input_step)
		{
			case 0:
				if(number_two_input_step_timer == 0)
				{
					first_value =(init_value/10)%10;
					second_value=(init_value/1)%10;

					lcd_want_display(WEIGHT, 4, first_value);
			  		lcd_want_display(WEIGHT, 5, second_value);
					

					number_two_input_step=1;
					number_two_input_step_timer=250;
				}	
				break;

			case 1:
				if(number_two_input_step_timer == 0)
				{
					String_Display(WEIGHT, "     ");
					
					number_two_input_step=0;
					number_two_input_step_timer=150;
				}	
				break;

			default:
				break;
		}

		
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			switch(x_key) 
			{
				case ZERO_KEY:/* 1 */
				case HOLD_KEY:/* 2 */
				case ADD_KEY:/* 3 */
				case SUM_KEY:/* 4 */
				case SAVE_KEY:/* 5 */
				case MULTI_KEY:/* 6 */	
				case F1_KEY:/* 7 */
				case F2_KEY:/* 8 */	
				case F3_KEY:/* 9 */	
				case CLR_K:/* 0 */	
					tone_type=0x01;

					if(first_input_flag==1)
					{
						first_input_flag=0;
						init_value=x_key;

						break;
					}

					init_value = ((init_value*10)+x_key);


					if(init_value>99)
					{
						init_value=x_key;
					}

					break;
					
				case ENT_K:
					tone_type=LONG_TONE_PLAY;									

					first_value =(init_value/10)%10;
					second_value=(init_value/1)%10;

					return(init_value);

					break;

				case POWER_K:							//	파워, 제로, 임시단가 키
					tone_type=SHORT_TONE_PLAY;
					while((key_code == POWER_K) && tbi(key_status,key_press_f)) 
					{
						if(++key_cnt > ep_long_key_time) 
						{	
							tone_type=SHORT_TONE_PLAY;
							poff_sig=1;
							return;
						}
						DelayMs(100);
					}
					key_cnt=0;
					break;
					

				default:
					break;
			}
		}
  	}
	
  	return(0);
}



//unsigned long Input_Number_Long(unsigned long init_value, unsigned char type)
//{
//	unsigned char 	i=0;
//	unsigned char 	x_key;
//	unsigned char	key_cnt=0;
//	unsigned long 	number=0;
//
//	number=init_value;
//
//	weight_display(number);
//
//
//	while(1)
//	{
//		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
//		{
//			x_key=Key_Data_Read();
//			if(type == CAPA_INPUT)
//            {
//                switch(x_key) 
//                {
//                    case ZERO_KEY:/* 1 */
//                        number=15;  //15kg
//                        tone_type=0x01;
//                        break;
//                    case HOLD_KEY:/* 2 */
//                        number=30;  //30kg
//                        tone_type=0x01;
//                        break;
//                    case ADD_KEY:/* 3 */
//                        number=60;  //60kg
//                        tone_type=0x01;
//                        break;
//                    case   SUM_KEY:/* 4 */
//                        number=150;  //150kg
//                        tone_type=0x01;
//                        break;
//                    case SAVE_KEY:/* 5 */
//                    case MULTI_KEY:/* 6 */	
//                    case F1_KEY:/* 7 */
//                    case F2_KEY:/* 8 */	
//                    case F3_KEY:/* 9 */	
//                    case CLR_K:/* 0 */	
//                        break;
//
//                    case ENT_K:
//                        tone_type=0x01;
//                        return(number);
//                        break;
//
//                    case POWER_K:							//	파워, 제로, 임시단가 키
//                        tone_type=SHORT_TONE_PLAY;
//                        while((key_code == POWER_K) && tbi(key_status,key_press_f)) 
//                        {
//                            if(++key_cnt > ep_long_key_time) 
//                            {	
//                                tone_type=SHORT_TONE_PLAY;
//                                poff_sig=1;
//                                return;
//                            }
//                            DelayMs(100);
//                        }
//                        key_cnt=0;
//                        break;
//
//                    default:	
//                        break;
//                }
//                if(number==15)
//                {
//                    ep_minimum_division = POINT_DATA[4];
//                    ep_e_resolution = 7500;  // 15/0.002
//                    ep_minimum_division_dual = POINT_DATA[5];
//                    ep_e_resolution_dual = 3000;  // 15/0.005
//                    ep_DualRangeBoundary=6000;
//                }
//                else if(number==30)
//                {
//                    ep_minimum_division = POINT_DATA[5];
//                    ep_e_resolution = 6000; // 30/0.005
//                    ep_minimum_division_dual = POINT_DATA[6];
//                    ep_e_resolution_dual = 3000; //  30/0.01
//                    ep_DualRangeBoundary=15000;
//                }
//                else if(number==60)
//                {
//                    ep_minimum_division = POINT_DATA[6];
//                    ep_e_resolution = 6000; // 60/0.01
//                    ep_minimum_division_dual = POINT_DATA[7];
//                    ep_e_resolution_dual = 3000;  // 60/0.02
//                    ep_DualRangeBoundary=3000;
//                }
//                else if(number==150)
//                {
//                    ep_minimum_division = POINT_DATA[7];
//                    ep_e_resolution = 7500; // 150/0.02
//                    ep_minimum_division_dual = POINT_DATA[8];
//                    ep_e_resolution_dual = 3000;  // 150/0.05
//                    ep_DualRangeBoundary=6000;
//                }
////                if ((ep_minimum_division & 0xF8) == 0x08) ep_e_resolution = 3000 / 10; 
////                else if((ep_minimum_division & 0xF8) == 0x10) ep_e_resolution = 3000 / 100;
////                else if((ep_minimum_division & 0xF8) == 0x20) ep_e_resolution = 3000 * 10; 
////                else if((ep_minimum_division & 0xF8) == 0x40) ep_e_resolution = 3000 * 100;
////                else if((ep_minimum_division & 0xF8) == 0x60) ep_e_resolution = 3000 * 1000;
////                else if((ep_minimum_division & 0xF8) == 0x80) ep_e_resolution = 3000 * 10000;
//            }
//            else if(type == LOAD_INPUT)
//            {
//                switch(x_key) 
//                {
//                    case ZERO_KEY:/* 1 */
//                    case HOLD_KEY:/* 2 */
//                    case ADD_KEY:/* 3 */
//                    case SUM_KEY:/* 4 */
//                    case SAVE_KEY:/* 5 */
//                    case MULTI_KEY:/* 6 */	
//                    case F1_KEY:/* 7 */
//                    case F2_KEY:/* 8 */	
//                    case F3_KEY:/* 9 */	
//                    case CLR_K:/* 0 */	
//    					tone_type=0x01;
//    					number = number*10+x_key;
//    					if(number > 99999) number = 0+x_key;
//                        break;
//
//                    case ENT_K:
//                        tone_type=0x01;
//                        return(number);
//                        break;
//
//                    case POWER_K:							//	파워, 제로, 임시단가 키
//                        tone_type=SHORT_TONE_PLAY;
//                        while((key_code == POWER_K) && tbi(key_status,key_press_f)) 
//                        {
//                            if(++key_cnt > ep_long_key_time) 
//                            {	
//                                tone_type=SHORT_TONE_PLAY;
//                                poff_sig=1;
//                                return;
//                            }
//                            DelayMs(100);
//                        }
//                        key_cnt=0;
//                        break;
//
//                    default:	
//                        break;
//                }
//            }
//            weight_display(number);
//		}
//	}
//
//	return;
//}



unsigned char LCD3_1Byte_test_mode_Input_Blink(unsigned char input_num, unsigned char stx_num, unsigned char etx_num)
{
	unsigned char 	x_key=0;
	unsigned char	key_cnt=0;
	unsigned char 	loop_out=0;
	unsigned char	num_1=0;
	unsigned char	output_num=0;

	unsigned long 	bound_count=0;




	
	num_1	 = input_num%10;
	


	while(1)
	{
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();

			switch(x_key) 
			{
				case CLR_K:		// 0
				case ZERO_KEY:	// 1
				case HOLD_KEY:	// 2
				case ADD_KEY:	// 3
				case SUM_KEY:	// 4
				case SAVE_KEY:	// 5
				case MULTI_KEY:	// 6
				case F1_KEY:	// 7
				case F2_KEY:	// 8
				case F3_KEY:	// 9
					tone_type=SHORT_TONE_PLAY;									
					
					num_1 = x_key;

					if( (num_1>=stx_num)&&(num_1<=etx_num) )	num_1=x_key; 
					else										num_1=input_num;					
					
					break;

				case ENT_K:
					tone_type=LONG_TONE_PLAY;									
					String_Display(WEIGHT, "     "); 
					loop_out=1; 

					break;

				case POWER_K:							//	파워, 제로, 임시단가 키
					tone_type=SHORT_TONE_PLAY;
					while((key_code == POWER_K) && tbi(key_status,key_press_f)) 
					{
						if(++key_cnt > ep_long_key_time) 
						{	
							tone_type=SHORT_TONE_PLAY;
							poff_sig=1;
							return;
						}
						DelayMs(100);
					}
					key_cnt=0;
					break;	

				default:
					break;
			}
		}



		output_num = num_1;



		if(bound_count<=1000)
		{
			lcd_want_display(WEIGHT, 1, 11);
			lcd_want_display(WEIGHT, 2, 11);
			lcd_want_display(WEIGHT, 3, 11);
			lcd_want_display(WEIGHT, 4, 11);
			lcd_want_display(WEIGHT, 5, output_num%10);

			bound_count++;
		}
		else if(bound_count<=5000)
		{
			lcd_want_display(WEIGHT, 5,11);

			bound_count++;
		}
		else 
		{
			bound_count=0; 
		}

		if(loop_out)
		{

			return(output_num);
			break;
		}
	} 
}


unsigned char LCD3_2Byte_test_mode_Input_Blink(unsigned char input_num)
{
	unsigned char	x_key=0;
	unsigned char	key_cnt=0;
	unsigned char	bound=1;
	unsigned char	loop_out=0;
	unsigned char	num_10=0;
	unsigned char	num_1=0;

	unsigned long	bound_count=0;




	
	num_10	 = input_num/10;
	num_1	 = input_num%10;


	num_10=num_10*10;
	
	


	while(1)
	{
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();

			switch(x_key) 
			{
				case CLR_K: 	// 0
				case ZERO_KEY:	// 1
				case HOLD_KEY:	// 2
				case ADD_KEY:	// 3
				case SUM_KEY: 	// 4
				case SAVE_KEY: 	// 5
				case MULTI_KEY:	// 6
				case F1_KEY:	// 7
				case F2_KEY:	// 8
				case F3_KEY:	// 9
					tone_type=SHORT_TONE_PLAY;									
					if(bound==0) 
					{ 
						num_10 = 10*x_key; 

						//if(num_10+num_1>20) num_10=0;
					}
					
					if(bound==1) 
					{ 
						num_1 = x_key;
						#if 0
						if(num_10>20) 
						{ 
							num_1=0; 
						} 
						else if(num_1>9)
						{
							num_1=0; 
						}
						#endif 
					}
					
					bound++; 
					if( (bound>1) ) 
					{
						bound=0;
					}
					
					break;

				case ENT_K:
					tone_type=LONG_TONE_PLAY;									
					String_Display(WEIGHT, "     "); 
					bound=0; 
					loop_out=1; 

					break;

				case POWER_K:							//	파워, 제로, 임시단가 키
					tone_type=SHORT_TONE_PLAY;
					while((key_code == POWER_K) && tbi(key_status,key_press_f)) 
					{
						if(++key_cnt > ep_long_key_time) 
						{	
							tone_type=SHORT_TONE_PLAY;
							poff_sig=1;
							return;
						}
						DelayMs(100);
					}
					key_cnt=0;
					break;	
				
				default:
					break;
			}
		}



		input_num = num_10+num_1;



		if(bound_count<=1000)
		{
			lcd_want_display(WEIGHT, 1, 11);
			lcd_want_display(WEIGHT, 2, 11);
			lcd_want_display(WEIGHT, 3, 11);
			lcd_want_display(WEIGHT, 4, input_num/10);
			lcd_want_display(WEIGHT, 5, input_num%10);

			bound_count++;
		}
		else if(bound_count<=5000)
		{
			if(bound==0) lcd_want_display(WEIGHT, 4,11);
			if(bound==1) lcd_want_display(WEIGHT, 5,11);

			bound_count++;
		}
		else 
		{
			bound_count=0; 
		}

		if(loop_out)
		{

			return(input_num);
			break;
		}
	} 
}






unsigned char LCD2_2Byte_Input_Blink(unsigned char input_num, unsigned char mode)
{
	unsigned char 	x_key=0;
	unsigned char	key_cnt=0;
	unsigned char 	bound=1;
	unsigned char 	loop_out=0;
	unsigned char	num_10=0;
	unsigned char	num_1=0;
	
	unsigned long 	bound_count=0;




	
	num_10   = (input_num/10)*10;
	num_1	 = input_num%10;
	


	while(1)
	{
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();

			switch(x_key) 
			{
				case CLR_K:		// 0
				case ZERO_KEY:	// 1
				case HOLD_KEY:	// 2
				case ADD_KEY:	// 3
				case SUM_KEY:		// 4
				case SAVE_KEY:		// 5
				case MULTI_KEY:		// 6
				case F1_KEY:	// 7
				case F2_KEY:	// 8
				case F3_KEY:	// 9
					tone_type=SHORT_TONE_PLAY;									
					if(bound==0) 
					{ 
						num_10 = 10*x_key; 

						if(num_10 > 23) num_10=0;
					}
					
					if(bound==1) 
					{ 
						num_1 = x_key;

						if(num_10 > 23) 
						{ 
							num_1=0; 
						} 
						else if(num_1>9)
						{
							num_1=0; 
						}
					}
					
					bound++; 
					if( (bound>1) )	
					{
						bound=0;
					}
					
					break;

				case ENT_K:
					tone_type=LONG_TONE_PLAY;									
					String_Display(UNIT_PRICE, "     "); 
					bound=0; 
					loop_out=1; 

					break;

				case POWER_K:							//	파워  키 
					tone_type=SHORT_TONE_PLAY;
					while((key_code == POWER_K) && tbi(key_status,key_press_f)) 
					{
						if(++key_cnt > ep_long_key_time) 
						{
							tone_type=SHORT_TONE_PLAY;
							poff_sig=1;
							return;
						}
						DelayMs(100);
					}
					key_cnt=0;
					break;
					

				default:
					break;
			}
		}



		input_num = num_10+num_1;


        if(!mode)
        {
            if(bound_count<=1000)
            {
                lcd_want_display(UNIT_PRICE, 1, 11);
                //lcd_want_display(UNIT_PRICE, 2, 'F');
                lcd_want_display(UNIT_PRICE, 3, input_num/10);
                lcd_want_display(UNIT_PRICE, 4, input_num%10);
                lcd_want_display(UNIT_PRICE, 5, 11);

                bound_count++;
            }
            else if(bound_count<=5000)
            {
                if(bound==0) lcd_want_display(UNIT_PRICE, 3,11);
                if(bound==1) lcd_want_display(UNIT_PRICE, 4,11);

                bound_count++;
            }
            else 
            {
                bound_count=0; 
            }
        }
        else if(mode == 1)
        {
            if(bound_count<=1000)
            {
                //lcd_want_display(UNIT_PRICE, 1, 11);
                //lcd_want_display(UNIT_PRICE, 2, 'F');
                lcd_want_display(UNIT_PRICE, 4, input_num/10);
                lcd_want_display(UNIT_PRICE, 5, input_num%10);
                //lcd_want_display(UNIT_PRICE, 5, 11);

                bound_count++;
            }
            else if(bound_count<=5000)
            {
                if(bound==0) lcd_want_display(UNIT_PRICE, 4,11);
                if(bound==1) lcd_want_display(UNIT_PRICE, 5,11);

                bound_count++;
            }
            else 
            {
                bound_count=0; 
            }
        }
		if(loop_out)
		{

			return(input_num);
			break;
		}
	} 
}






unsigned short Input_Plu_Number_Char(unsigned char init_value)
{
	unsigned char 	i=0;
	unsigned char 	first_input_flag=1;
	unsigned char 	x_key;
	unsigned char 	number=0;
	unsigned char	key_cnt=0;

	number=init_value;

	plu_display(number);

	number_two_input_step=0;
	number_two_input_step_timer=0;


	while(1)
	{
		switch(number_two_input_step)
		{
			case 0:
				if(number_two_input_step_timer == 0)
				{
					plu_display(number);
					
					if(init_value>=10)
					{
					}
					else
					{
					}

					number_two_input_step=1;
					number_two_input_step_timer=250;
				}	
				break;

			case 1:
				if(number_two_input_step_timer == 0)
				{
					String_Display(PLU, "  ");
					
					if(init_value>=10)
					{
					}
					else
					{
					}

					number_two_input_step=0;
					number_two_input_step_timer=150;
				}	
				break;

			default:
				break;
		}

		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			
			switch(x_key) 
			{
				case ZERO_KEY:/* 1 */
				case HOLD_KEY:/* 2 */
				case ADD_KEY:/* 3 */
				case SUM_KEY:/* 4 */
				case SAVE_KEY:/* 5 */
				case MULTI_KEY:/* 6 */	
				case F1_KEY:/* 7 */
				case F2_KEY:/* 8 */	
				case F3_KEY:/* 9 */	
				case CLR_K:/* 0 */	
					tone_type=0x01;

					if(first_input_flag==1)
					{
						if(test_lcd_mode_f==1)
						{
							if( x_key<7 )	number=x_key;
							else			asm("Nop");
							
							first_input_flag=0;
							break;
						}
						
						first_input_flag=0;
						number=x_key;

						break;
					}


					if(test_lcd_mode_f==1)
					{
						if( x_key<7 )	number=x_key;
						else			asm("Nop");
					}
					else
					{
						if(number>9)
						{
							number=(number%10);
						}
						
						number = ((number*10)+x_key);

						if(number>99)
						{
							number=x_key;
						}
					}


					break;

					
				case ENT_K:
					tone_type=0x01;
					return(number);
					
					break;

				case POWER_K:							//	파워  키 
					tone_type=SHORT_TONE_PLAY;
					while((key_code == POWER_K) && tbi(key_status,key_press_f)) 
					{
						if(++key_cnt > ep_long_key_time) 
						{	
							tone_type=SHORT_TONE_PLAY;
							poff_sig=1;
							return;
						}
						DelayMs(100);
					}
					key_cnt=0;
					break;

				default:	
					break;
				
			}
		}
	}
}


unsigned long Input_U_P_Number_Long(unsigned long init_value)
{
	unsigned char 	i=0;
	unsigned char 	x_key;
	unsigned char	key_cnt=0;
	unsigned long 	number=0;
	unsigned char  	first_input_flag=1;

	number=init_value;

	unit_price_display(number);

	number_two_input_step=0;
	number_two_input_step_timer=0;


	while(1)
	{
		switch(number_two_input_step)		
		{
			case 0:
				if(number_two_input_step_timer == 0)
				{
					unit_price_display(number);		
					
					if(init_value>=10)	// 두 자리 수 일 경우 
					{
						//plu_display(number);	

						//lcd_want_display(3, init_value/10);
						//lcd_want_display(4, init_value%10);
					}	
					else				// 한 자리 수 일 경우 
					{
						//lcd_want_display(4, init_value);
					}

					number_two_input_step=1;
					number_two_input_step_timer=250;
				}	
				break;

			case 1:
				if(number_two_input_step_timer == 0)
				{
					String_Display(UNIT_PRICE, "     ");
					
					if(init_value>=10)	// 두 자리 수 일 경우 
					{
						//String_Display(PLU, "  ");

						//lcd_want_display(3, ' ');
						//lcd_want_display(4, ' ');
					}
					else				// 한 자리 수 일 경우 
					{
						//lcd_want_display(4, ' ');
					}

					number_two_input_step=0;
					number_two_input_step_timer=150;
				}	
				break;

			default:
				break;
		}
		
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			
			switch(x_key) 
			{
				case ZERO_KEY:/* 1 */
				case HOLD_KEY:/* 2 */
				case ADD_KEY:/* 3 */
				case SUM_KEY:/* 4 */
				case SAVE_KEY:/* 5 */
				case MULTI_KEY:/* 6 */	
				case F1_KEY:/* 7 */
				case F2_KEY:/* 8 */	
				case F3_KEY:/* 9 */	
				case CLR_K:/* 0 */	
					tone_type=0x01;


					
					if(first_input_flag==1)
					{
						first_input_flag=0;
						number=x_key;

						break;
					}

					
					number = number*10+x_key;

					if(number > 99999) 
					{
						number = 0;
						number = 0+x_key;
					}

					break;

					
				case ENT_K:
					tone_type=0x01;
					return(number);

					break;

				case POWER_K:							//	파워, 제로, 임시단가 키
					tone_type=SHORT_TONE_PLAY;
					while((key_code == POWER_K) && tbi(key_status,key_press_f)) 
					{
						if(++key_cnt > ep_long_key_time) 
						{	
							tone_type=SHORT_TONE_PLAY;
							poff_sig=1;
							return;
						}
						DelayMs(100);
					}
					key_cnt=0;
					break;	
					
				default:	
					break;
				
			}
		}

		//unit_price_display(number);
	}

	return;
}

void Func_Check(void)
{
	unsigned char 	x_key=0;
    unsigned char 	check=0;
	
    //if(x_key == ZERO_KEY)
	if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
	{
		x_key=Key_Data_Read();
		ex_main_first_key_f=0;
		switch(x_key) 
		{
			case ZERO_KEY:	// TEST_MODE			//	1번 키 
				tone_type=LONG_TONE_PLAY;
				Test_Mode();

				break;

			case HOLD_KEY:	// SET_MODE			//	2번 키 
				tone_type=LONG_TONE_PLAY;
				Setting_Mode(); 
                eeprom_read();
				break;

			
			case ADD_KEY:	// CAL_MODE			//	3번 키 
                tone_type=LONG_TONE_PLAY;
                DelayMs(100);
                tone_type=LONG_TONE_PLAY;
                DelayMs(100);
                check = Pass_Word_Input(1);
                if(!check) break;
            case CAL_K:    
				Single_Calibration_Mode(); 
				break;

			case SUM_KEY:								//	4번 키 
				tone_type=LONG_TONE_PLAY;				
				break;

			case SAVE_KEY:								//	5번 키 
				tone_type=LONG_TONE_PLAY;
				#if 0
				String_Display(PAY, " CLEAR");
				Atflash_PluData_Push( PAGE_TEST, (unsigned char)200, (unsigned long)0);
				ex_serial_no=(unsigned short)Atflash_PluData_Pop( PAGE_TEST, 200 );
				
				Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no), (unsigned long)0);		//	1
				Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+4), (unsigned long)0);		//	2
				Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+8), (unsigned long)0);		//	3
				Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+12), (unsigned long)0); 	//	4
				Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+16), (unsigned long)0); 	//	5
				Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+20), (unsigned long)0); 	//	6
				Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+24), (unsigned long)0); 	//	7
				Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+28), (unsigned long)0); 	//	8
				Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+32), (unsigned long)0); 	//	9
				Atflash_PluData_Push( (unsigned short)(3), (unsigned char)(ex_serial_no+36), (unsigned long)0); 	//	10
				#endif 
				
				break;

			case MULTI_KEY:								//	6번 키 
				tone_type=LONG_TONE_PLAY;
				HiddinSet_Mode(); 
                eeprom_read();
				
				break;

			case F1_KEY:							//	7번 키 
				tone_type=LONG_TONE_PLAY;
				
				break;

			case F2_KEY:								//	8번 키 
				tone_type=LONG_TONE_PLAY;
				break;
				
			case F3_KEY:								//	9번 키 
				tone_type=LONG_TONE_PLAY;
				netid_mode(); 
			
				break;


			case CLR_K: 		//	RTC_SET_MODE	//	0번 키 		
				tone_type=LONG_TONE_PLAY;
				rtc_set();

				break;

			case ENT_K: 							//	enter key			
				tone_type=LONG_TONE_PLAY;

				break;




			default: 
				break;
		}
	}
}

unsigned short three_byte(unsigned short init_value)
{
	unsigned char 	x_key;
	unsigned char	key_cnt=0;

 	unsigned char 	first_value; 
 	unsigned char 	second_value; 
 	unsigned char 	third_value; 
	unsigned char  	first_input_flag=1;

	first_value=(init_value/100);
	second_value=(init_value%100)/10;
	third_value=(init_value%100)%10;

	number_two_input_step=0;
	number_two_input_step_timer=0;


  	while(1)
  	{
		switch(number_two_input_step)
		{
			case 0:
				if(number_two_input_step_timer == 0)
				{
					first_value=(init_value/100);
					second_value=(init_value%100)/10;
					third_value=(init_value%100)%10;

					lcd_want_display(WEIGHT, 3, first_value);
			  		lcd_want_display(WEIGHT, 4, second_value);
			  		lcd_want_display(WEIGHT, 5, third_value);
					

					number_two_input_step=1;
					number_two_input_step_timer=250;
				}	
				break;

			case 1:
				if(number_two_input_step_timer == 0)
				{
					String_Display(WEIGHT, "PC   ");
					
					number_two_input_step=0;
					number_two_input_step_timer=150;
				}	
				break;

			default:
				break;
		}

		
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			switch(x_key) 
			{
				case ZERO_KEY:/* 1 */
				case HOLD_KEY:/* 2 */
				case ADD_KEY:/* 3 */
				case SUM_KEY:/* 4 */
				case SAVE_KEY:/* 5 */
				case MULTI_KEY:/* 6 */	
				case F1_KEY:/* 7 */
				case F2_KEY:/* 8 */	
				case F3_KEY:/* 9 */	
				case CLR_K:/* 0 */	
					tone_type=0x01;

					if(first_input_flag==1)
					{
						first_input_flag=0;
						init_value=x_key;

						break;
					}

					init_value = ((init_value*10)+x_key);


					if(init_value>999)
					{
						init_value=x_key;
					}

					break;
					
				case ENT_K:
					tone_type=LONG_TONE_PLAY;									

					first_value=(init_value/100);
					second_value=(init_value%100)/10;
					third_value=(init_value%100)%10;

					return(init_value);

					break;

				case POWER_K:							//	파워, 제로, 임시단가 키
					tone_type=SHORT_TONE_PLAY;
					while((key_code == POWER_K) && tbi(key_status,key_press_f)) 
					{
						if(++key_cnt > ep_long_key_time) 
						{	
							tone_type=SHORT_TONE_PLAY;
							poff_sig=1;
							return;
						}
						DelayMs(100);
					}
					key_cnt=0;
					break;	
					
				default:
					break;
			}
		}
  	}
	
  	return(0);
}

unsigned long five_byte(unsigned long init_value)
{
	unsigned char 	x_key;
	unsigned char	key_cnt=0;
	/*
    unsigned char 	first_value; 
 	unsigned char 	second_value; 
 	unsigned char 	third_value; 
	unsigned char 	fourth_value;
	unsigned char 	fifth_value;
    */
	unsigned char  	first_input_flag=1;


    /*
	first_value =(init_value/10000)%10;
	second_value=(init_value/1000)%10;
	third_value =(init_value/100)%10;
	fourth_value=(init_value/10)%10;
	fifth_value =(init_value/1)%10;
    */

	
	number_two_input_step=0;
	number_two_input_step_timer=0;


  	while(1)
  	{
		switch(number_two_input_step)
		{
			case 0:
				if(number_two_input_step_timer == 0)
				{
					/*
                    first_value =(init_value/10000)%10;
					second_value=(init_value/1000)%10;
					third_value =(init_value/100)%10;
					fourth_value=(init_value/10)%10;
					fifth_value =(init_value/1)%10;

					lcd_want_display(WEIGHT, 1, first_value);
			  		lcd_want_display(WEIGHT, 2, second_value);
			  		lcd_want_display(WEIGHT, 3, third_value);
					lcd_want_display(WEIGHT, 4, fourth_value);
			  		lcd_want_display(WEIGHT, 5, fifth_value);
                    */
                    weight_display(init_value);
					

					number_two_input_step=1;
					number_two_input_step_timer=250;
				}	
				break;

			case 1:
				if(number_two_input_step_timer == 0)
				{
					String_Display(WEIGHT, "     ");
					
					number_two_input_step=0;
					number_two_input_step_timer=150;
				}	
				break;

			default:
				break;
		}

		
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			switch(x_key) 
			{
				case ZERO_KEY:/* 1 */
				case HOLD_KEY:/* 2 */
				case ADD_KEY:/* 3 */
				case SUM_KEY:/* 4 */
				case SAVE_KEY:/* 5 */
				case MULTI_KEY:/* 6 */	
				case F1_KEY:/* 7 */
				case F2_KEY:/* 8 */	
				case F3_KEY:/* 9 */	
				case CLR_K:/* 0 */	
					tone_type=0x01;

					if(first_input_flag==1)
					{
						first_input_flag=0;
						init_value=x_key;

						break;
					}

					init_value = ((init_value*10)+x_key);


					if(init_value>99999)
					{
						init_value=x_key;
					}

					break;
					
				case ENT_K:
					tone_type=LONG_TONE_PLAY;									
                    /*
					first_value =(init_value/10000);
					second_value=(init_value%10000)/1000;
					third_value =((init_value%10000)/1000)%100;
					fourth_value=(((init_value%10000)/1000)%100)/10;
					fifth_value =(((init_value%10000)/1000)%100)%10;
                     */
					return(init_value);

					break;

				case POWER_K:							//	파워, 제로, 임시단가 키
					tone_type=SHORT_TONE_PLAY;
					while((key_code == POWER_K) && tbi(key_status,key_press_f)) 
					{
						if(++key_cnt > ep_long_key_time) 
						{	
							tone_type=SHORT_TONE_PLAY;
							poff_sig=1;
							return;
						}
						DelayMs(100);
					}
					key_cnt=0;
					break;	
					
				default:
					break;
			}
		}
  	}
	
  	return(0);
}


void Initial_IC(void)
{
	RPINR7bits.IC1R = 4; 		// RP4

	IFS0bits.IC1IF = 0;  		//IC1 인터럽트 플래그 초기화
	IPC0bits.IC1IP = 4;  		//IC1 인터럽트 우선순위 레벨4로 설정(디폴트값임)
	IEC0bits.IC1IE = 1;  		//IC1 인터럽트 동작 모드로 설정(Enable)

	IC1CON1bits.ICSIDL = 0; 	//아이들 모드시에도 동작 설정
	IC1CON2bits.SYNCSEL = 16; 	// trig by software

	IC1CON1bits.ICTSEL = 7; 	// System clock (Fosc/2 = 8Mhz / 2 = 0.125 / 2 = 0.0625usec)
	IC1CON1bits.ICI = 0;  		//매번 캡쳐마다 인터럽트 발생 ICI<1:0> = 00
	IC1CON2bits.ICTRIG = 1; 	// Enable trig mode
	IC1CON2bits.TRIGSTAT = 0; 	// clear trig state
	IC1CON1bits.ICM = 1;  		//초기화이므로 동작 중지 ICM<2:0> = 000
}

//****************************************************************
void IC_buffer_clr(void)
{
	unsigned char i;

	for(i=0; i<24; i++) IC_buffer[i] = 0;

	ic=0; 
	edge_low=0; 
	edge_high=0; 
	ir_key=0;
}


unsigned char Pass_Word_Input(unsigned char type)
{
    char pass[4] = {0,0,0,0};
    char x_key = 0;
    char KeyCount = 0;
    char PassOkFlg = 0;
    long LoopCount = 0;
    
    while(1)
    {
        if(type==1)
        {
            if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
            {
                tone_type=SHORT_TONE_PLAY;	
                x_key = Key_Data_Read();
                if(pass[KeyCount] == x_key && !KeyCount)
                {
                    KeyCount++;
                }
                else if(pass[KeyCount] == x_key && KeyCount == 1)
                {
                    KeyCount++;
                }
                else if(pass[KeyCount] == x_key && KeyCount == 2)
                {
                    KeyCount++;
                }
                else if(pass[KeyCount] == x_key && KeyCount == 3)
                {
                    PassOkFlg = 1;
                    tone_type=SHORT_TONE_PLAY;
                    break;
                }
                else break;
            }
            DelayMs(10);
            LoopCount++;
            if(LoopCount > 300) break;
        }
        else break;
    }
    DelayMs(200);
    return PassOkFlg;
}

