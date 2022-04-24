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


DATA_EE_FLAGS dataEEFlags;

//Data EE info stored in PM in following format
//  Status in first two locations of PM page,
//  8-bit DEE Address (odd address, low byte) 16-bit DEE data (even address)
#ifdef __AUXFLASH

#define DEE_BANK_SIZE (NUMBER_OF_INSTRUCTIONS_IN_PAGE * 2 * NUM_DATA_EE_PAGES)
#define DEE_PAGE_SIZE (NUMBER_OF_INSTRUCTIONS_IN_PAGE * 2)

#define DEE_PAGE_TBL(bank, page) ((0x7FC000 + (DEE_BANK_SIZE * (bank)) + (DEE_PAGE_SIZE * (page))) >> 16)
#define DEE_PAGE_OFFSET(bank, page) ((0x7FC000 + (DEE_BANK_SIZE * (bank)) + (DEE_PAGE_SIZE * (page))) & 0xFFFF)

#else

unsigned char emulationPages[DATA_EE_BANKS * NUM_DATA_EE_PAGES][NUMBER_OF_INSTRUCTIONS_IN_PAGE * 2]
    __attribute__ ((space(psv), aligned(NUMBER_OF_INSTRUCTIONS_IN_PAGE * 2), noload));

#define DEE_BANK_SIZE (sizeof(emulationPages[0])*NUM_DATA_EE_PAGES)
#define DEE_PAGE_SIZE (sizeof(emulationPages[0]))

#if __C30_VERSION__ > 301
    #define DEE_PAGE_TBL(bank, page) ((__builtin_tbladdress(&emulationPages) + (DEE_BANK_SIZE * (bank)) + (DEE_PAGE_SIZE * (page))) >> 16)
    #define DEE_PAGE_OFFSET(bank, page) ((__builtin_tbladdress(&emulationPages) + (DEE_BANK_SIZE * (bank)) + (DEE_PAGE_SIZE * (page))) & 0xFFFF)
#else
    #warning "Please upgrade your C30 compiler"
    #define DEE_PAGE_TBL(bank, page) ((((((unsigned long)__builtin_tblpage(&emulationPages)) << 16) + __builtin_tbloffset(&emulationPages)) + \
                                     (DEE_BANK_SIZE * (bank)) + (DEE_PAGE_SIZE * (page))) >> 16)
    #define DEE_PAGE_OFFSET(bank, page) ((((((unsigned long)__builtin_tblpage(&emulationPages)) << 16) + __builtin_tbloffset(&emulationPages)) + \
                                        (DEE_BANK_SIZE * (bank)) + (DEE_PAGE_SIZE * (page))) & 0xFFFF)
#endif

#endif




