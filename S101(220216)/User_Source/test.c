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

extern unsigned char weight_division;


void Test_Mode(void)
{ 
	unsigned char temp_ch;
	unsigned int password_return_value;
	static unsigned char password_enroll_count;

	unsigned char 	x_key=0;
	unsigned char	key_cnt=0;
	unsigned char 	input_num=1;


    eeprom_read();
    
	none_point_flag=1;
	test_lcd_mode_f=1;

	LAMP_LCD3_K_OFF;
	LAMP_LCD3_G_OFF;
	LAMP_LCD3_STABLE_OFF;
	LAMP_LCD3_LB_OFF;
	LAMP_LCD3_ZERO_OFF;


	String_Display(PAY, " TEST "); 
//    String_Display(UNIT_PRICE, "T=215");
	String_Display(PLU, "  "); 
	String_Display(WEIGHT, "     ");

	DelayMs(700);
	

	while(1)
	{
		Low_Batt_check();
		
		String_Display(PAY, " TEST "); 
		String_Display(UNIT_PRICE, " TEST"); 
		String_Display(WEIGHT, "     "); 
		input_num = Input_Plu_Number_Char( input_num );
		plu_display( input_num );

		
		x_key=input_num;

		if( (input_num==0) )	break;
		
		switch(x_key) 
		{
			case ZERO_KEY:	// TEST_MODE			//	1번 키 
				tone_type=SHORT_TONE_PLAY;
				test_lcd();
				input_num=2;
				break;

			case HOLD_KEY:	// SET_MODE			//	2번 키 
				tone_type=SHORT_TONE_PLAY;
				test_key();
				input_num=3;
				break;

			case ADD_KEY:	// CAL_MODE			//	3번 키 
				tone_type=SHORT_TONE_PLAY;
                AdcDataInit();
				test_load_cell();
				input_num=4;
				break;

			case SUM_KEY:								//	4번 키 
				tone_type=SHORT_TONE_PLAY;
				test_back_light();
				input_num=5;
				break;

			case SAVE_KEY:								//	5번 키 
				tone_type=SHORT_TONE_PLAY;
				test_eepr();
				input_num=6;
				break;

			case MULTI_KEY:								//	6번 키 
				tone_type=SHORT_TONE_PLAY;
				rtc_test();
				input_num=1;
				break;

			case F1_KEY:							//	7번 키 
				tone_type=FOUR_TONE_PLAY;
				break;

			case F2_KEY:								//	8번 키 
				tone_type=FOUR_TONE_PLAY;
				break;
				
			case F3_KEY:								//	9번 키 
				tone_type=FOUR_TONE_PLAY;
				break;

			case CLR_K: 		//	RTC_SET_MODE	//	0번 키 		
				tone_type=SHORT_TONE_PLAY;
				break;
				
			case ENT_K: 							//	enter key			
				tone_type=FOUR_TONE_PLAY;
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
    eeprom_read();
    adc_initial();
	String_Display(PAY, "  END "); 
	String_Display(UNIT_PRICE, "     "); 
	String_Display(PLU, "  "); 
	String_Display(WEIGHT, "     "); 
	DelayMs(700);
	
	none_point_flag=0;
	test_lcd_mode_f=0;
}



void test_key(void)
{
	unsigned char 	x_key;
	unsigned char	key_cnt=0;

	String_Display(UNIT_PRICE,"  KEY ");
	String_Display(PLU, "02"); 

	while(1)
	{
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			switch(x_key) 
			{
				case ZERO_KEY:
					tone_type=0x01;
					weight_display(1);
					break;
					
				case HOLD_KEY:
					tone_type=0x01;
					weight_display(2);
					break;

				case ADD_KEY:
					tone_type=0x01;
					weight_display(3);
					break;
				case SUM_KEY:
					tone_type=0x01;
					weight_display(4);
					break;

				case SAVE_KEY:
					tone_type=0x01;
					weight_display(5);
					break;

				case MULTI_KEY:
					tone_type=0x01;
					weight_display(6);
					break;
					
				case F1_KEY:
					tone_type=0x01;
					weight_display(7);
					break;

				case F2_KEY:
					tone_type=0x01;
					weight_display(8);
					break;

				case F3_KEY:
					tone_type=0x01;
					weight_display(9);
					break;

				case CLR_K:
					tone_type=0x01;
					weight_display(0);
					break;
                
                case CAL_K:
					tone_type=0x01;
					weight_display(11);
					break;
                    
				case ENT_K:
					tone_type=0x01;
					weight_display(13);
					DelayMs(700);
					return;
					break;

				case POWER_K:							//	파워, 제로, 임시단가 키
                    tone_type=0x01;
					weight_display(12);
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

void test_eepr(void)
{
	unsigned char	x_key=0;
	unsigned char	key_cnt=0;
    unsigned char 	DFaddr1 = 0;
	unsigned char	DFaddr2 =4;
	unsigned char	DFaddr3 = 8;
	
    unsigned long 	value=0;
	unsigned long	value_check=0;
    unsigned long 	DFdata = 25;
	unsigned long	delay_t=300;

	
	String_Display(PLU,"05");
	String_Display(UNIT_PRICE," MEM ");


	//	25
	tone_type=SHORT_TONE_PLAY;
	Atflash_PluData_Push( PAGE_TEST, DFaddr1, DFdata);
	value=Atflash_PluData_Pop( PAGE_TEST, DFaddr1 );
	pay_display(value, 0);
	DelayMs(delay_t);											
	value_check += value;

	//	50
	tone_type=SHORT_TONE_PLAY;
	Atflash_PluData_Push( PAGE_TEST, DFaddr2, DFdata*2);
	value=Atflash_PluData_Pop( PAGE_TEST, DFaddr2 );
	pay_display(value,0); 
	DelayMs(delay_t);											
	value_check += value;

	//	75
	tone_type=SHORT_TONE_PLAY;
	Atflash_PluData_Push( PAGE_TEST, DFaddr3, DFdata*3);
	value=Atflash_PluData_Pop( PAGE_TEST, DFaddr3 );
	pay_display(value,0); 
	DelayMs(delay_t);											
	value_check += value;


	tone_type=LONG_TONE_PLAY;
	
	if(value_check==150)	String_Display(WEIGHT, " GOOD");	
	else					String_Display(WEIGHT, " FAIL");	


	while(1)
	{
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			switch(x_key) 
			{
				case ENT_K:
					tone_type=LONG_TONE_PLAY;
					return;
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
}

void Get_Flash_Information(void)
{
	unsigned char j=0, i=0, information=0x9F, get_byte[4]={5};
		;
	cbi(LATF, F_SCK);
	cbi(LATF, F_CS);
	cbi(LATF, F_MOSI);

	for(j=0; j<8; j++)  // cmd
	{
		if(information & 0x80)    	sbi(LATF, F_MOSI); 
		else                  		cbi(LATF, F_MOSI);
		
		sbi(LATF, F_SCK);
		cbi(LATF, F_SCK);
		
		information = information << 1;
	}	

	sbi(LATF, F_CS);


	
	cbi(LATF, F_CS);

	sbi(LATF, F_SCK);
	cbi(LATF, F_SCK);


	get_byte[0]=0;
	
	for(i=0; i<8; i++)
	{
		get_byte[0] = get_byte[0]<< 1;
		
		if( ((PORTF&(0x01<<F_MISO))== BV(F_MISO)) )	get_byte[0] = get_byte[0] | 0x01;
		else                                      	get_byte[0] = get_byte[0] & 0xFE;
		
		sbi(LATF, F_SCK);
		cbi(LATF, F_SCK);

		DelayMs(1);
	}

	for(i=0; i<8; i++)
	{
		get_byte[1] = get_byte[1]<< 1;
		
		if( ((PORTF&(0x01<<F_MISO))== BV(F_MISO)) )	get_byte[1] = get_byte[1] | 0x01;
		else                                      	get_byte[1] = get_byte[1] & 0xFE;
		
		sbi(LATF, F_SCK);
		cbi(LATF, F_SCK);

		DelayMs(1);
	}

	
	sbi(LATF, F_CS); 



	


	weight_display(get_byte[1]);
	//while(1);
}


void test_rs232c(void)
{

}

void test_lcd(void)
{
	unsigned char 	x_key;
	unsigned char	key_cnt=0;

	ex_back_light_flag=0;
	test_lcd_seg_on();
	test_lcd_part_off();
	while(1)
	{
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			switch(x_key) 
			{
				case F3_KEY:
					tone_type=SHORT_TONE_PLAY;
					if(ex_back_light_flag)	{	BackLight_Off;	 ex_back_light_flag=0;	}
					else					{	BackLight_On;	 ex_back_light_flag=1;	}
					break;
				
				case ENT_K:
					tone_type=SHORT_TONE_PLAY;
					if(ex_back_light_flag)	{	BackLight_Off;	 ex_back_light_flag=0;	}
					test_lcd_keytest_off();
					return;
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
}

void test_back_light(void)
{
	unsigned char	x_key=0;
	unsigned char	key_cnt=0;
	unsigned char	ep_bright_tmp=0;
	unsigned char	ep_bright_org=0;
	

	ep_bright_org=ep_bright;
	ep_bright=ep_bright_tmp=5;
	ex_back_light_flag=0;

	Initial_PWM();


	String_Display(PAY," TEST ");
	String_Display(PLU,"04");
	String_Display(UNIT_PRICE,"LIGHT");
	while(1)
	{
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			switch(x_key) 
			{
				case F3_KEY:
					tone_type=SHORT_TONE_PLAY;
					
					if(ex_back_light_flag)	
					{	
						BackLight_Off;	 
						ex_back_light_flag=0;	
						String_Display(WEIGHT," OFF ");
					}
					else					
					{	
						BackLight_On;	
						ex_back_light_flag=1;	
						String_Display(WEIGHT," ON  ");	
						lcd_want_display(WEIGHT, 5,ep_bright_tmp);
					}


					break;

				case F2_KEY:
					ex_back_light_flag=1;
					String_Display(WEIGHT," ON  ");
					if( (ep_bright_tmp>=1)&&(ep_bright_tmp<=9) )	ep_bright_tmp--;
					if(ep_bright_tmp<1)								ep_bright_tmp=1;
					OC1R = (ep_bright_tmp+1)*16/10;
					OC1CON1bits.OCM = 6;
					lcd_want_display(WEIGHT, 5,ep_bright_tmp);
					break;

				case CLR_K:
					ex_back_light_flag=1;
					String_Display(WEIGHT," ON  ");
					if( (ep_bright_tmp>=1)&&(ep_bright_tmp<=9) )	ep_bright_tmp++;
					if(ep_bright_tmp>9)								ep_bright_tmp=9;
					OC1R = (ep_bright_tmp+1)*16/10;
					OC1CON1bits.OCM = 6;
					lcd_want_display(WEIGHT, 5,ep_bright_tmp);
					break;


				case ENT_K:
					tone_type=SHORT_TONE_PLAY;
					ep_bright=ep_bright_org;
					Initial_PWM();
					if(ex_back_light_flag)	{	BackLight_Off;	 ex_back_light_flag=0;	}
					return;
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
}

void test_load_cell(void)
{
	unsigned char 	x_key=0;
    unsigned char   RawAnalogFlg=0;
	unsigned char	key_cnt=0;
    unsigned char	ep_minimum_divisionBack=0;
	volatile long 	temp_long=0;
    volatile long 	temp_long0=0;
    volatile long 	temp_long1=0;
    volatile long 	temp_long2=0;
	
	String_Display(UNIT_PRICE,"  AD ");
	String_Display(PLU,"03");
    
    test_adc_initial();
    temp_long1 = read_filtered_adc(); 
	weight_division = ep_minimum_division & 0x07;
    if(ep_minimum_division & 0x08) weight_division = weight_division * 10; 
    if(ep_minimum_division & 0x10) weight_division = weight_division * 100;
	
	while(1)
	{
		do
		{ 
			temp_long = read_filtered_adc(); 
		} 
		while(ex_ad_flag==0);

		if( ( temp_long < 10)||(temp_long > 1000000UL) ) 
		{
			String_Display(WEIGHT,"CH 02");		// LOAD CELL ERROR
           
			while(1)
			{
				do
                { 
                    temp_long = read_filtered_adc(); 
                } 
                while(ex_ad_flag==0);
                if( ( temp_long >= 10)  && (temp_long <= 1000000UL) )
                {
                    break;
                }
                
                if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
				{
					x_key=Key_Data_Read();
					switch(x_key) 
					{
						case ENT_K:
							tone_type=LONG_TONE_PLAY;
							return;
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
		}
		else 
		{
			break;
		}
	}


	temp_long0 = (long)((float)temp_long * ep_res_factor);
	while(1)
	{ 
		temp_long1 = read_filtered_adc(); 
		if(ex_ad_flag)
		{
            temp_long1 = data_avg(temp_long1, 0);
            if(!RawAnalogFlg)
            {
                temp_long1 = (long)((float)temp_long1 * ep_res_factor);
                if(temp_long1 >= temp_long0) 
                {
                    temp_long2 = temp_long1 - temp_long0;
                    ex_sign_flag = 0;
                }
                else 
                {
                    temp_long2 = temp_long0 - temp_long1;
                    ex_sign_flag = 1;
                }
                if(weight_division==2) temp_long = ((temp_long2)/2);   //capa 150kg or 15kg
                else if(weight_division==1) temp_long = (long)((temp_long2)/2); //capa 60kg 
                else if(weight_division==5) temp_long = (long)((temp_long2)/2); //capa 30kg 
                //temp_long = (temp_long * (long)weight_division);
                //if(temp_long < 0) ex_sign_flag = 1;
                ep_minimum_divisionBack = ep_minimum_division;  
                ep_minimum_division=0x20;
                pay_count( temp_long);
                ep_minimum_division = ep_minimum_divisionBack;
                
                if(weight_division==2) temp_long = ((temp_long2)+10) / 20;      //capa 150kg or 15kg
                else if(weight_division==5) temp_long = ((temp_long2)+10) / 20; //capa 30kg 
                else if(weight_division==1) temp_long = ((temp_long2)+10) / 20; //capa 60kg
                temp_long = (temp_long * (long)weight_division);
                //if(temp_long < 0) ex_sign_flag = 1;
                //else ex_sign_flag = 0;
                weight_display(temp_long);
            }
            else
            {
                pay_count( temp_long1/2);   //RAW
                weight_display(0);
            }
		}
		
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			switch(x_key) 
			{
                case ZERO_KEY:
                    tone_type=SHORT_TONE_PLAY;
                    do
                    { 
                        temp_long = read_filtered_adc();
                        temp_long0 = (long)((float)temp_long * ep_res_factor);
                    } 
                    while(ex_ad_flag==0);
                    break;
                case HOLD_KEY:
                    tone_type=SHORT_TONE_PLAY;
                    if(RawAnalogFlg)RawAnalogFlg = 0;
                    else RawAnalogFlg=1;
					break;
                    
				case ENT_K:
					tone_type=SHORT_TONE_PLAY;
					return;
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
}

