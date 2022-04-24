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


void RTCCUnlock(void)
{
	asm volatile("disi	#5");
	asm volatile("mov	#0x55, w7");
	asm volatile("mov	w7, _NVMKEY");
	asm volatile("mov	#0xAA, w8");
	asm volatile("mov	w8, _NVMKEY");
	asm volatile("bset	_RCFGCAL, #13");
	asm volatile("nop");
	asm volatile("nop");
}

void RTCCInit(void)
{
	asm("mov OSCCON, W1");
	asm("mov.b	#0x02, W0");	 
	asm("mov.b	#0x46, W2");
	asm("mov.b	#0x57, W3");
	asm("mov.b	W2, [W1]");
	asm("mov.b	W3, [W1]");
	asm("mov.b	W0, [W1]");		

	// RCFGCAL: RTCC CALIBRATION/CONFIGURATION REGISTER
	RCFGCAL	= 0x0000;	 

	RTCCUnlock();
	
	// ALCFGRPT: ALARM CONFIGURATION REGISTER
	// Alarm is off
	ALCFGRPT = 0x0000; 


	// RTCLK<1:0>: RTCC Clock Source Select bits
	// 00 = External Secondary Oscillator (SOSC)
	RTCPWCbits.RTCLK0 = 0;
	RTCPWCbits.RTCLK1 = 0;

	//RCFGCALbits.RTSECSEL=1;
	RCFGCALbits.RTCOE=0;

	// RTCEN: RTCC Enable bit, 
	// 1=RTCC module is enabled
	RCFGCALbits.RTCEN = 1;


	// RTCWREN: RTCC Value Registers Write Enable bit
	// 0=RTCVALH and RTCVALL registers are locked out from being written to by the user
	RCFGCALbits.RTCWREN = 0; // Lock rtcc
}

void rtc_read(void)
{
	unsigned long datetime[4] = {0};


	RTCCUnlock(); // Unlock rtcc

	// RTCPTR<1:0>: RTCC Value Register Window Pointer bits
	RCFGCALbits.RTCPTR =3;


	datetime[0] = RTCVAL;	//	year
	datetime[1] = RTCVAL;	//	month, day
	datetime[2] = RTCVAL;	//	hour
	datetime[3] = RTCVAL;	//	minute, second



	// RTCWREN: RTCC Value Registers Write Enable bit
	// 0=RTCVALH and RTCVALL registers are locked out from being written to by the user
	RCFGCALbits.RTCWREN = 0; // Lock rtcc


	sRTC.year   = bcd_to_decimal((unsigned char)(datetime[0] & 0x00FF));
	sRTC.month  = bcd_to_decimal((unsigned char)(datetime[1] >> 8));
	sRTC.day    = bcd_to_decimal((unsigned char)(datetime[1] & 0x00FF));
	sRTC.hour   = bcd_to_decimal((unsigned char)(datetime[2] & 0x00FF));
	sRTC.minute = bcd_to_decimal((unsigned char)(datetime[3] >> 8));
	sRTC.second = bcd_to_decimal((unsigned char)(datetime[3] & 0x00FF));

	if(sRTC.year  >99) 	sRTC.year   = 15;
	if(sRTC.month >12)	sRTC.month  = 10;
	if(sRTC.day   >31) 	sRTC.day    = 30;
	if(sRTC.hour  >24) 	sRTC.hour   = 11;
	if(sRTC.minute>59)	sRTC.minute = 04;
	if(sRTC.second>59) 	sRTC.second = 17;
}