void Setting_Mode(void)
{ 
	unsigned char			x_key=0;
	unsigned char			input_num_menu=1;
	unsigned char 			temp_ch;
	unsigned char			plu_init=0;
	unsigned char			i=0;
	unsigned int 			password_return_value;
	static unsigned char	password_enroll_count;

	LAMP_LCD3_K_OFF;
	LAMP_LCD3_G_OFF;
	LAMP_LCD3_STABLE_OFF;
	LAMP_LCD3_LB_OFF;
	LAMP_LCD3_ZERO_OFF;


	String_Display(PAY, "  SET "); 
	String_Display(UNIT_PRICE, "     "); 
	String_Display(WEIGHT, "     "); 
	String_Display(PLU, "  ");
	DelayMs(700);
    
	while(1)
	{
		Low_Batt_check();
		String_Display(WEIGHT, "     "); 
		String_Display(UNIT_PRICE, " F   "); 

		input_num_menu = LCD2_2Byte_Input_Blink(input_num_menu, 0);
		String_Display(UNIT_PRICE, " F   "); 
		lcd_want_display(UNIT_PRICE, 3, input_num_menu/10);
		lcd_want_display(UNIT_PRICE, 4, input_num_menu%10);

		
		switch(input_num_menu) 
		{
			case 0:		// SAVE & END
				tone_type=LONG_TONE_PLAY;
				String_Display(WEIGHT, " END "); 
				DelayMs(700);
			 	return;
			 	break;
				
			case 1:	//F01 Digital Filter (1~9)
				tone_type=SHORT_TONE_PLAY;
				none_point_flag=1;
				ep_filter_degree = LCD3_1Byte_test_mode_Input_Blink(ep_filter_degree, 1, 9);
				none_point_flag=0;
				if(ep_filter_degree==0) ep_filter_degree = 1;
				adc_initial(); 
				DelayMs(10);
				eeprom_write();
				DelayMs(10); 
				input_num_menu++;
				break;

			case 2:	//F02 Weight Backup (0~1)
				tone_type=SHORT_TONE_PLAY;
				none_point_flag=1;
				ep_wei_backup = LCD3_1Byte_test_mode_Input_Blink(ep_wei_backup, 0, 1);
				none_point_flag=0;
				if(ep_wei_backup>1) ep_wei_backup = 0;
				eeprom_write();
				DelayMs(10);
                if(ModelRecognition)input_num_menu = 4;
                else input_num_menu++;
				break;
           
			case 3:	//F03 Stable condition (1~9)
				tone_type=SHORT_TONE_PLAY;
				none_point_flag=1;
				ep_stable = LCD3_1Byte_test_mode_Input_Blink(ep_stable, 1, 9);
				none_point_flag=0;
				if(ep_stable==0) ep_stable = 1;
				eeprom_write();
				DelayMs(10); 
				input_num_menu++;
				break;
                
            case 4:	//F20 자동영점 지정 (0~99)////////////////////////////////////////////////////						
				tone_type=SHORT_TONE_PLAY;
				temp_ch = ep_az_digit * 10 + ep_az_second;
				temp_ch = LCD3_2Byte_test_mode_Input_Blink(temp_ch);
				ep_az_digit  = temp_ch / 10;
				ep_az_second = temp_ch % 10;
				eeprom_write();
				DelayMs(10); 
				input_num_menu++;
				break;
                
			case 5:	//F04 Hold Speed (10~99)/////////////////////////////////////////////////////								
				tone_type=SHORT_TONE_PLAY;
				//ep_holdspeed = LCD3_1Byte_test_mode_Input_Blink(ep_holdspeed, 1, 9);
				ep_holdspeed = LCD3_2Byte_test_mode_Input_Blink(ep_holdspeed);
				if((ep_holdspeed > 99)||(ep_holdspeed<10))
                {
                    String_Display(WEIGHT, "  ERR ");
                    ep_holdspeed = 10;
                    DelayMs(1000); 
                }
                else
                {
                    eeprom_write();
                    DelayMs(10); 
                    input_num_menu++;
                }
				break;

			case 6:	//F05 자동홀드 시작(0~1)								
				tone_type=SHORT_TONE_PLAY;
				none_point_flag=1;
				ep_hold_start = LCD3_1Byte_test_mode_Input_Blink(ep_hold_start, 0, 1);
				none_point_flag=0;
				eeprom_write();
				DelayMs(10); 
				input_num_menu++;
				break;

			case 7:	//F06 영점부근 신호 (홀드 풀림) (0~9)						
				tone_type=SHORT_TONE_PLAY;
				none_point_flag=1;
				ep_hold_zero = LCD3_1Byte_test_mode_Input_Blink(ep_hold_zero, 0, 9);
				none_point_flag=0;
				eeprom_write();
				DelayMs(10); 
				input_num_menu++;
				break;

			#if 0
			case 8:	//F08 * 키 사용용도(0~3)								
				tone_type=SHORT_TONE_PLAY;
				#if 0
				none_point_flag=1;
				ep_star = LCD3_1Byte_test_mode_Input_Blink(ep_star, 0, 3);
				none_point_flag=0;
				#endif 
				eeprom_write();
				DelayMs(10); 
				input_num_menu++;
				break;
			#endif 

			case 8:	//F07 통신 포맷(0~12) 				
				tone_type=SHORT_TONE_PLAY;
				none_point_flag=1;
				ep_rs_form = LCD3_2Byte_test_mode_Input_Blink(ep_rs_form);
				none_point_flag=0;
				eeprom_write();
				DelayMs(10); 
				input_num_menu++;
				break;
                
			case 9:	//F08 프린트 간격지정(0~9)										
				tone_type=SHORT_TONE_PLAY;
				none_point_flag=1;
				ep_feed = LCD3_1Byte_test_mode_Input_Blink(ep_feed, 0, 9);
				none_point_flag=0;
				eeprom_write();
				DelayMs(10); 
				input_num_menu++;
				break;
                
			case 10:	//F09 계량번호 초기화(0, 1)									
				tone_type=SHORT_TONE_PLAY;
				none_point_flag=1;
				ex_wei_clr = LCD3_1Byte_test_mode_Input_Blink(ex_wei_clr, 0, 1);
				none_point_flag=0;
				if(ex_wei_clr) 
				{
					ex_wei_clr=0;
					ep_wei_no=1;
					ex_serial_no=0;
					Atflash_PluData_Push( PAGE_TEST, (unsigned char)200, (unsigned long)ex_serial_no);
				}
				eeprom_write();
				DelayMs(10); 
				input_num_menu++;
				break;
				
			case 11:	//F10 무선 통신 설정(0~4)										
				tone_type=SHORT_TONE_PLAY;
				none_point_flag=1;
				ep_wireless = LCD3_1Byte_test_mode_Input_Blink(ep_wireless, 0, 4);
				none_point_flag=0;
				eeprom_write();
				DelayMs(10); 
				input_num_menu++;
				break;
				
			case 12:	//F11 Backlight 사용(0~2)										
				tone_type=SHORT_TONE_PLAY;
				none_point_flag=1;
				ep_backlight = LCD3_1Byte_test_mode_Input_Blink(ep_backlight, 0, 2);
				none_point_flag=0;
				eeprom_write();
				DelayMs(10); 
				input_num_menu++;
				break;
				
			case 13:	//F12 Backlight brightness (1~9)										
				tone_type=SHORT_TONE_PLAY;
				none_point_flag=1;
				ep_bright = LCD3_1Byte_test_mode_Input_Blink(ep_bright, 1, 9); 
				none_point_flag=0;
				eeprom_write();
				DelayMs(10); 
				input_num_menu++;
				break;
				
			case 14:	//F13 PLU Init (0~1)									
				tone_type=SHORT_TONE_PLAY;
				none_point_flag=1;
				plu_init = LCD3_1Byte_test_mode_Input_Blink(plu_init, 0, 1); 
				none_point_flag=0;
				
				if(plu_init==1)
				{
					for(i=0; i<50; i++)
					{
						Atflash_PluData_Push(PLU_U_P_HEAD, i*4, 0);
					}
					
					for(i=0; i<50; i++)
					{
						Atflash_PluData_Push(PLU_U_P_TAIL, i*4, 0);
					}
				
					
					ex_atflash_PluNum=Atflash_PluData_Pop(PLU_CUR, 200);
					
					if(ex_atflash_PluNum<50)	ex_atflash_PluReadValue=Atflash_PluData_Pop(PLU_U_P_HEAD, (ex_atflash_PluNum*4) );
					else						ex_atflash_PluReadValue=Atflash_PluData_Pop(PLU_U_P_TAIL, ( (ex_atflash_PluNum-50)*4) );
					
                    if(ex_atflash_PluReadValue > 99999) ex_atflash_PluReadValue = 0;
					ex_atflash_PluWriteValue=ex_atflash_PluReadValue;
					
					
					ex_serial_no=(unsigned short)Atflash_PluData_Pop( PAGE_TEST, 200 );
				}
				if(plu_init>0)	plu_init=0;
                input_num_menu++;
				
				break;
            
			case 15:	//F15 PLU Unit (0~1)									
				tone_type=SHORT_TONE_PLAY;
				none_point_flag=1;
				ep_plu_unit = LCD3_1Byte_test_mode_Input_Blink(ep_plu_unit, 0, 1); 
				none_point_flag=0;
				if(ep_plu_unit>1)	ep_plu_unit=0;
				eeprom_write();
				DelayMs(10); 
				input_num_menu++;  
				break;
                
            case 16:	//F08 * 키 사용용도(0~1)								
                tone_type=SHORT_TONE_PLAY;
                none_point_flag=1;
                ep_enter_key_set = LCD3_1Byte_test_mode_Input_Blink(ep_enter_key_set, 0, 1);
                none_point_flag=0;
                eeprom_write();
                DelayMs(10); 
                input_num_menu = 1;
                break;
           
			default: 
				break;
		}
	}
    eeprom_read();
}


