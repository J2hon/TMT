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




void Load_Cell_Check(void)
{
	unsigned char 	x_key=0;
	unsigned char	key_cnt=0;
	unsigned long 	temp_long=0;
	volatile unsigned long 	temp_long_tmp=0;

	
	ex_main_first_key_f=0;

	
	if( (ep_zero==0)||(ep_zero>1000000l) )
	{
		Single_Calibration_Mode(); 
	}

	ex_first_zero = ex_adc_org = (unsigned long) ( ((float) ep_zero) * ep_res_factor );
    if(ep_zero_range_value <= 10)
    {
        FirstZeroPlusLimit = (ex_first_zero + (ep_e_resolution/5)*10);
        if(ex_first_zero >= ((ep_e_resolution/5)*10)) FirstZeroMinusLimit = (ex_first_zero - (ep_e_resolution/5)*10);
        else FirstZeroMinusLimit = 0;
    }
    else
    {
        FirstZeroPlusLimit = (ex_first_zero + (ep_e_resolution/5)*ep_zero_range_value);
        if(ex_first_zero >= ((ep_e_resolution/5)*10)) FirstZeroMinusLimit = (ex_first_zero - (ep_e_resolution/5)*ep_zero_range_value);
        else FirstZeroMinusLimit = 0;
    }

	while(1)
	{
		do 
		{
			temp_long = read_filtered_adc();
		}	
		while(ex_ad_flag==0);

		if( (temp_long < 100) || (temp_long > 1000000l) )	// ad error
		{ 
			String_Display(WEIGHT, "CH 02");
			String_Display(UNIT_PRICE, "     ");
			String_Display(PAY, "      ");
			String_Display(PLU, "  ");
			Low_Batt_check();

			if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
			{
				x_key=Key_Data_Read();
				switch(x_key)
				{
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
		else
		{
			do 
			{
				temp_long_tmp = read_filtered_adc(); 
			}while(ex_ad_flag==0);
			
			//ep_zero = temp_long_tmp;
			temp_long_tmp = (unsigned long) (((float) temp_long_tmp) * ep_res_factor);
			
			if((temp_long_tmp > (ex_first_zero + ep_e_resolution/5*10)) && !ex_calib_trace_flag && !ep_wei_backup) 		// +10%
			{ 
				tone_type=FOUR_TONE_PLAY;
			
				String_Display(WEIGHT, "CH 06");
				String_Display(UNIT_PRICE, "     ");
				String_Display(PAY, "      ");
				String_Display(PLU, "  ");
				while(1)
				{
                    do temp_long_tmp = read_filtered_adc(); while(ex_ad_flag==0);
                    temp_long_tmp = (unsigned long) (((float) temp_long_tmp) * ep_res_factor);
                    if(temp_long_tmp <= (ex_first_zero + ep_e_resolution/5*10)) 
                    {
                        DelayMs(200);
                        do temp_long = read_filtered_adc(); while(ex_ad_flag==0);
                        break;
                    }
					if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
					{
						x_key=Key_Data_Read();
						switch(x_key) 
						{
                            case ENT_K: 						//	1번 키 
								goto MESCP;
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
            ex_adc_org = (unsigned long) ( ((float) temp_long) * ep_res_factor );
            ep_tare_value = ex_adc_org;
            PowerOnZero = ex_adc_org;
            ep_tare_flag=0;
			break;
		}
	}  
MESCP:
	if( (ep_wei_backup==0) && (ex_calib_trace_flag==0) )
	{
		ex_adc_org = (unsigned long) ( ((float) temp_long) * ep_res_factor );
		//ex_first_zero = ex_adc_org;
		ep_tare_value = ex_adc_org;
        PowerOnZero = ex_adc_org;
		ep_tare_flag=0;
	}
    else
    {
        ex_adc_org = ex_first_zero;
        //ex_adc_org = ex_first_zero + ep_e_resolution;
        ep_tare_value = ex_adc_org;
        PowerOnZero = ex_adc_org;
    }
	ex_calib_trace_flag=0;
}


void Initial_PWM(void)
{
	RPOR9bits.RP18R     = 18; 			// PWM OC1
	OC1CON1bits.OCSIDL = 0;  			// ¾Æ???? ¸ð??½?¿¡?? ?¿?? ¼³?¤
	OC1CON1bits.OCFLT  = 0;  			// PWM Fault ?? ??·? ??¿? º?°¡´? ¼³?¤
	OC1CON1bits.OCTSEL = 0;  			// ?¸??¸? 2 ¼³?¤
	OC1CON1bits.OCM    = 0;  			// OUTPUT OFF
	OC1R = (ep_bright+1)*16/10;
}


void Initial_Timer2( void )
{
	IFS0bits.T2IF = 0;			// Initialize timer2 interrupt flag
	IEC0bits.T2IE = 1;			// Enable timer2

	T2CON = 0x8020; 			// Prescaler 64:1, tcy = Fosc / 2 = 8MHz / 2 = 4MHz
	PR2 = 31;					// 4MHz = 250ns, 250*64(prescaler) = 16us, 16us*63 = 1008us, 1008us/4=252us
}



void io_output_init(void)
{
	// io_output_init
	sbi(LATB, BUZZER); // Buzzer off

	BackLight_Off;  // Buzzer off

	sbi(LATB, POWER_ON);
	sbi(LATB, POWER_SW);

	cbi(LATG, 2);

}

void io_analog_init(void)
{
	// io_analog_init
	ANSB = 0x0000;
	ANSB &= ~(1<<4);	// BUZZER
	ANSB |= (1<<5);		// LOW BATT
	ANSB &= ~(1<<5);	// BL
	ANSB &= ~(1<<12);	// SW
	ANSB &= ~(1<<13);	// SW
	ANSB &= ~(1<<14);	// SW
	ANSB &= ~(1<<15);	// SW

	
	ANSD  = 0x0000; 	// Digital Port D
	ANSD &= ~(1<<3);	// LCD
	ANSD &= ~(1<<4);	// LCD
	ANSD &= ~(1<<5);	// LCD
	ANSD &= ~(1<<6);	// LCD
	ANSD &= ~(1<<7);	// LCD
	ANSD &= ~(1<<8);	// LCD

	ANSE = 0x0000;		// Digital Port E
	ANSE &= ~(1<<0);	// SW
	ANSE &= ~(1<<1);	// SW
	ANSE &= ~(1<<2);	// SW
	ANSE &= ~(1<<3);	// SW
	ANSE &= ~(1<<4);	// SW
	ANSE &= ~(1<<5);	// SW
	ANSE &= ~(1<<6);	// SW
	ANSE &= ~(1<<7);	// SW

	
	ANSG  = 0x0000; // Digital Port G

}

void io_direction_init(void)
{
	// io_direction
	// init port       		15	14	13	12	11	10	9	8	7	6	5	4	3	2	1	0
	TRISB = 0xF984;	// 	1	1	1	1 	1	0	0	1 	1	0	0	0	0	1	0	0
	#if 0
	TRISBbits.TRISB0 = OUT;	// PGD_1
	TRISBbits.TRISB1 = OUT; // PGC_1
	TRISBbits.TRISB2 =  IN;	// PSW
	TRISBbits.TRISB3 = OUT;	// POFF
	TRISBbits.TRISB4 = OUT;	// BUZZER
	//TRISBbits.TRISB5 =  IN;	// BATT_CHECK
	TRISBbits.TRISB5 = OUT; // BACK LIGHT

	TRISBbits.TRISB6 = OUT;	// TXD_1
	TRISBbits.TRISB7 =  IN;	// RXD_1
	TRISBbits.TRISB8 = IN;  // RXD_2
	TRISBbits.TRISB9 = OUT;	// TXD_2
	TRISBbits.TRISB10= OUT;	// NC
	TRISBbits.TRISB11=  IN;	// NC
	TRISBbits.TRISB12=  IN;	// SW_8
	TRISBbits.TRISB13=  IN;	// SW_9
	TRISBbits.TRISB14=  IN;	// SW_10
	TRISBbits.TRISB15=  IN;	// CAL_K
	#endif 

	
	TRISC = 0x0000; 
	#if 0
	// 15=out, 14=out, 13=IN, 12=OUT
	//TRISC = 0x2000; 
	// io_direction
	// init port       		15	14	13	12	11	10	9	8	7	6	5	4	3	2	1	0
	//					1	0	0	1	0	0	0	0	0	0	0	0	0	0	0	0
	//TRISCbits.TRISC0 =
	//TRISCbits.TRISC1 =
	//TRISCbits.TRISC2 =
	//TRISCbits.TRISC3 =
	//TRISCbits.TRISC4 =
	//TRISCbits.TRISC5 =
	//TRISCbits.TRISC6 =
	//TRISCbits.TRISC7 =
	//TRISCbits.TRISC8 =
	//TRISCbits.TRISC9 =
	//TRISCbits.TRISC10=
	//TRISCbits.TRISC11=
	TRISCbits.TRISC12=  IN;	// NC
	//TRISCbits.TRISC13= IN;	// SOSCI
	//TRISCbits.TRISC14= OUT;	// SOSCO
	TRISCbits.TRISC15=  IN;	// NC
	#endif 
	

	TRISD = 0x0E1C; 	
	#if 0
											// 0 	0	0	0	       1	1	1	0	       0	0	0	1	       1	1	0	0
											//
	TRISDbits.TRISD0 = OUT;	// BOOT1
	TRISDbits.TRISD1 = OUT;	// TXD3
	TRISDbits.TRISD2 =  IN;	// RXD3
	TRISDbits.TRISD3 =  IN;	// LCD_IRQ_1
	TRISDbits.TRISD4 =  IN;	// LCD_IRQ_2
	TRISDbits.TRISD5 = OUT;	// LCD_CS_1
	TRISDbits.TRISD6 = OUT;	// LCD_CS_2
	TRISDbits.TRISD7 = OUT;	// LCD_WR
	TRISDbits.TRISD8 = OUT;	// LCD_DATA
	TRISDbits.TRISD9 =  IN;	// IR_SIG
	TRISDbits.TRISD10=  IN;	// Z_RESET
	TRISDbits.TRISD11=  IN;	// KEY_IRQ
	//TRISDbits.TRISD12=
	//TRISDbits.TRISD13=
	//TRISDbits.TRISD14=
	//TRISDbits.TRISD15=
	#endif 


	TRISE = 0x00FF; // 0	0	0	0	0	0	0	0	1	1	1	1	1	1	1	1
	TRISEbits.TRISE0 = IN;	// SW_0
	TRISEbits.TRISE1 = IN;	// SW_1
	TRISEbits.TRISE2 = IN;	// SW_2
	TRISEbits.TRISE3 = IN;	// SW_3
	TRISEbits.TRISE4 = IN;	// SW_4
	TRISEbits.TRISE5 = IN;	// SW_5
	TRISEbits.TRISE6 = IN;	// SW_6
	TRISEbits.TRISE7 = IN;	// SW_7
	//TRISEbits.TRISE8 =
	//TRISEbits.TRISE9 =
	//TRISEbits.TRISE10=
	//TRISEbits.TRISE11=
	//TRISEbits.TRISE12=
	//TRISEbits.TRISE13=
	//TRISEbits.TRISE14=
	//TRISEbits.TRISE15=
		
					//										SCK	MISOMOSI		CS
					//	15	14	13	12	11	10	9	8	7	6	5	4	3	2	1	0
	TRISF = 0x0020;	//										0	1	0			0
	TRISFbits.TRISF0 =  OUT;	// NC
	TRISFbits.TRISF1 = OUT;	// FLASH_CS
	TRISFbits.TRISF2 =  IN;	// RXD_4
	TRISFbits.TRISF3 = OUT;	// TXD_4
	TRISFbits.TRISF4 = OUT;	// FLASH_MOSI
	TRISFbits.TRISF5 =  IN;	// FLASH_MISO
	TRISFbits.TRISF6 = OUT;	// FLASH_SCK
	//TRISFbits.TRISF7 =
	//TRISFbits.TRISF8 =
	//TRISFbits.TRISF9 =
	//TRISFbits.TRISF10=
	//TRISFbits.TRISF11=
	//TRISFbits.TRISF12=
	//TRISFbits.TRISF13=
	//TRISFbits.TRISF14=
	//TRISFbits.TRISF15=

					// 15	14	13	12	11	10	9	8	7	6	5	4	3	2	1	0
	TRISG = 0x0080; // 0	0	0	0	0	0	0	0	1	0	0	0	0	0	0	0
	//TRISGbits.TRISG0 =
	//TRISGbits.TRISG1 =
	//TRISGbits.TRISG2 =  IN;	// NC
	//TRISGbits.TRISG3 = OUT;	// WAKEUP1
	//TRISGbits.TRISG4 =
	//TRISGbits.TRISG5 =
	//TRISGbits.TRISG6 = OUT;	// AD_MOSI
	//TRISGbits.TRISG7 =  IN;	// AD_MISO
	//TRISGbits.TRISG8 = OUT;	// AD_SCK
	//TRISGbits.TRISG9 = OUT;	// AD_CS
	//TRISGbits.TRISG10=
	//TRISGbits.TRISG11=
	//TRISGbits.TRISG12=
	//TRISGbits.TRISG13=
	//TRISGbits.TRISG14=
	//TRISGbits.TRISG15=
}



void DelayMs(volatile unsigned long Time)
{ 
	volatile unsigned short		i=0; 
	volatile unsigned short		j=0; 

	for(i=0; i<=Time; i++) 
	{ 
		asm("nop"); 
		for(j=0; j<=534; j++)   //1ms
		{ 
			asm("nop"); 
			asm("nop"); 
		} 
	}
}

void Init_Device(void)
{
	ex_main_first_key_f=1;
	RCONbits.SWDTEN=0;
	io_direction_init();
	io_analog_init();
	io_output_init();
	Init_Uart_1();
	Initial_UART2();
	Initial_UART3();
	Initial_UART4();

	BackLight_Off;
	lcd_initial();

	Initial_PWM();
	Initial_Timer2();
	adc_initial();
	Initial_IC();
	MCU_ADC_INIT();
	
    DataEEInit();
	dataEEFlags.val = 0; // Initialize EEPROM
	DelayMs(10);
	eeprom_read();
	cond_check();

	poff_sig = 0;


	while( ((PORTB&(0x01<<POWER_SW))== BV(POWER_SW))==0 );

	//poff_sig=1;

	RCONbits.SWDTEN=1;

	
	ex_zero_flag=0;


	atflash_init();

	ex_atflash_PluNum=Atflash_PluData_Pop(PLU_CUR, PLU_CUR_BUF);
	
	if(ex_atflash_PluNum<50)	ex_atflash_PluReadValue=Atflash_PluData_Pop(PLU_U_P_HEAD, (ex_atflash_PluNum*4) );
	else						ex_atflash_PluReadValue=Atflash_PluData_Pop(PLU_U_P_TAIL, ( (ex_atflash_PluNum-50)*4) );

    if(ex_atflash_PluReadValue > 99999) ex_atflash_PluReadValue = 0;
	ex_atflash_PluWriteValue=ex_atflash_PluReadValue;

	
	ex_serial_no=(unsigned short)Atflash_PluData_Pop( PAGE_TEST, PLU_CUR_BUF );

	tone_type=SHORT_TONE_PLAY;

	RTCCInit();
	batt_check_sig=1;
	CLKDIV = 0x0000;	// 이걸 안넣으면 페어링 안됨 
}


/*********************************************************************************************/
// Tone Generator
void Tone_Generation(void)
{
	switch(tone_type) 
	{
		case 0x01: 												// 띠 (단음)
			BuzzerOn;											// Buzzer On
			
			buzzer_play_timer=SHORT_TONE_TIME;				
			tone_type=0xFE;	

			sbi(LATG, 2);

			break;

		case 0x02: 												// 띠~(장음)
			BuzzerOn;											// Buzzer On

			buzzer_play_timer=LONG_TONE_TIME;				
			tone_type=0xFE;

			sbi(LATG, 2);

			break;

		case 0x10: 												// 띠-띠-띠-띠 (4회)		
			BuzzerOn;											// Buzzer On

			buzzer_play_timer=SHORT_TONE_TIME;				
			tone_type++;

			sbi(LATG, 2);

			break;

		case 0x11: 
			if(buzzer_play_timer==0) 
			{
				BuzzerOff;										// Buzzer Off
				buzzer_play_timer=SHORT_TONE_TIME;				
				tone_type++;

				cbi(LATG, 2);
				
			}
			break;

		case 0x12: 
			if(buzzer_play_timer==0) 
			{
				BuzzerOn;										// Buzzer On

				buzzer_play_timer=SHORT_TONE_TIME;				
				tone_type++;							
			}

			sbi(LATG, 2);

			
			break;

		case 0x13: 
			if(buzzer_play_timer==0) 
			{
				BuzzerOff;										// Buzzer Off
				buzzer_play_timer=SHORT_TONE_TIME;				
				tone_type++;
			}

			cbi(LATG, 2);

			
			break;

			
		case 0x14: 
			if(buzzer_play_timer==0) 
			{
				BuzzerOn;										// Buzzer On

				buzzer_play_timer=SHORT_TONE_TIME;				
				tone_type++;							
			}

			sbi(LATG, 2);


			
			break;

		case 0x15: 
			if(buzzer_play_timer==0)
			{
				BuzzerOff;										// Buzzer Off
				buzzer_play_timer=SHORT_TONE_TIME;				
				tone_type++;
			}


			cbi(LATG, 2);

			
			break;

		case 0x16: 
			if(buzzer_play_timer==0) 
			{
				BuzzerOn;										// Buzzer On

				buzzer_play_timer=SHORT_TONE_TIME;				
				tone_type=0xFE;							
			}

			sbi(LATG, 2);

			
			break;

		case 0xFE: 										
			if(buzzer_play_timer==0) 
			{											
				BuzzerOff;										// Buzzer Off
				tone_type=0;						
			}	


			cbi(LATG, 2);

			
			break;										

		case 0xFF: 										
			if(buzzer_play_timer==0) 
			{
				tone_type++;
			}

			cbi(LATG, 2);

			
			break;

		case 0x00: 
			__asm("NOP");

			break;

		default: 
			tone_type=0;
			break;
	}	
}

void short_u_p_input(void)
{
	weight_display(0);
	ex_atflash_PluNum=0;
	ex_atflash_PluWriteValue=key_task_number;
	unit_price_display(ex_atflash_PluWriteValue);
	Atflash_PluData_Push(PLU_U_P_HEAD, (ex_atflash_PluNum*4), ex_atflash_PluWriteValue);
	Atflash_PluData_Push(PLU_CUR, 200, ex_atflash_PluNum);
	key_task_number=0;
	key_task_key_f=0;
	
#if 0
	String_Display(PAY, "P UNIT");
	String_Display(PLU, " 0");
	weight_display(0);
	String_Display(WEIGHT, "INPUT");
	
	ex_atflash_PluNum=0;

	ex_atflash_PluReadValue=Atflash_PluData_Pop( PLU_U_P_HEAD, (ex_atflash_PluNum*4) );

	unit_price_display(ex_atflash_PluReadValue);
	ex_atflash_PluWriteValue=Input_U_P_Number_Long(ex_atflash_PluReadValue);

	unit_price_display(ex_atflash_PluWriteValue);
	Atflash_PluData_Push(PLU_U_P_HEAD, (ex_atflash_PluNum*4), ex_atflash_PluWriteValue);

	Atflash_PluData_Push(PLU_CUR, 200, ex_atflash_PluNum);
#endif 
}



void long_u_p_input(void)
{
	unsigned char x_key;


	LAMP_LCD3_K_OFF;
	LAMP_LCD3_G_OFF;
	LAMP_LCD3_STABLE_OFF;
	LAMP_LCD3_ZERO_OFF;



	tone_type=SHORT_TONE_PLAY;									

	String_Display(PAY, "PLU NO");

	if(ex_atflash_PluNum<50)	
		ex_atflash_PluReadValue=Atflash_PluData_Pop(PLU_U_P_HEAD, (ex_atflash_PluNum*4) );
	else				
		ex_atflash_PluReadValue=Atflash_PluData_Pop(PLU_U_P_TAIL, ( (ex_atflash_PluNum-50)*4) );

    if(ex_atflash_PluReadValue > 99999) ex_atflash_PluReadValue = 0;
    
	unit_price_display(ex_atflash_PluReadValue);
	weight_display(0);


	// STEP_01 -> Input Plu No
	ex_atflash_PluNum = Input_Plu_Number_Char( ex_atflash_PluNum );

	if(ex_atflash_PluNum<50)	
		ex_atflash_PluReadValue=Atflash_PluData_Pop(PLU_U_P_HEAD, (ex_atflash_PluNum*4) );
	else				
		ex_atflash_PluReadValue=Atflash_PluData_Pop(PLU_U_P_TAIL, ( (ex_atflash_PluNum-50)*4) );

    if(ex_atflash_PluReadValue > 99999) ex_atflash_PluReadValue = 0;
	plu_display( ex_atflash_PluNum );



	// STEP 02 -> Input unit_price value
	String_Display(PAY, "P UNIT");
	weight_display(0);
	ex_atflash_PluWriteValue=Input_U_P_Number_Long(ex_atflash_PluReadValue);


	unit_price_display(ex_atflash_PluWriteValue);

	if(ex_atflash_PluNum<50)	
		Atflash_PluData_Push(PLU_U_P_HEAD, (ex_atflash_PluNum*4), ex_atflash_PluWriteValue);
	else					
		Atflash_PluData_Push(PLU_U_P_TAIL, ( (ex_atflash_PluNum-50)*4 ), ex_atflash_PluWriteValue );

	Atflash_PluData_Push(PLU_CUR, 200, ex_atflash_PluNum);
#if 0
	tone_type=SHORT_TONE_PLAY;									
	ex_no_weight_flag=0;
	short_u_p_input();
	ex_no_weight_flag=0;
#endif 
}

void short_call_input(void)
{
	// STEP_01 -> Input Plu No
	if(key_task_number>99)
	{
		key_task_key_f=0;
		key_task_number=0;
		tone_type=FOUR_TONE_PLAY;									
		return;
	}
	ex_atflash_PluNum = (unsigned char)key_task_number;
	plu_display( ex_atflash_PluNum );


	// STEP 02 -> Displayed unit_price value
	if(ex_atflash_PluNum<50)	
		ex_atflash_PluReadValue=Atflash_PluData_Pop(PLU_U_P_HEAD, (ex_atflash_PluNum*4) );
	else				
		ex_atflash_PluReadValue=Atflash_PluData_Pop(PLU_U_P_TAIL, ( (ex_atflash_PluNum-50)*4) );
    
    if(ex_atflash_PluReadValue > 99999) ex_atflash_PluReadValue = 0;
	ex_atflash_PluWriteValue=ex_atflash_PluReadValue;
	unit_price_display(ex_atflash_PluWriteValue);

	Atflash_PluData_Push(PLU_CUR, 200, ex_atflash_PluNum);

	key_task_key_f=0;
	key_task_number=0;
#if 0
	String_Display(PAY, "PLU NO");
	String_Display(UNIT_PRICE, "     ");

	// STEP_01 -> Input Plu No
	ex_atflash_PluNum = Input_Plu_Number_Char( ex_atflash_PluNum );
	plu_display( ex_atflash_PluNum );


	// STEP 02 -> Displayed unit_price value
	if(ex_atflash_PluNum<50)	
		ex_atflash_PluReadValue=Atflash_PluData_Pop(PLU_U_P_HEAD, (ex_atflash_PluNum*4) );
	else				
		ex_atflash_PluReadValue=Atflash_PluData_Pop(PLU_U_P_TAIL, ( (ex_atflash_PluNum-50)*4) );

	ex_atflash_PluWriteValue=ex_atflash_PluReadValue;
	unit_price_display(ex_atflash_PluWriteValue);

	Atflash_PluData_Push(PLU_CUR, 200, ex_atflash_PluNum);
#endif 
}

void same_product_sum(void)
{
	unsigned char 	x_key;
	unsigned char	key_cnt=0;
	unsigned short 	input_value=1;
	unsigned short 	sum_count=0;
	unsigned char 	i=0;
	unsigned long 	ex_same_product_sum[100]={0};
	unsigned long 	same_product_sum_final_temp=0;
	unsigned long 	ex_e_value_temp=ex_e_value;
	unsigned char 	division_tmep;
	unsigned short 	temp_x;



	
	ex_same_product_sum_final=0;

	
	LAMP_LCD3_K_OFF;
	LAMP_LCD3_G_OFF;
	LAMP_LCD3_LB_OFF;
	LAMP_LCD3_STABLE_OFF;
	LAMP_LCD3_ZERO_OFF;
	weight_display(0);


RE:
	
	pay_display(ex_e_value_temp*ex_atflash_PluWriteValue, 0);	
	unit_price_display(ex_atflash_PluWriteValue);


	String_Display(PLU, "  ");
	String_Display(WEIGHT, "PC   ");

	lcd_want_display(WEIGHT, 5, 1);


	input_value=1;
	input_value = three_byte(input_value);
	print_count=input_value;
	//pay_display( ((unsigned long)input_value*ex_e_value_temp*ex_atflash_PluWriteValue),0 );
    pay_display( ((unsigned long)input_value * cut_pay), 1 );
	
	//print_total_money=((unsigned long)input_value*ex_e_value_temp*ex_atflash_PluWriteValue,0);
    print_total_money=((unsigned long)input_value * cut_pay);
    
	if(input_value>99)
	{
		String_Display(WEIGHT, "PC   ");
		lcd_want_display(WEIGHT, 3, (unsigned char)(input_value/100) );
		lcd_want_display(WEIGHT, 4, (unsigned char)((input_value%100)/10));
		lcd_want_display(WEIGHT, 5, (unsigned char)((input_value%100)%10));
	}
	else if(input_value>9)
	{
		String_Display(WEIGHT, "PC   ");
		lcd_want_display(WEIGHT, 4, (unsigned char)(input_value/10) );
		lcd_want_display(WEIGHT, 5, (unsigned char)(input_value%10) );
	}
	else if(input_value<10)
	{
		String_Display(WEIGHT, "PC   ");
		lcd_want_display(WEIGHT, 5, (unsigned char)input_value);
	}



	while(1)
	{
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			switch(x_key) 
			{
				case F3_KEY:
					tone_type=SHORT_TONE_PLAY;	
					//tf200_send(ex_e_value_temp);
					break;

				case ADD_KEY:
					tone_type=SHORT_TONE_PLAY;	
					same_product_sum_flag=1;
					weight_add();
					return;
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

unsigned long money_change(void)
{
	Input_U_P_Number_Long(0);
}















void weight_add(void)
{
	ex_weight_sum_blink_flag=1;
	ex_weight_add_tick=1;

	if(same_product_sum_flag==1)
	{
		same_product_sum_flag=0;
		ex_weight_add_value[ex_weight_add_count]=print_total_money;
	}
	else
	{
		//if(ex_weight_hold_value)	ex_weight_add_value[ex_weight_add_count]=( (ex_weight_hold_value*ex_atflash_PluWriteValue) );
		//else						ex_weight_add_value[ex_weight_add_count]=( (ex_e_value*ex_atflash_PluWriteValue) );
        ex_weight_add_value[ex_weight_add_count] = cut_pay;
	}
    /*
	if(ep_maximum_capacity == 150)ex_weight_add_value[ex_weight_add_count] = (ex_weight_add_value[ex_weight_add_count]/10)*10;  //절삭
    else if(ep_maximum_capacity == 60)ex_weight_add_value[ex_weight_add_count] = (ex_weight_add_value[ex_weight_add_count]/10)*10;
    else if(ep_maximum_capacity == 30)ex_weight_add_value[ex_weight_add_count] = (ex_weight_add_value[ex_weight_add_count]/100)*100;
    else if(ep_maximum_capacity == 15)ex_weight_add_value[ex_weight_add_count] = (ex_weight_add_value[ex_weight_add_count]/100)*100;
    */
	ex_weight_add_count++;
}


void weight_total_sum(void)
{
	unsigned char 	x_key;
	unsigned char	key_cnt=0;
	unsigned char	i=0;

	String_Display(PAY, 		"  DTP ");
	String_Display(UNIT_PRICE,	"     ");
	String_Display(PLU, 		"  ");
	LAMP_LCD3_K_OFF;
	LAMP_LCD3_G_OFF;
	LAMP_LCD3_STABLE_OFF;
	LAMP_LCD3_ZERO_OFF;
	weight_display(0);
	String_Display(WEIGHT,		"     ");




	rtc_read();

	for(i=0; i<20; i++)
	{
		ex_fw_buffer[i]=1;
		
		//	Input the (unit16 Page_Address, uint8 Buffer_Address, uint32 Buffer_Data)
		Atflash_PluData_Push(10, i*4, ex_fw_buffer[i]);
	}


	

	while(1)
	{
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			switch(x_key) 
			{
				case CLR_K:
					tone_type=SHORT_TONE_PLAY;	
					
					String_Display(PAY, 		" CLEAR");
					String_Display(UNIT_PRICE, 	"     ");
					String_Display(PLU, 		"  ");
					String_Display(WEIGHT, 	 	"     ");
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
					
					Atflash_PluData_Push( PAGE_TEST, (unsigned char)200, (unsigned long)0);
					ex_serial_no=Atflash_PluData_Pop(PAGE_TEST,(unsigned char)200);
					
					DelayMs(700);
					return;
					
					break;
				
				case SAVE_KEY:
					tone_type=SHORT_TONE_PLAY;	
					
					ex_read_total_sum=0;
					#if 0

					for(i=0; i<20; i++)
					{
						ex_fw_buffer[i]=2;
						
						//	Input the (unit16 Page_Address, uint8 Buffer_Address, uint32 Buffer_Data)
						Atflash_PluData_Push(10, i*4, ex_fw_buffer[i]);
						ex_read_total_sum += Atflash_PluData_Pop(10, i*4); 	//	1
					}
					#endif 

					#if 1
					ex_read_total_sum += Atflash_PluData_Pop(3, 0);		//	1
					ex_read_total_sum += Atflash_PluData_Pop(3, 4);		//	2
					ex_read_total_sum += Atflash_PluData_Pop(3, 8);		//	3
					ex_read_total_sum += Atflash_PluData_Pop(3, 12);	//	4
					ex_read_total_sum += Atflash_PluData_Pop(3, 16);	//	5
					ex_read_total_sum += Atflash_PluData_Pop(3, 20);	//	6
					ex_read_total_sum += Atflash_PluData_Pop(3, 24);	//	7
					ex_read_total_sum += Atflash_PluData_Pop(3, 28);	//	8
					ex_read_total_sum += Atflash_PluData_Pop(3, 32);	//	9
					ex_read_total_sum += Atflash_PluData_Pop(3, 36);	//	10
					#endif 

					pay_display(ex_read_total_sum,0);
					String_Display(UNIT_PRICE,		"DAILY");
					lcd_want_display(WEIGHT, 3, (ex_serial_no/100));
					lcd_want_display(WEIGHT, 4, (ex_serial_no%100)/10);
					lcd_want_display(WEIGHT, 5, (ex_serial_no%100)%10);
					
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


	
#if 0
	while(1)
	{
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			switch(x_key) 
			{
				case CLR_K:
					tone_type=SHORT_TONE_PLAY;	
					
					String_Display(PAY, 		" CLEAR");
					String_Display(UNIT_PRICE, 	"     ");
					String_Display(PLU, 		"  ");
					String_Display(WEIGHT, 	 	"     ");
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
					
					Atflash_PluData_Push( PAGE_TEST, (unsigned char)200, (unsigned long)0);
					ex_serial_no=Atflash_PluData_Pop(PAGE_TEST,(unsigned char)200);
					
					DelayMs(700);
					return;
					
					break;

				case SAVE_KEY:
					tone_type=SHORT_TONE_PLAY;	
					
					ex_read_total_sum=0;

					ex_read_total_sum += Atflash_PluData_Pop(3, 0);		//	1
					ex_read_total_sum += Atflash_PluData_Pop(3, 4);		//	2
					ex_read_total_sum += Atflash_PluData_Pop(3, 8);		//	3
					ex_read_total_sum += Atflash_PluData_Pop(3, 12);	//	4
					ex_read_total_sum += Atflash_PluData_Pop(3, 16);	//	5
					ex_read_total_sum += Atflash_PluData_Pop(3, 20);	//	6
					ex_read_total_sum += Atflash_PluData_Pop(3, 24);	//	7
					ex_read_total_sum += Atflash_PluData_Pop(3, 28);	//	8
					ex_read_total_sum += Atflash_PluData_Pop(3, 32);	//	9
					ex_read_total_sum += Atflash_PluData_Pop(3, 36);	//	10
					pay_display(ex_read_total_sum);
					String_Display(UNIT_PRICE, 		"DAILY");
					lcd_want_display(WEIGHT, 3, (ex_serial_no/100));
					lcd_want_display(WEIGHT, 4, (ex_serial_no%100)/10);
					lcd_want_display(WEIGHT, 5, (ex_serial_no%100)%10);
					
					break;

				case ENT_K:
					tone_type=SHORT_TONE_PLAY;	
					return;
					break;


				default:
					break;
			}
		}
	}
#endif 
}






void weight_sum(void)
{
	unsigned short i=0;

	tone_type=SHORT_TONE_PLAY;
	ex_weight_add_tick=0;
	ex_weight_sum_blink_flag=0;
	ex_weight_sum_blink_timer=0;
	ex_weight_sum_lamp_flag=1;

	ex_serial_no++;
	Atflash_PluData_Push( PAGE_TEST, (unsigned char)200, (unsigned long)ex_serial_no);
	ex_serial_no=Atflash_PluData_Pop(PAGE_TEST,(unsigned char)200);

	ex_weight_sum_value=0;
    
	for(i=0; i<500; i++)	
	{
		ex_weight_sum_value += ex_weight_add_value[i];
	}
}







void Sub_Task(void)
{
	if(ex_unit_price_input_flag==1)
	{	
		ex_unit_price_input_flag=0;
		long_u_p_input();	
	}

	if(ex_unit_price_call_flag==1)
	{
		ex_unit_price_call_flag=0;
		short_call_input();	
	}

	if(ex_same_product_flag==1)
	{
		ex_same_product_flag=0;
		same_product_sum();
	}

	if(ex_weight_add_flag==1)
	{
		ex_weight_add_flag=0;
		weight_add();
	}
	
	if(ex_weight_sum_flag==1)
	{
		ex_weight_sum_flag=0;
		weight_sum();
	}

	if(ex_weight_total_sum_flag==1)
	{
		ex_weight_total_sum_flag=0;
		weight_total_sum();
	}
}