void rtc_test(void)
{
	unsigned long 	tmp;
	unsigned char	x_key=0;
	unsigned char	key_cnt=0;

	
	String_Display(PLU,"06");
	String_Display(UNIT_PRICE," RTC ");

	while(1)
	{
		rtc_read();
		tmp=((sRTC.hour*100)+(sRTC.minute));
		plu_display(sRTC.second);
		weight_display(tmp);

		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			switch(x_key) 
			{
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


//****************************************************************
void rtc_set(void)
{
	rtc_read();

	String_Display(PAY, "  RTC "); 

	String_Display(UNIT_PRICE, " YEAR"); 
	clock_disp(0);
	if(poff_sig==1)	return;
	String_Display(UNIT_PRICE, " DATE"); 
	clock_disp(1);
	String_Display(UNIT_PRICE, " TIME"); 
	clock_disp(2);

	RTCCUnlock(); // Unlock rtcc

	RCFGCALbits.RTCPTR =3;

	RTCVAL = (unsigned long)decimal_to_bcd(sRTC.year);
	RTCVAL = ((unsigned long)decimal_to_bcd(sRTC.month) << 8) | (unsigned long)decimal_to_bcd(sRTC.day);
	RTCVAL = (unsigned long)decimal_to_bcd(sRTC.hour);
	RTCVAL = ((unsigned long)decimal_to_bcd(sRTC.minute) << 8) | (unsigned long)decimal_to_bcd(sRTC.second);

	RCFGCALbits.RTCWREN = 0; // Lock rtcc

	String_Display(PAY, "   END"); 
 	DelayMs(700);
}

//****************************************************************

void clock_disp(unsigned char v_rtc)
{
	unsigned char bound=0;
	unsigned char loop_out=0;
	unsigned char year_10=0;
	unsigned char year_1=0;
	unsigned char month_10=0;
	unsigned char month_1=0;
	unsigned char day_10=0;
	unsigned char day_1=0;
	unsigned char hour_10=0;
	unsigned char hour_1=0;
	unsigned char minute_10=0;
	unsigned char minute_1=0;

	unsigned long bound_count=0;

	unsigned char x_key=0;
	unsigned char key_cnt=0;


	year_10   = sRTC.year/10*10;
	year_1	  = sRTC.year%10;
	
	month_10  = sRTC.month/10*10;
	month_1   = sRTC.month%10;
	
	day_10	  = sRTC.day/10*10;
	day_1	  = sRTC.day%10;
	
	hour_10   = sRTC.hour/10*10;
	hour_1	  = sRTC.hour%10;
	
	minute_10 = sRTC.minute/10*10;
	minute_1  = sRTC.minute%10;


	if(v_rtc==0)	bound=2;
	else			bound=0;
	
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
				case F2_KEY:		// 8
				case F3_KEY:		// 9
					tone_type=SHORT_TONE_PLAY;									
					if(v_rtc==0)
					{
						if(bound==2) { year_10 = 10*x_key; if(year_10>90) year_10=0; }
						if(bound==3) { year_1 = x_key; if(year_1>9) year_1=0; }
						
					}
					else if(v_rtc==1)
					{
						if(bound==0) { month_10 = 10*x_key; if(month_10>10) month_10=0; }
						if(bound==1) { month_1 = x_key; if(month_10==10) { if(month_1>2) month_1=0; } else if(month_1>9) month_1=0; }
						if(bound==2) { day_10 = 10*x_key; if(day_10>30) day_10=0; }
						if(bound==3) { day_1 = x_key; if(day_10==30) { if(day_1>1) day_1=0; } else if(day_1>9) day_1=0; }                     
					}
					else if(v_rtc==2)
					{
						if(bound==0) { hour_10 = 10*x_key; if(hour_10>20) hour_10=0; }
						if(bound==1) { hour_1 = x_key; if(hour_10==20) { if(hour_1>3) hour_1=0; } else if(hour_1>9) hour_1=0; }
						if(bound==2) { minute_10 = 10*x_key; if(minute_10>50) minute_10=0; }
						if(bound==3) { minute_1 = x_key; if(minute_1>9) minute_1=0; }
					}
					bound++; 
					if( (v_rtc==0)&&(bound>3) ) 											bound=2;
					else if( ((v_rtc==1)||(v_rtc==2))&&(bound>3) )							bound=0;
					break;
				
				case ENT_K:
					tone_type=LONG_TONE_PLAY;									
					String_Display(WEIGHT, "     "); 
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



		sRTC.year   = year_10 + year_1;
		sRTC.month  = month_10 + month_1;
		sRTC.day    = day_10 + day_1;
		sRTC.hour   = hour_10 + hour_1;
		sRTC.minute = minute_10 + minute_1;



		if(bound_count<=1000)
		{
			if(v_rtc==0)
			{
				//lcd_want_display(0, 11);
				lcd_want_display(WEIGHT, 1, 11);
				lcd_want_display(WEIGHT, 2, 2);
				lcd_want_display(WEIGHT, 3, 0);
				lcd_want_display(WEIGHT, 4, sRTC.year/10);
				lcd_want_display(WEIGHT, 5, sRTC.year%10);
			}
			else if(v_rtc==1)
			{
				//lcd_want_display(0, 11);
				lcd_want_display(WEIGHT, 1, sRTC.month/10);
				lcd_want_display(WEIGHT, 2, sRTC.month%10);
				lcd_want_display(WEIGHT, 3, 11);
				lcd_want_display(WEIGHT, 4, sRTC.day/10);
				lcd_want_display(WEIGHT, 5, sRTC.day%10);
			}
			else if(v_rtc==2)
			{
				//lcd_want_display(0, 11);
				lcd_want_display(WEIGHT, 1, sRTC.hour/10);
				lcd_want_display(WEIGHT, 2, sRTC.hour%10);
				lcd_want_display(WEIGHT, 3, 11);
				lcd_want_display(WEIGHT, 4, sRTC.minute/10);
				lcd_want_display(WEIGHT, 5, sRTC.minute%10);
			}

			bound_count++;
		}
		else if(bound_count<=5000)
		{
			if(v_rtc==0)
			{
				if(bound==2) lcd_want_display(WEIGHT, 4,11);
				if(bound==3) lcd_want_display(WEIGHT, 5,11);

				bound_count++;
			}
			else
			{
				if(bound==0) lcd_want_display(WEIGHT, 1,11);
				if(bound==1) lcd_want_display(WEIGHT, 2,11);
				if(bound==2) lcd_want_display(WEIGHT, 4,11);
				if(bound==3) lcd_want_display(WEIGHT, 5,11);

				bound_count++;
			}
		}
		else 
		{
			bound_count=0; 
		}

		if(loop_out)
		{
			break;
		}
	} 

	sRTC.second = 0;
}


//****************************************************************
unsigned char bcd_to_decimal(unsigned char rtc_value)
{
	unsigned char temp;


	temp = (rtc_value & 0xF0) >> 4;
	temp = (temp * 10) + (rtc_value & 0x0F);

	return(temp);
}

//****************************************************************
unsigned char decimal_to_bcd(unsigned char rtc_value)
{
	unsigned char temp;


	temp = (rtc_value/10) << 4;
	temp = temp | (rtc_value % 10);

	return(temp);
}

void clock_epr_set(void)
{
	unsigned char 	bound=0;
	unsigned char 	loop_out=0;
	unsigned char 	hour_10=0;
	unsigned char 	hour_1=0;
	unsigned char 	minute_10=0;
	unsigned char 	minute_1=0;

	unsigned long 	bound_count=0;

	unsigned char 	x_key=0;
	unsigned char	key_cnt=0;



	
	hour_10   = ep_rtc_hour/10*10;
	hour_1	  = ep_rtc_hour%10;
	
	minute_10 = ep_rtc_minute/10*10;
	minute_1  = ep_rtc_minute%10;



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
					if(bound==0) { hour_10 = 10*x_key; if(hour_10>20) hour_10=0; }
					if(bound==1) { hour_1 = x_key; if(hour_10==20) { if(hour_1>3) hour_1=0; } else if(hour_1>9) hour_1=0; }
					if(bound==2) { minute_10 = 10*x_key; if(minute_10>50) minute_10=0; }
					if(bound==3) { minute_1 = x_key; if(minute_1>9) minute_1=0; }
					bound++; 
					if( (bound>3) )	bound=0;
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



		ep_rtc_hour   = hour_10 + hour_1;
		ep_rtc_minute = minute_10 + minute_1;



		if(bound_count<=1000)
		{
			lcd_want_display(WEIGHT, 1, ep_rtc_hour/10);
			lcd_want_display(WEIGHT, 2, ep_rtc_hour%10);
			lcd_want_display(WEIGHT, 3, 11);
			lcd_want_display(WEIGHT, 4, ep_rtc_minute/10);
			lcd_want_display(WEIGHT, 5, ep_rtc_minute%10);

			bound_count++;
		}
		else if(bound_count<=5000)
		{
			if(bound==0) lcd_want_display(WEIGHT, 1,11);
			if(bound==1) lcd_want_display(WEIGHT, 2,11);
			if(bound==2) lcd_want_display(WEIGHT, 4,11);
			if(bound==3) lcd_want_display(WEIGHT, 5,11);

			bound_count++;
		}
		else 
		{
			bound_count=0; 
		}

		if(loop_out)
		{
			break;
		}
	} 
}