void HiddinSet_Mode(void)
{ 
	unsigned char			x_key=0;
	unsigned char			input_num_menu=1;
	unsigned char 			temp_ch;
	unsigned char			plu_init=0;
	unsigned char			i=0;
	unsigned int 			password_return_value;
	static unsigned char	password_enroll_count;

	LAMP_LCD3_K_OFF;
	LAMP_LCD3_G_OFF;
	LAMP_LCD3_STABLE_OFF;
	LAMP_LCD3_LB_OFF;
	LAMP_LCD3_ZERO_OFF;


	String_Display(PAY, "HF-SET"); 
	String_Display(UNIT_PRICE, "     "); 
	String_Display(WEIGHT, "     "); 
	String_Display(PLU, "  ");
	DelayMs(700);
    
	while(1)
	{
		Low_Batt_check();
		String_Display(WEIGHT, "     "); 
		String_Display(UNIT_PRICE, "HF-  "); 

		input_num_menu = LCD2_2Byte_Input_Blink(input_num_menu, 1);
		String_Display(UNIT_PRICE, "HF-  "); 
		lcd_want_display(UNIT_PRICE, 4, input_num_menu/10);
		lcd_want_display(UNIT_PRICE, 5, input_num_menu%10);

		
		switch(input_num_menu) 
		{
			case 0:		// SAVE & END
				tone_type=LONG_TONE_PLAY;
				String_Display(WEIGHT, " END "); 
				DelayMs(700);
			 	return;
			 	break;
             
            case 1:	//HF1 마이너스 표시 허용 19 Division 이하 표시/
                tone_type=SHORT_TONE_PLAY;
				none_point_flag=1;
				ep_MinusDiplayEnable = LCD3_1Byte_test_mode_Input_Blink(ep_MinusDiplayEnable, 0,1);
				none_point_flag=0;
				if(ep_MinusDiplayEnable>1) ep_MinusDiplayEnable = 0;
                //ep_minus_display_enable = 0;  // 형식 승인
				eeprom_write();
				DelayMs(10); 
				input_num_menu++;
				break;
                
            case 2: //HF2 key tare use
                tone_type=SHORT_TONE_PLAY;
                none_point_flag=1;
				KeyTareEnable = LCD3_1Byte_test_mode_Input_Blink(KeyTareEnable, 0, 1);
				none_point_flag=0;
                eeprom_write();
				DelayMs(10); 
				input_num_menu++;
				break;
                
			case 3:	//HF3 영점 범위 설정, 2%~20%/////////////////////////////////////////////////////								
				tone_type=SHORT_TONE_PLAY;
				none_point_flag=1;
				ep_zero_range_value = LCD3_2Byte_test_mode_Input_Blink(ep_zero_range_value);
				none_point_flag=0;
				if(ep_zero_range_value>20 || ep_zero_range_value < 2)
                {
                    String_Display(WEIGHT, "  ERR ");
                    ep_zero_range_value = 2;
                    DelayMs(1000); 
                }
                else
                {
                    eeprom_write();
                    DelayMs(10); 
                    input_num_menu++;
                }
				break;
            
            case 4:	//HF4 가격 반올림 설정 '0' 반올림 안함 '1' 반올림 사용/////////////////////////////////////////////////////								
				tone_type=SHORT_TONE_PLAY;
				none_point_flag=1;
                round_off = LCD3_1Byte_test_mode_Input_Blink(round_off, 0, 1);
				none_point_flag=0;
				eeprom_write();
				DelayMs(10); 
				input_num_menu++;
				break;
                
            case 5:	//HF5 절삭 자리수   '0' 절삭 없음 '1' 자리 절삭 '2' 10자리 절삭 '3' 100자리 절삭  '4' 1000 자리 절삭 /////////////////////////////////////////////////////								
				tone_type=SHORT_TONE_PLAY;
				none_point_flag=1;
                cut_cipher = LCD3_1Byte_test_mode_Input_Blink(cut_cipher, 0, 4);
				none_point_flag=0;
                if(cut_cipher>4) cut_cipher = 1;
				eeprom_write();
				DelayMs(10); 
				input_num_menu = 1;
				break;
                
			default: 
				break;
		}
	}

#if 0
		//F11 프린터 양식(0~1)
		String_Display(UNIT_PRICE, " F11 ");
		none_point_flag=1;
		ep_prt_form = Input_Number_Weight_ONE(ep_prt_form);
		none_point_flag=0;
#endif 
        eeprom_read();
}

void cond_check(void)
{ 
	unsigned char i=0;
	
	if(ep_maximum_capacity > 50000) 
	{
		ep_maximum_capacity = 15;
	}

	if( (ep_minimum_division > 0x81)||(ep_minimum_division < 0x01) ) ep_minimum_division = 0x22; 
	if( (ep_gravity_factor1  < 9700)||(ep_gravity_factor1  > 9900) ) ep_gravity_factor1  = 9799;
	if( (ep_gravity_factor2  < 9700)||(ep_gravity_factor2  > 9900) ) ep_gravity_factor2  = 9799;

	if(ep_hold_start >     1) ep_hold_start = 0;
	if(ep_hold_zero  >     9) ep_hold_zero  = 5;
	if(ep_star       >     3) ep_star       = 0;
    if(ep_enter_key_set >  1) ep_enter_key_set = 0;
	if(ep_id         >     9) ep_id         = 0;
	if(ep_rs_form    >     12) ep_rs_form    = 0;
	if(ep_prt_form   >     1) ep_prt_form   = 0;
	if(ep_feed       >     9) ep_feed       = 5;
	if(ep_wei_no     >   999) ep_wei_no     = 1;
	if(ep_wireless   >     4) ep_wireless   = 0;
	if(ep_backlight  >     2) ep_backlight  = 0;
	if(ep_bright     >     9) ep_bright     = 5;
	if(ep_wei_backup >     1) ep_wei_backup = 0;
	if(ep_az_digit   >     9) ep_az_digit   = 2;
	if(ep_az_second  >     9) ep_az_second  = 3;
	if(ep_tare_value > 99999) ep_tare_value = 0;
	if(ep_tare_flag  >     1) ep_tare_flag  = 0;
	if(ep_cal_no     >   100) ep_cal_no     = 0;
	if(ep_short_plu  >  9999) ep_short_plu  = 100;
	if(ep_unit  	 >     2) ep_unit 		= 0;
	if(ep_hold_range_value> 99) ep_hold_range_value=10;		// Hold Range 초기화  : 단위 눈금 x 10
	if(ep_zero_range_value> 20) ep_zero_range_value=2;		// Zero Range 초기화  : 2
	if(ep_plu_unit> 1) 			ep_plu_unit=0;		// Zero Range 초기화  : 2
	if(ep_rtc_hour>12)			ep_rtc_hour=11;
	if(ep_rtc_minute>59)		ep_rtc_minute=4;

	if( (ep_long_key_time<10)||(ep_long_key_time>30) )	ep_long_key_time=20;	
	if(KeyTareEnable >1)	KeyTareEnable=0;
    if(cut_cipher > 4)  cut_cipher = 1;
    if(round_off > 1)  round_off = 0;
    
    if(ep_MinusDiplayEnable >1)	ep_MinusDiplayEnable=0;



	if( (ep_filter_degree > 9)||(ep_filter_degree==0) ) ep_filter_degree = 5;
	if( (ep_stable        > 9)||(ep_stable       ==0) ) ep_stable        = 1;
	if( (ep_holdspeed > 99)||(ep_holdspeed<10)) ep_holdspeed = 10;
	//if((ep_holdspeed > 9)||(!ep_holdspeed)) ep_holdspeed = 4;
	if((ep_res_factor <=  0.0)||(ep_res_factor > 10.0)) 
	{ 
		String_Display(PAY, " CH 01");
		//buzzer_delay=150; 
		DelayMs(700); 
		eeprom_write(); 
		DelayMs(10);
	}

	for(i=0; i<100; i++)
	{
		if(ep_plu_no_value[i]>9999)	
		{	
			ep_plu_no_value[i]=100;
		}
	}
}

void eeprom_write(void)
{
	unsigned char i=0;
	unsigned long eep_temp;


	ep_res_factor = ep_res_factor * 10000000;
	eep_temp = (unsigned long)ep_res_factor & 0xFFFF0000;
	eep_temp = eep_temp >> 16;
	DataEEWrite(eep_temp, EP_RES_FACTOR);
	eep_temp = (unsigned long)ep_res_factor & 0x0000FFFF;
	DataEEWrite(eep_temp, EP_RES_FACTOR+1);
	ep_res_factor = ep_res_factor / 10000000L;
    
    ep_res_factor_dual = ep_res_factor_dual * 10000000;   //dual factor
	eep_temp = (unsigned long)ep_res_factor_dual & 0xFFFF0000;
	eep_temp = eep_temp >> 16;
	DataEEWrite(eep_temp, EP_RES_FACTOR_DUAL);
	eep_temp = (unsigned long)ep_res_factor_dual & 0x0000FFFF;
	DataEEWrite(eep_temp, EP_RES_FACTOR_DUAL+1);
	ep_res_factor_dual = ep_res_factor_dual / 10000000L;

	eep_temp = ep_zero & 0xFFFF0000;
	eep_temp = eep_temp >> 16;
	DataEEWrite(eep_temp, EP_ZERO);
	eep_temp = (unsigned long)ep_zero & 0x0000FFFF;
	DataEEWrite(eep_temp, EP_ZERO+1);

	eep_temp = ep_maximum_capacity & 0xFFFF0000;
	eep_temp = eep_temp >> 16;
	DataEEWrite(eep_temp, EP_MAXIMUM_CAPACITY);
	eep_temp = (unsigned long)ep_maximum_capacity & 0x0000FFFF;
	DataEEWrite(eep_temp, EP_MAXIMUM_CAPACITY+1);

	eep_temp = ep_e_resolution & 0xFFFF0000;
	eep_temp = eep_temp >> 16;
	DataEEWrite(eep_temp, EP_E_RESOLUTION);
	eep_temp = (unsigned long)ep_e_resolution & 0x0000FFFF;
	DataEEWrite(eep_temp, EP_E_RESOLUTION+1);
    
    eep_temp = ep_e_resolution_dual & 0xFFFF0000;
	eep_temp = eep_temp >> 16;
	DataEEWrite(eep_temp, EP_E_RESOLUTION_DUAL);
	eep_temp = (unsigned long)ep_e_resolution_dual & 0x0000FFFF;
	DataEEWrite(eep_temp, EP_E_RESOLUTION_DUAL+1);

	eep_temp = ep_tare_value & 0xFFFF0000;
	eep_temp = eep_temp >> 16;
	DataEEWrite(eep_temp, EP_TARE_VALUE);
	eep_temp = (unsigned long)ep_tare_value & 0x0000FFFF;
	DataEEWrite(eep_temp, EP_TARE_VALUE+1);

    eep_temp = ep_DualRangeBoundary & 0xFFFF0000;
	eep_temp = eep_temp >> 16;
	DataEEWrite(eep_temp, EP_DUAL_RANGE_BOUNDARY);
	eep_temp = (unsigned long)ep_DualRangeBoundary & 0x0000FFFF;
	DataEEWrite(eep_temp, EP_DUAL_RANGE_BOUNDARY+1);
    
	DataEEWrite(ep_gravity_factor1, 	EP_GRAVITY_FACTOR1);
	DataEEWrite(ep_gravity_factor2, 	EP_GRAVITY_FACTOR2);
	DataEEWrite(ep_minimum_division, 	EP_MINIMUM_DIVISION);
    DataEEWrite(ep_minimum_division_dual, 	EP_MINIMUM_DIVISION_DUAL);
	DataEEWrite(ep_cal_no, 				EP_CAL_NO);

	DataEEWrite(ep_filter_degree, 		EP_FILTER_DEGREE);
	DataEEWrite(ep_stable, 				EP_STABLE);
	DataEEWrite(ep_az_digit, 			EP_AZ_DIGIT);
	DataEEWrite(ep_az_second, 			EP_AZ_SECOND);
	DataEEWrite(ep_holdspeed, 			EP_HOLDSPEED);
	DataEEWrite(ep_hold_start, 			EP_HOLD_START);
	DataEEWrite(ep_hold_zero, 			EP_HOLD_ZERO);
	DataEEWrite(ep_star, 				EP_STAR);
	DataEEWrite(ep_id, 					EP_ID);
	DataEEWrite(ep_rs_form, 			EP_RS_FORM);
	DataEEWrite(ep_prt_form, 			EP_PRT_FORM);
	DataEEWrite(ep_feed, 				EP_FEED);
	DataEEWrite(ep_wei_no, 				EP_WEI_NO);
	DataEEWrite(ep_wireless, 			EP_WIRELESS);
	DataEEWrite(ep_backlight, 			EP_BACKLIGHT);
	DataEEWrite(ep_bright, 				EP_BRIGHT);
	DataEEWrite(ep_tare_flag, 			EP_TARE_FLAG);
	DataEEWrite(ep_wei_backup, 			EP_WEI_BACKUP);
	DataEEWrite(ep_unit, 				EP_UNIT);

	DataEEWrite(ep_hold_range_value, 	EP_HOLD_RANGE);
	DataEEWrite(ep_zero_range_value, 	EP_ZERO_RANGE_VALUE);
	DataEEWrite(ep_plu_unit, 			EP_PLU_UNIT);
	
	DataEEWrite(ep_rtc_hour, 			EP_RTC_HOUR);
	DataEEWrite(ep_rtc_minute, 			EP_RTC_MINUTE);

	DataEEWrite(ep_long_key_time,		EP_LONG_KEY_TIME);
    DataEEWrite(ep_MinusDiplayEnable,   EP_MINUS_DISPLAY);
    DataEEWrite(KeyTareEnable,          EP_KEY_TARE_ENABLE);
    DataEEWrite(cut_cipher,             EP_CIPHER);
    DataEEWrite(round_off,              EP_ROUND_OFF);
    DataEEWrite(ep_enter_key_set,       EP_ENTER_KEY_SET);
}


void eeprom_read(void)
{
	unsigned char i=0;
	unsigned long eep_temp;

	

	eep_temp = DataEERead(EP_RES_FACTOR);
	ep_res_factor = eep_temp << 16;
	eep_temp = DataEERead(EP_RES_FACTOR+1);
	ep_res_factor = eep_temp | (unsigned long)ep_res_factor;
	ep_res_factor = ep_res_factor / 10000000L;
    
    eep_temp = DataEERead(EP_RES_FACTOR_DUAL);
	ep_res_factor_dual = eep_temp << 16;
	eep_temp = DataEERead(EP_RES_FACTOR_DUAL+1);
	ep_res_factor_dual = eep_temp | (unsigned long)ep_res_factor_dual;
	ep_res_factor_dual = ep_res_factor_dual / 10000000L;

	eep_temp = DataEERead(EP_ZERO);   
	ep_zero = eep_temp << 16;
	eep_temp = DataEERead(EP_ZERO+1);   
	ep_zero = eep_temp | ep_zero;

	eep_temp = DataEERead(EP_MAXIMUM_CAPACITY);   
	ep_maximum_capacity = eep_temp << 16;
	eep_temp = DataEERead(EP_MAXIMUM_CAPACITY+1);   
	ep_maximum_capacity = eep_temp | ep_maximum_capacity;
  
	eep_temp = DataEERead(EP_E_RESOLUTION);   
	ep_e_resolution = eep_temp << 16;
	eep_temp = DataEERead(EP_E_RESOLUTION+1);   
	ep_e_resolution = eep_temp | ep_e_resolution;
    
    eep_temp = DataEERead(EP_E_RESOLUTION_DUAL);   
	ep_e_resolution_dual = eep_temp << 16;
	eep_temp = DataEERead(EP_E_RESOLUTION_DUAL+1);   
	ep_e_resolution_dual = eep_temp | ep_e_resolution_dual;

	eep_temp = DataEERead(EP_TARE_VALUE);   
	ep_tare_value = eep_temp << 16;
	eep_temp = DataEERead(EP_TARE_VALUE+1);   
	ep_tare_value = eep_temp | ep_tare_value;

    eep_temp = DataEERead(EP_DUAL_RANGE_BOUNDARY);   
	ep_DualRangeBoundary = eep_temp << 16;
	eep_temp = DataEERead(EP_DUAL_RANGE_BOUNDARY+1);   
	ep_DualRangeBoundary = eep_temp | ep_DualRangeBoundary;

    
	ep_gravity_factor1  = DataEERead(EP_GRAVITY_FACTOR1);   
	ep_gravity_factor2  = DataEERead(EP_GRAVITY_FACTOR2);   
	ep_minimum_division = DataEERead(EP_MINIMUM_DIVISION);   
    ep_minimum_division_dual = DataEERead(EP_MINIMUM_DIVISION_DUAL); 
	ep_cal_no           = DataEERead(EP_CAL_NO);   

	ep_filter_degree    = DataEERead(EP_FILTER_DEGREE);     
	ep_stable           = DataEERead(EP_STABLE);   
	ep_az_digit         = DataEERead(EP_AZ_DIGIT);   
	ep_az_second        = DataEERead(EP_AZ_SECOND);   
	ep_holdspeed        = DataEERead(EP_HOLDSPEED);   
	ep_hold_start       = DataEERead(EP_HOLD_START);   
	ep_hold_zero        = DataEERead(EP_HOLD_ZERO);   
	ep_star             = DataEERead(EP_STAR);   
	ep_id               = DataEERead(EP_ID);   
	ep_rs_form          = DataEERead(EP_RS_FORM);   
	ep_prt_form         = DataEERead(EP_PRT_FORM);    
	ep_feed             = DataEERead(EP_FEED);   
	ep_wei_no           = DataEERead(EP_WEI_NO);   
	ep_wireless         = DataEERead(EP_WIRELESS);   
	ep_backlight        = DataEERead(EP_BACKLIGHT);   
	ep_bright           = DataEERead(EP_BRIGHT);   


	ep_tare_flag        = DataEERead(EP_TARE_FLAG);   
	ep_wei_backup       = DataEERead(EP_WEI_BACKUP);  
	ep_unit       		= DataEERead(EP_UNIT);  

	ep_hold_range_value = DataEERead(EP_HOLD_RANGE);   
	ep_zero_range_value = DataEERead(EP_ZERO_RANGE_VALUE);   
	ep_plu_unit 		= DataEERead(EP_PLU_UNIT);   

	ep_rtc_hour 		= DataEERead(EP_RTC_HOUR);   
	ep_rtc_minute 		= DataEERead(EP_RTC_MINUTE);   

	
	ep_long_key_time 	= DataEERead(EP_LONG_KEY_TIME);   
    KeyTareEnable       = DataEERead(EP_KEY_TARE_ENABLE);
    ep_MinusDiplayEnable= DataEERead(EP_MINUS_DISPLAY);
    cut_cipher          = DataEERead(EP_CIPHER);
    round_off           = DataEERead(EP_ROUND_OFF);
    ep_enter_key_set    = DataEERead(EP_ENTER_KEY_SET);
    
	DelayMs(20);  
	
    if(ModelRecognition)  //형식승인
    {
        ep_az_digit   = 2;
        ep_az_second  = 3;
        ep_stable = 1;
        ep_long_key_time = 10;
        ep_zero_range_value = 2;
        KeyTareEnable = 0;
        ep_unit = UNIT_KG;   //AND용 kg FIX
    }
    
	Initial_PWM();
	
	//if(ep_backlight==2) 	BackLight_On;
	//else               		BackLight_Off; 
    
	DelayMs(20);
}














/************************************************************************
UnlockWrite

This routine saves the current CPU priority and sets it the highest
user level of 7. It calls an assembly routine to perform an unlock
sequence and sets the WR bit in NVMCON. The WR bit is polled until it
clears indicating the flash operation is complete. The previous CPU
priority is restored.

Parameters:		None
Return:			None
Side Effects:	None
************************************************************************/
void UnlockWrite(void)
{
    unsigned int sh_SR;

    SET_AND_SAVE_CPU_IPL(sh_SR, 7);

    UnlockPM();

    RESTORE_CPU_IPL(sh_SR);

    return;
}

/************************************************************************
GetPageStatus

This routine returns the page status for the selected page for the
selected field. The return value is right shifted into LSb position.

Parameters:		Page number, Status Field
Return:			Right justified bit value representing selected Status
                Field value
Side Effects:	None
************************************************************************/
int GetPageStatus(unsigned char bank, unsigned char page, unsigned char field)
{
    unsigned int statusOffset;
    unsigned char statusByte;
    unsigned char status;
    int savedTBLPAG;        //Context save of TBLPAG value. Current and packed page are on same page.

    savedTBLPAG = TBLPAG;

    // Point to proper TBLPAG and offset
    TBLPAG = DEE_PAGE_TBL(bank, page);
    statusOffset = DEE_PAGE_OFFSET(bank, page);

    statusByte = (ReadPMHigh(statusOffset) & 0xFF);

    switch(field)
    {
        case STATUS_AVAILABLE:
            status = ((statusByte & 4) >> 2);
            break;
        case STATUS_CURRENT:
            status = ((statusByte & 8) >> 3);
            break;
        case STATUS_EXPIRED:
            status = ((statusByte & 16) >> 4);
            break;
        default:
            status = 0;
            break;
    }

    TBLPAG = savedTBLPAG;

    return(status);
}

/************************************************************************
ErasePage

This routine erases the selected page.

Parameters:		Page number
Return:			None
Side Effects:	Loads NVCOM with erase opcode
************************************************************************/
void ErasePage(unsigned char bank, unsigned char page)
{
    unsigned int pmOffset;           //Current array (page) offset of selected element (PM 16-bit word)
    int savedTBLPAG;        //Context save of TBLPAG value. Current and packed page are on same page.

    savedTBLPAG = TBLPAG;

    // Point to proper TBLPAG and offset
    TBLPAG = DEE_PAGE_TBL(bank, page);

    NVMCON = ERASE;

    pmOffset = DEE_PAGE_OFFSET(bank, page);

    WritePMLow(pmOffset, pmOffset);

    UnlockWrite();

    TBLPAG = savedTBLPAG;

    return;
}

/************************************************************************
GetNextAvailCount

This routine finds the active page and performs a backward search to find
the first available location. The available location is determined by
reading an LSB (odd address) with 0xFF. The returned value can be added
to the first address in page to compute the available address. A return
value of 0 means the entire page was filled which is an error condition.
This routine can be called by the user to determine how full the current
page is prior to a pack.

Parameters:		None
Return:			Page offset to next available location
Side Effects:	None
************************************************************************/
unsigned int GetNextAvailCount(unsigned char bank)
{
    int i = 0;
    int currentPage;        //Array row (PM page) of active DEE page
    unsigned char dataEEval;
    unsigned int pmOffset;           //Current array (page) offset of selected element (PM 16-bit word)
    int savedTBLPAG;        //Context save of TBLPAG value. Current and packed page are on same page.

    savedTBLPAG = TBLPAG;

    // Find the active page.
    for (currentPage = 0;
         (currentPage < NUM_DATA_EE_PAGES) &&
         (GetPageStatus(bank, currentPage, STATUS_CURRENT) == PAGE_NOT_CURRENT);
         currentPage++) {}

    TBLPAG = DEE_PAGE_TBL(bank, currentPage);
    pmOffset = DEE_PAGE_OFFSET(bank, currentPage);

    do
    {
        i+=2;
        pmOffset += 2;

        dataEEval = (ReadPMHigh(pmOffset) & 0xFF);
    }
    while ((i<NUMBER_OF_INSTRUCTIONS_IN_PAGE * 2) && (dataEEval != 0xFF));

    if(i == NUMBER_OF_INSTRUCTIONS_IN_PAGE * 2)
    {
        i = 0;  //Error - No available locations
    }

    TBLPAG = savedTBLPAG;

    return(i);
}

/************************************************************************
PackEE

This routine finds the active page and an unexpired packed page. The most
recent data EEPROM values are located for each address using ReadEE
function and written into write latches. Page status is read from active
page and erase/write count is incremented if page 0 is packed. After all
information is programmed and verified, the current page is erased. The
packed page becomes the active page. This function can be called at any-
time by the user to schedule the CPU stall.

Parameters:		None
Return:			Status value (0 for pass)
Side Effects:	Generates CPU stall during program/erase operations and
                overwrites program memory write latches. Data EE flags
                may be updated
************************************************************************/
int PackEE(unsigned char bank)
{
    int currentPage;        //Array row (PM page) of active DEE page
    int packedPage;         //Array row (PM page) of packed page
    int savedTBLPAG;        //Context save of TBLPAG value. Current and packed page are on same page.
    int currentOffset;      //Current page offset
    int packedOffset;       //Packed page offset
    int i;
    unsigned char latchAddr;
    unsigned int latchData;
    unsigned char dataEEFlags_sh;

    savedTBLPAG = TBLPAG;

    // Find the active page.
    for (currentPage = 0;
         (currentPage < NUM_DATA_EE_PAGES) &&
         (GetPageStatus(bank, currentPage, STATUS_CURRENT) == PAGE_NOT_CURRENT);
         currentPage++) {}


    if (currentPage == NUM_DATA_EE_PAGES)
    {
        TBLPAG = savedTBLPAG;
        SetPagePackBeforeInit(1);
        return(3);      // Error - no active page
    }
    else
    {
        // Find the next unexpired page to use
        packedPage = currentPage + 1;
        if (packedPage == NUM_DATA_EE_PAGES)
        {
            packedPage = 0;
        }
        while(GetPageStatus(bank, packedPage, STATUS_EXPIRED) == PAGE_EXPIRED)
        {
            packedPage++;
            if (packedPage == NUM_DATA_EE_PAGES)
            {
                packedPage = 0;
            }
            if(packedPage == currentPage)
            {
                TBLPAG = savedTBLPAG;
                SetPageExpiredPage(1);
                return(1);      // Error - all pages expired
            }
        }
    }

    // Point to first location in packed page
    TBLPAG = DEE_PAGE_TBL(bank, packedPage);
    packedOffset = DEE_PAGE_OFFSET(bank, packedPage);

    if(GetNextAvailCount(bank))
    {
        SetPagePackBeforePageFull(1);           // Pack called before the page was full
    }

    dataEEFlags_sh = dataEEFlags.val;
    SetaddrNotFound(0);                 // Initialize flag
    i = 0;
    NVMCON = PROGRAM_ROW;

    WritePMLow(0xFFFF, packedOffset);
    WritePMHigh(0xFF, packedOffset);
    packedOffset += 2;

    latchAddr = 0;
    i++;

    do
    {
        while((latchAddr != DATA_EE_SIZE) && (i < NUMBER_OF_INSTRUCTIONS_IN_ROW))
        {
            latchData = DataEERead((255 * bank) + latchAddr);
            if(GetaddrNotFound())       //if address is unwritten, skip to next address
            {
                SetaddrNotFound(0);
            }
            else
            {
                WritePMLow(latchData, packedOffset);
                WritePMHigh(latchAddr, packedOffset);
                packedOffset += 2;
                i++;
            }
            latchAddr++;
            while((latchAddr == DATA_EE_SIZE) && (i < NUMBER_OF_INSTRUCTIONS_IN_ROW))
            {
                WritePMLow(0xFFFF, packedOffset);
                WritePMHigh(0xFF, packedOffset);
                packedOffset += 2;
                i++;
            }
        }
        UnlockWrite();
        i = 0;
    }
    while(latchAddr != DATA_EE_SIZE);

    dataEEFlags.val = dataEEFlags_sh;   //Restore status flags

    //Verify data was written correctly into packed page

    // Point to first location after status
    TBLPAG = DEE_PAGE_TBL(bank, packedPage);
    packedOffset = DEE_PAGE_OFFSET(bank, packedPage) + 2;

    latchAddr = ReadPMHigh(packedOffset++);
    latchData = ReadPMLow(packedOffset++);

    while(latchAddr != 0xFF)
    {
        if(DataEERead((255 * bank) + latchAddr) != latchData)
        {
            TBLPAG = savedTBLPAG;
            SetPageWriteError(1);
            return(7);          //Error - data does not match
        }
       latchAddr = ReadPMHigh(packedOffset++);
       latchData = ReadPMLow(packedOffset++);
    }


    //Program page status
    currentOffset = DEE_PAGE_OFFSET(bank, currentPage);
    packedOffset = DEE_PAGE_OFFSET(bank, packedPage);

    // Point to proper TBLPAG
    TBLPAG = DEE_PAGE_TBL(bank, currentPage);
    latchData = ReadPMLow(currentOffset);
    latchAddr = ReadPMHigh(currentOffset);
    if(packedPage == 0)
    {
        latchData++;        //Increment E/W counter
    }

    if (latchData >= ERASE_WRITE_CYCLE_MAX - 1)
    {
        SetPageExpiredPage(1);
        latchAddr &= 0b11101111;
    }

    // Point to proper TBLPAG
    TBLPAG = DEE_PAGE_TBL(bank, packedPage);
    WritePMHigh(latchAddr, packedOffset);
    WritePMLow(latchData, packedOffset);

    NVMCON = PROGRAM_WORD;
    UnlockWrite();

    if((latchAddr != ReadPMHigh(packedOffset)) ||
        (latchData != ReadPMLow(packedOffset)))
    {
        TBLPAG = savedTBLPAG;
        SetPageWriteError(1);
        return(7);
    }

    //Erase active page
    ErasePage(bank, currentPage);

    TBLPAG = savedTBLPAG;
    return(GetPageExpiredPage());
}

/************************************************************************
DataEEInit

This routine finds an unexpired page to become an active page. It then
counts the number of active pages. If no active pages are found, the
first unexpired page is initialized for emulation. If one active page is
found, it is assumes a reset occurred and the function does nothing. If
two active pages are found, it is assumes a reset occurred during a pack.
The second page is erased and a pack is called. If three, an error code
is returned as the allocated memory is assumed to be corrupted. This
function must be called prior to any other operation.

Parameters:		None
Return:			Status value (0 for pass)
Side Effects:	Data EE flags may be updated.
************************************************************************/
unsigned char DataEEInit(void)
{
    unsigned char pageCnt;
    unsigned char erasePage;
    unsigned int savedTBLPAG;        //Context save of TBLPAG value. Current and packed page are on same page.
    unsigned int currentPage;
    unsigned int statusOffset;
    int packedPage;         //Array row (PM page) of packed page
    unsigned char bank;

    savedTBLPAG = TBLPAG;

    // Point the table page pointer to the emulation pages
    TBLPAG = DEE_PAGE_TBL(0, 0);

    for(bank = 0; bank < DATA_EE_BANKS; bank++)
    {
        pageCnt = 0;
        erasePage = 0;
        packedPage = 0;

        // Find unexpired page
        for (currentPage = 0;
            (currentPage < NUM_DATA_EE_PAGES) &&
            (GetPageStatus(bank, currentPage, STATUS_EXPIRED) == PAGE_EXPIRED);
            currentPage++) {}

        if (currentPage == NUM_DATA_EE_PAGES)
        {
            TBLPAG = savedTBLPAG;
            SetPageExpiredPage(1);
            return(1);     // Error - All pages expired
        }

        // Count active page(s).
        for (currentPage = 0; currentPage < NUM_DATA_EE_PAGES; currentPage++)
        {
            if(GetPageStatus(bank, currentPage, STATUS_CURRENT) == PAGE_CURRENT)
            {
                pageCnt++;
            }
        }

        //If no active pages found, initialize page 0
        if(pageCnt == 0)
        {
            ErasePage(bank, 0);

            // Point to proper TBLPAG and offset
            TBLPAG = DEE_PAGE_TBL(bank, 0);
            statusOffset = DEE_PAGE_OFFSET(bank, 0);

            NVMCON = PROGRAM_WORD;

            WritePMLow(0, statusOffset);    //New page: unavailable, active, reset count
            WritePMHigh(0xF3, statusOffset);
            UnlockWrite();

            TBLPAG = savedTBLPAG;
            continue;
        }
        //If one active page, do nothing
        else if(pageCnt == 1)
        {
            TBLPAG = savedTBLPAG;
            continue;
        }
        //If two active pages, erase second and repack first
        else if(pageCnt == 2)
        {
            if((GetPageStatus(bank, NUM_DATA_EE_PAGES - 1, STATUS_CURRENT) == PAGE_CURRENT) &&
                (GetPageStatus(bank, 0, STATUS_CURRENT) == PAGE_CURRENT))
            {
                currentPage = NUM_DATA_EE_PAGES - 1;
                erasePage = 0;
            }
            else
            {
                currentPage = 0;
                while((GetPageStatus(bank, currentPage, STATUS_CURRENT) == PAGE_NOT_CURRENT) &&
                    (currentPage < NUM_DATA_EE_PAGES))
                {
                    currentPage++;
                }
                erasePage = currentPage + 1;
                if (erasePage == NUM_DATA_EE_PAGES)
                {
                    erasePage = 0;
                }
            }
            ErasePage(bank, erasePage);

            if(!GetNextAvailCount(bank))
            {
                PackEE(bank);
            }
            TBLPAG = savedTBLPAG;
            continue;
        }
        else
        {
            TBLPAG = savedTBLPAG;
            SetPageCorruptStatus(1);
            return(6);
        }
    }
    return(0);
}

/************************************************************************
DataEERead

This routine verifies the address is valid. If not, the Illegal Address
flag is set and 0xFFFF is returned. It then finds the active page. If an
active page can not be found, the Page Corrupt status bit is set and
0xFFFF is returned. A reverse search of the active page attempts to find
the matching address in the program memory MSB (odd address). If a match
is found, the corresponding data EEPROM data (even address) is returned,
otherwise 0xFFFF is returned. This function can be called by the user.

Parameters:		Data EE address
Return:			Data EE data or 0xFFFF if address not found
Side Effects:	Data EE flags may be updated.
************************************************************************/
unsigned int DataEERead(unsigned int addr)
{
    unsigned int savedTBLPAG;        //Context save of TBLPAG value. Current and packed page are on same page.
    unsigned int currentPage;
    unsigned int pmOffset;           //Current array (page) offset of selected element (PM 16-bit word)
    unsigned int latch;
    unsigned int i;
    unsigned char bank;

    if(addr >= DATA_EE_TOTAL_SIZE)
    {
        SetPageIllegalAddress(1);
        return(0xFFFF);
    }

    bank = addr / DATA_EE_SIZE;

    savedTBLPAG = TBLPAG;

    // Find the active page.
    for (currentPage = 0;
         (currentPage < NUM_DATA_EE_PAGES) &&
         (GetPageStatus(bank, currentPage, STATUS_CURRENT) == PAGE_NOT_CURRENT);
         currentPage++) {}

    if (currentPage == NUM_DATA_EE_PAGES)
    {
        TBLPAG = savedTBLPAG;
        SetPageCorruptStatus(1);
        return(0xFFFF);     // Error - no active page
    }

    // Point to proper TBLPAG and offset
    TBLPAG = DEE_PAGE_TBL(bank, currentPage);
    pmOffset = DEE_PAGE_OFFSET(bank, (currentPage + 1)) - 2;

    i=NUMBER_OF_INSTRUCTIONS_IN_PAGE;

    do
    {
        latch = ReadPMHigh(pmOffset);
        pmOffset -= 2;

        i--;
    }
    while((i > 0) && (latch != (addr % DATA_EE_SIZE)));

    if(!i)
    {
        SetaddrNotFound(1);
        TBLPAG = savedTBLPAG;
        return(0xFFFF);
    }

    pmOffset += 2;
    latch = ReadPMLow(pmOffset);

    TBLPAG = savedTBLPAG;
    return(latch);
}

/************************************************************************
DataEEWrite

This routine verifies the address is valid. If not, the Illegal Address
flag is set and an error code is returned. It then finds the active page.
If an active page can not be found, the Page Corrupt status bit is set
and an error code is returned. A read is performed, if the data was not
changed, the function exits. If the last location is programmed, the Pack
Skipped error flag is set (one location should always be available). The
data EE information (MSB = address, LSW = data) is programmed and
verified. If the verify fails, the Write Error flag is set. If the write
went into the last location of the page, pack is called. This function
can be called by the user.

Parameters:		Data EE address and data
Return:			Pass or fail status (0 = Pass)
Side Effects:	Data EE flags may be updated. CPU stall occurs for flash
                programming. Pack may be generated.
************************************************************************/
unsigned char DataEEWrite(unsigned int data, unsigned int addr)
{
    int savedTBLPAG;        //Context save of TBLPAG value. Current and packed page are on same page.
    int currentPage;
    int pmOffset;           //Current array (page) offset of selected element (PM 16-bit word)
    unsigned int nextLoc;
    volatile unsigned char latch;
    unsigned char dataEEFlags_sh;
    unsigned int bank;

    if(addr >= DATA_EE_TOTAL_SIZE)
    {
        SetPageIllegalAddress(1);
        return(5);
    }

    bank = addr / DATA_EE_SIZE;

    savedTBLPAG = TBLPAG;
    NVMCON = PROGRAM_WORD;

    // Find the active page.
    for (currentPage = 0;
         (currentPage < NUM_DATA_EE_PAGES) &&
         (GetPageStatus(bank, currentPage, STATUS_CURRENT) == PAGE_NOT_CURRENT);
         currentPage++) {}

    if (currentPage == NUM_DATA_EE_PAGES)
    {
        TBLPAG = savedTBLPAG;
        SetPageCorruptStatus(1);
        return(6);      // Error - no active page
    }

    // Point to proper TBLPAG and offset
    TBLPAG = DEE_PAGE_TBL(bank, currentPage);
    pmOffset = DEE_PAGE_OFFSET(bank, currentPage);

    dataEEFlags_sh = dataEEFlags.val;

    //Do not write data if it did not change
    if(DataEERead(addr) == data)
    {
        if(GetaddrNotFound() == 0) // Check if the read was successful
        {
            TBLPAG = savedTBLPAG;
            dataEEFlags.val = dataEEFlags_sh;
            return(0);
        }
    }

    dataEEFlags.val = dataEEFlags_sh;       //Restore status flags
    nextLoc = GetNextAvailCount(bank);

    if(!nextLoc)
    {
        TBLPAG = savedTBLPAG;
        SetPagePackSkipped(1);
        return(4);  //Error - Number of writes exceeds page size
    }

    pmOffset = pmOffset + nextLoc;

    WritePMLow(data, pmOffset);
    WritePMHigh((addr % DATA_EE_SIZE), pmOffset);

    UnlockWrite();

    Nop();
    Nop();

    latch = (ReadPMLow(pmOffset) & 0xFF);

    if(latch != (data & 0xFF))
    {
        TBLPAG = savedTBLPAG;
        SetPageWriteError(1);
        return(7);  //Error - RAM does not match PM
    }

    latch = (ReadPMHigh(pmOffset) & 0xFF);

    if(latch != (addr % DATA_EE_SIZE))
    {
        TBLPAG = savedTBLPAG;
        SetPageWriteError(1);
        return(7);  //Error - RAM does not match PM
    }

    pmOffset += 1;
    latch = ((ReadPMLow(pmOffset) >> 8) & 0xFF);

    if(latch != ((data >> 8) & 0xFF))
    {
        TBLPAG = savedTBLPAG;
        SetPageWriteError(1);
        return(7);  //Error - RAM does not match PM
    }

    //Pack if page is full
    if ((nextLoc + 2) == ((NUMBER_OF_INSTRUCTIONS_IN_PAGE) * 2))
    {
        PackEE(bank);
    }

    TBLPAG = savedTBLPAG;

    return(0);
}
