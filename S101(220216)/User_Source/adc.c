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

void zero_function_key(void);
void tare_function_key(void);
void key_tare_function_key(void);

void hold_function_key(void);
void hold_mode_step_0(void);
void hold_mode_step_1(void);
void hold_bar_ing(void);
void hold_mode_step_3(void);





unsigned  long g_temp_long=0;
unsigned  long g_set_weight=0;
unsigned  char err=0;
unsigned  long g_ad_set_buf[2] = {0};

unsigned  long g_prev_adc1=0;
unsigned  char g_az_scope=0;
unsigned short g_az_ok=0;
unsigned  long g_middle_value=0;
		 float g_middle_factor=0;



//******************************************************************************
void adc_initial(void)
{
	unsigned char i, j, temp_char;
	unsigned char adc_set_data[8]= { 0x08,0x0C,0x00,0x00,0x10,0x00,0x02,0x18 };
    //unsigned char adc_set_data[8]= { 0x08,0x00,0x00,0x00,0x10,0x00,0x02,0x18 };  //외부 OSC
    
	unsigned int  v_speed;

	//unsigned char ep_filter_degree=5;


	// communications : 0000 1000 (mode resister select)
	// mode_1         : 0000 1100 (internal clock)
	// mode_2         : 0000 0000
	// mode_3         : 0000 0000 (4805/10Hz)
	// communications : 0001 0000 (configuration resister select)
	// configration_1 : 0000 0000 (chop disable)
	// configration_2 : 0000 0010 (channel 3-4)
	// configration_3 : 0001 1000 (buffer , unipolar, chop enable)

    /*
	if(		ep_filter_degree==1)	temp_char=10;
	else if(ep_filter_degree==2) 	temp_char=9;
	else if(ep_filter_degree==3) 	temp_char=8;
	else if(ep_filter_degree==4) 	temp_char=7;
	else if(ep_filter_degree==5) 	temp_char=6;
	else if(ep_filter_degree==6) 	temp_char=5;
	else if(ep_filter_degree==7) { 	temp_char=10; adc_set_data[5]=0x80; 	} // chop enable 1/4 speed
	else if(ep_filter_degree==8) { 	temp_char=9;  adc_set_data[5]=0x80; 	} // chop enable 1/4 speed
	else if(ep_filter_degree==9) { 	temp_char=8;  adc_set_data[5]=0x80; 	} // chop enable 1/4 speed
    */
    if(ep_filter_degree <= 7)	temp_char=9;  //F1-1~7
	else temp_char = 5;  //F1-8~9
    
	v_speed = 4805/temp_char;
	adc_set_data[2] = adc_set_data[2] | (unsigned char)(v_speed>>8);
	adc_set_data[3] = adc_set_data[3] | (unsigned char)(v_speed&0x00FF);
	adc_set_data[7] = adc_set_data[7] | 7; // gain 128
    //adc_set_data[7] = adc_set_data[7] | 3; // gain 8
    
	cbi( LATG, AD_CS );
	sbi( LATG, AD_SCLK );
	sbi( LATG, AD_DIN );

	Nop(); 
	Nop();
	
	for(i=0; i<33; i++)
	{ 
		cbi( LATG, AD_SCLK );
		sbi( LATG, AD_SCLK ); 
	}
	
	cbi( LATG, AD_DIN );

	for(j=0; j<8; j++)
	{
		temp_char=adc_set_data[j]; 
		adc_loop(temp_char);
	}
	
	sbi ( LATG, AD_CS );
}
//******************************************************************************

void test_adc_initial(void)
{
	unsigned char i, j, temp_char;
	unsigned char adc_set_data[8]= { 0x08,0x0C,0x00,0x00,0x10,0x00,0x02,0x18 };
    //unsigned char adc_set_data[8]= { 0x08,0x00,0x00,0x00,0x10,0x00,0x02,0x18 };  //외부 OSC
    
	unsigned int  v_speed;

	//unsigned char ep_filter_degree=5;


	// communications : 0000 1000 (mode resister select)
	// mode_1         : 0000 1100 (internal clock)
	// mode_2         : 0000 0000
	// mode_3         : 0000 0000 (4805/10Hz)
	// communications : 0001 0000 (configuration resister select)
	// configration_1 : 0000 0000 (chop disable)
	// configration_2 : 0000 0010 (channel 3-4)
	// configration_3 : 0001 1000 (buffer , unipolar, chop enable)

    /*
	if(		ep_filter_degree==1)	temp_char=10;
	else if(ep_filter_degree==2) 	temp_char=9;
	else if(ep_filter_degree==3) 	temp_char=8;
	else if(ep_filter_degree==4) 	temp_char=7;
	else if(ep_filter_degree==5) 	temp_char=6;
	else if(ep_filter_degree==6) 	temp_char=5;
	else if(ep_filter_degree==7) { 	temp_char=10; adc_set_data[5]=0x80; 	} // chop enable 1/4 speed
	else if(ep_filter_degree==8) { 	temp_char=9;  adc_set_data[5]=0x80; 	} // chop enable 1/4 speed
	else if(ep_filter_degree==9) { 	temp_char=8;  adc_set_data[5]=0x80; 	} // chop enable 1/4 speed
    */
    //if(ep_filter_degree <= 7)	temp_char=9;  //F1-1~7
	//else temp_char = 5;  //F1-8~9
    temp_char=8;  adc_set_data[5]=0x80;
    
	v_speed = 4805/temp_char;
	adc_set_data[2] = adc_set_data[2] | (unsigned char)(v_speed>>8);
	adc_set_data[3] = adc_set_data[3] | (unsigned char)(v_speed&0x00FF);
	adc_set_data[7] = adc_set_data[7] | 7; // gain 128
    //adc_set_data[7] = adc_set_data[7] | 3; // gain 8
    
	cbi( LATG, AD_CS );
	sbi( LATG, AD_SCLK );
	sbi( LATG, AD_DIN );

	Nop(); 
	Nop();
	
	for(i=0; i<33; i++)
	{ 
		cbi( LATG, AD_SCLK );
		sbi( LATG, AD_SCLK ); 
	}
	
	cbi( LATG, AD_DIN );

	for(j=0; j<8; j++)
	{
		temp_char=adc_set_data[j]; 
		adc_loop(temp_char);
	}
	
	sbi ( LATG, AD_CS );
}

//******************************************************************************
void adc_loop(unsigned char adc_ini)
{
	unsigned char i;


	for(i=0; i<8; i++)
	{
		if(adc_ini & 0x80) 	sbi ( LATG, AD_DIN );
		else               	cbi ( LATG, AD_DIN );

		cbi ( LATG, AD_SCLK );
		sbi ( LATG, AD_SCLK );
		
		adc_ini = adc_ini << 1;
	}
}
//******************************************************************************


//******************************************************************************
unsigned long read_filtered_adc(void)
{
	unsigned char i, j;
	unsigned long read_data=0, imsi_data;


	read_data = 0;
	ex_ad_flag = 0;

	cbi( LATG, AD_CS ); 
	
	Nop(); 
	Nop(); 

	if( ((PORTG&(0x0001<<AD_DOUT))==BV(AD_DOUT)) ) 
	{ 
		sbi( LATG, AD_CS );
		
		return(0);
	}

	adc_loop(0x58);

	for(i=0; i<3; i++)
	{
		for(j=0; j<8; j++)
		{
			cbi ( LATG, AD_SCLK );
			read_data = read_data << 1;

			if ( (PORTG&(0x0001<<AD_DOUT))==BV(AD_DOUT) )
			{
				read_data = read_data | 0x00000001;
			}

			sbi ( LATG, AD_SCLK );
		}
	}
	
	imsi_data= read_data;
	read_data= read_data >> 3;
	sbi ( LATG, AD_CS );
	ex_ad_flag = 1;

	return(read_data);
    //return(imsi_data);
}
//******************************************************************************


void Capa_Set(void)
{
	//CAL1
	String_Display(PAY, "  CAL ");
	String_Display(PLU, "  ");
	String_Display(UNIT_PRICE, " CAPA");
	DelayMs(700);
	Low_Batt_check();
	none_point_flag=1;  
    ep_maximum_capacity = five_byte(ep_maximum_capacity);
	none_point_flag=0;
}

unsigned char Div_Set(void)
{
	unsigned char 	i=0;
	unsigned char	j=0;
	unsigned char	exit_flag=0;
	unsigned char 	x_key=0;
	unsigned char	key_cnt=0;
	float 			temp_float=0.0;
	unsigned long 	temp_long=0;

	//CAL2
	exit_flag=0;
	String_Display(UNIT_PRICE, "  DIV");
  	for(i=1; i<21; i++) 
  	{ 
    	if(ep_minimum_division == POINT_DATA[i]) 
    	{ 
			j = i; 
			break; 
		}
  	}

	temp_long = (unsigned long)( ep_minimum_division & 0x07);
	if(ep_minimum_division & 0x08) temp_long = temp_long * 10; 
	if(ep_minimum_division & 0x10) temp_long = temp_long * 100;

	weight_display(temp_long);

	do
	{
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			switch(x_key) 
			{
				case SAVE_KEY:/* 5 */	
					tone_type=SHORT_TONE_PLAY;
					
					j++; 

					if(j>=21) j=0;
					
					break;

				case CLR_K:/* 0 */	
					tone_type=SHORT_TONE_PLAY;

					if(j==0) j=20; 
					else 	 j--;
					
					break;

				case ENT_K:
					tone_type=SHORT_TONE_PLAY;

					ep_minimum_division = POINT_DATA[j]; 
					temp_float = (float)ep_maximum_capacity / (float)(ep_minimum_division & 0x07);

					if     ((ep_minimum_division & 0xF8) == 0x08) temp_float = temp_float / 10; 
					else if((ep_minimum_division & 0xF8) == 0x10) temp_float = temp_float / 100;
					else if((ep_minimum_division & 0xF8) == 0x20) temp_float = temp_float * 10; 
					else if((ep_minimum_division & 0xF8) == 0x40) temp_float = temp_float * 100;
					else if((ep_minimum_division & 0xF8) == 0x60) temp_float = temp_float * 1000;
					else if((ep_minimum_division & 0xF8) == 0x80) temp_float = temp_float * 10000;

					ep_e_resolution = (unsigned long)temp_float;
					
					if((ep_e_resolution > 50000) || (ep_e_resolution < 10))
					{ 
						String_Display(WEIGHT, "CH 03");
						DelayMs(1000); 
						return(1); 
					}                    
					
					exit_flag = 1;
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

				default:	// 0.2
					tone_type=0x01;
					
					j=12;	// 200g
					break;
			}
		}	

		ep_minimum_division = POINT_DATA[j];
		
		temp_long = (unsigned long)( ep_minimum_division & 0x07);
		if(ep_minimum_division & 0x08) temp_long = temp_long * 10; 
		if(ep_minimum_division & 0x10) temp_long = temp_long * 100;

		weight_display(temp_long);
		
	}while(exit_flag==0);

	return(0);
}

unsigned char Set_Weight(void)
{
	String_Display(UNIT_PRICE, "SET-F");
	//DelayMs(700);

	none_point_flag=1;
	g_set_weight = five_byte(ep_maximum_capacity);
	none_point_flag=0;
	err=0;
	cal_err();
	return(0);
}

void Zero_Calibration(void)
{
	// zero calibration
	String_Display(UNIT_PRICE, " ZERO");
	String_Display(WEIGHT, "     ");
	zero_span_set(0);
}

unsigned char Full_Calibration(void)
{
	// full calibration
	if(ex_stab_flag==0)
	{  
		zero_span_set(1);  
		
		if(ex_stab_flag==0)
		{
			return(1);
		}
	}  

	return(0);
}

void End_Single_Cal_Mode(void)
{
	unsigned char 	x_key=0;
	unsigned char	key_cnt=0;
	unsigned char	enter_f=0;


	String_Display(UNIT_PRICE, " END ");
	String_Display(WEIGHT, "     ");

	while(enter_f==0) 
	{  
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			switch(x_key) 
			{
				case ENT_K:  
					tone_type=0x01;
					ep_cal_no++;
					
					eeprom_write();
					DelayMs(10); 

					ex_calib_trace_flag=1;
					enter_f=1;

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

void Single_Calibration_Mode(void)
{
	unsigned char i=0, j=0;
	unsigned char x_key=0;
	unsigned char single_cal_step=0;


	ex_calib_trace_flag=0;
	ex_sign_flag = 0;

	while(1)
	{
		switch(single_cal_step)
		{
			case 0:
					Capa_Set();
					single_cal_step=1;
					break;

			case 1:
					Div_Set(); 			
                    if((ep_e_resolution > 50000) || (ep_e_resolution < 10)) single_cal_step=0;
                    else single_cal_step=2;
					break;

			case 2:
					Set_Weight(); 			
                    if(err) {err=0; single_cal_step=0;}
                    else single_cal_step=3;
					break;

			case 3:
					Zero_Calibration();
					single_cal_step=4;
					break;

			case 4:
					if( Full_Calibration() )	single_cal_step=0;
					else						single_cal_step=5;
					break;

			case 5:
					End_Single_Cal_Mode();
                    String_Display(UNIT_PRICE, " END ");
                    DelayMs(500);
                    String_Display(UNIT_PRICE, "     ");
                    DelayMs(500); 
                    String_Display(UNIT_PRICE, " END ");
                    DelayMs(500);
                    String_Display(UNIT_PRICE, "     ");
					return;

			default:
					break;
		}
	}
}


//******************************************************************************
void zero_span_set(unsigned char flag)  // 0 : zero set, 1: span set 
{
	unsigned char   exit_flag=0;
	unsigned char	enter_f=0;
	unsigned char   x_key=0;
	unsigned char	key_cnt=0;
	unsigned long   i=0;
	unsigned long	j=0;
	float  			temp_float=0.0;
    long            TempLong[5];
	// Modified Variable
	// v_adc3_buf --> v_i_resolution
	// v_adc1_buf --> setting_weight 


	ex_stab_flag = 0; // Used for only zero calibration

	if(flag)
	{
		String_Display(UNIT_PRICE, " LOAD"); 
		String_Display(WEIGHT, "--UP-"); 
	}

	while(enter_f==0) 
	{   
		if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
		{
			x_key=Key_Data_Read();
			switch(x_key)
			{
				case ENT_K:
					tone_type=0x01;
					enter_f=1;
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
	
	do
	{
		String_Display(WEIGHT, "-----");

		do TempLong[0] = read_filtered_adc(); while(ex_ad_flag==0);
        do TempLong[1] = read_filtered_adc(); while(ex_ad_flag==0);
        do TempLong[2] = read_filtered_adc(); while(ex_ad_flag==0);
        do TempLong[3] = read_filtered_adc(); while(ex_ad_flag==0);
        do TempLong[4] = read_filtered_adc(); while(ex_ad_flag==0);
        
        g_temp_long = (TempLong[0] + TempLong[1] + TempLong[2] + TempLong[3] + TempLong[4])/5;
		ex_e_value = g_temp_long;

		if(flag) g_ad_set_buf[1] = ex_e_value;
		else     g_ad_set_buf[0] = ex_e_value;

		if(flag)
		{
			// add
			g_temp_long = ep_e_resolution/ (unsigned long)((float)ep_maximum_capacity / (float)g_set_weight);

			if((g_ad_set_buf[1] - g_ad_set_buf[0]) < g_temp_long)
		  	{
		  		String_Display(WEIGHT, "CH 05"); 
		  		DelayMs(700); 			// span low
		  		exit_flag = 1;		 	//goto mainload;
			}
			else
			{
				temp_float = (float)(g_ad_set_buf[1] - g_ad_set_buf[0]) * ((float)ep_maximum_capacity / (float)g_set_weight);
				ep_res_factor = (float)(ep_e_resolution * 20) / temp_float;
//                ep_res_factor_dual = (float)(ep_e_resolution_dual * 20) / temp_float;
				ex_adc_org = (unsigned long) ((float)g_ad_set_buf[0] * ep_res_factor);
//                ex_adc_org_dual = (unsigned long) ((float)g_ad_set_buf[0] * ep_res_factor_dual);
				ep_zero = g_ad_set_buf[0];
				ex_stab_flag = 1;
				ep_tare_value = ex_adc_org;
				ep_tare_flag = 0;
				exit_flag=1;
			}
		} 
		else
		{
			exit_flag =1; 
		}
	} 
	while(exit_flag==0);
}
//******************************************************************************




void cal_err(void)
{
	if(g_set_weight > ep_maximum_capacity)
	{
		String_Display(WEIGHT, " CH 04");
		DelayMs(700);
		
		err=1;
	}
	
	if((g_set_weight*100) < ep_maximum_capacity)
	{
		String_Display(WEIGHT, " CH 04");
		DelayMs(700);
		
		err=1;
	}
}


void Weight_Init(void)
{
	unsigned char toggle=0;
	unsigned char i;

	unsigned long v_tare_long;
	unsigned long v_b_value, v_sum_value;


	//buffer_initial();

	g_prev_adc1 = ex_adc_org;
	v_sum_value=0;

	g_az_scope = ep_az_digit * 10;
	if(ep_az_second==0) 	g_az_ok = 1000;  // 1 sec
	else 					g_az_ok = ep_az_second * 1000;

	
	if(ep_hold_start==1)
	{
		ex_hold_flag=1; 
		ex_auto_hold_on=1; 

	#if 0
		//lampdsp[8]=1; 
		ex_hold_flag=1;
		ex_weight_hold_value =0; 
		ex_hold_finish=0; 
		ex_hold_count=0;      
		ex_auto_hold_on=1; 
		ex_hold_on=0; 
	#endif 
	}
}







void Weight_Task(void)
{
	volatile unsigned long adc1_buf=0, adc3_buf=0, adc1_buf_dual=0; 
	long diff1=0, diff2=0, diff4=0;
    long diff3=0;

	unsigned char x_key=0;
	unsigned char auto_flag=0;
	unsigned char stb_stt=0;


	static float hold_range_value;
	static float hold_old_value;

	unsigned long h_speed_tmp;

	h_speed_tmp=ep_holdspeed*200;

	ex_weight_task_en_f=1;

	weight_division = ep_minimum_division & 0x07;
	if(ep_minimum_division & 0x08) weight_division = weight_division * 10; 
	if(ep_minimum_division & 0x10) weight_division = weight_division * 100;
//    ep_minimum_division_dual=POINT_DATA[5];
//    weight_division_dual = ep_minimum_division_dual & 0x07;
//	if(ep_minimum_division_dual & 0x08) weight_division_dual = weight_division_dual * 10; 
//	if(ep_minimum_division_dual & 0x10) weight_division_dual = weight_division_dual * 100;


	// 5개의 '-'로 Display 하기 위해 5로 나누어줌
	//ep_holdspeed=99;
	weight_h_speed = ((float)(ep_holdspeed/HOLD_COUNT_DIVIDE)/5.0);
	//weight_h_speed = 10;

	//////////////////////////////////////////////////////////////////////// KEY
	if(		ex_weight_zero_flag==1)	zero_function_key();
	else if(ex_weight_tare_flag==1)	tare_function_key();
	else if(ex_weight_hold_flag==1)	hold_function_key();
	else if(ex_weight_key_tare_flag==1) key_tare_function_key();
	//else if(ex_weight_add_flag==1)
	//else if(ex_weight_sum_flag==1)
	//////////////////////////////////////////////////////////////////////// KEY



	//	READ A/D	////////////////////////////////////////////////////////////
	do 
	{
		weight_temp_long = read_filtered_adc();
	}
	while(ex_ad_flag==0);

    weight_temp_long = data_avg(weight_temp_long, 0);
    //adc1_buf = weight_temp_long;
	adc1_buf  = (long) ( ((float) weight_temp_long) * ep_res_factor );    //raw * factor
    //adc1_buf_dual  = (long) ( ((float) weight_temp_long) * ep_res_factor_dual );    //raw * factor
	////////////////////////////////////////////////////////////////////////////
	//	Calculate diff A/D Value	////////////////////////////////////////////
	if(ep_tare_flag==0)
    {
        if ( adc1_buf > ex_adc_org)   	diff1 =  adc1_buf   - ex_adc_org;  //영점 보정을 위해
        else                           	diff1 =  ex_adc_org - adc1_buf;
    }
    else
    {
        if ( adc1_buf > ep_tare_value)  diff1 =  adc1_buf   - ep_tare_value;  //영점 보정을 위해
        else                           	diff1 =  ep_tare_value - adc1_buf;
        if ( adc1_buf > ex_adc_org)   	diff4 =  adc1_buf   - ex_adc_org;  //영점 보정을 위해
        else                           	diff4 =  ex_adc_org - adc1_buf;
    }

	if ( adc1_buf > g_prev_adc1 )  	diff2 =  adc1_buf   - g_prev_adc1;  //안정 체크를 위해
	else                           	diff2 =  g_prev_adc1  - adc1_buf;

	if(ep_az_digit)  //자동 영점
    {
        if (diff1<=g_az_scope && ex_stab_flag )
        {
            if((adc1_buf <= FirstZeroPlusLimit) &&  (adc1_buf >= FirstZeroMinusLimit)) //calibration zero 10% 
            {
                if(PowerOnZero >= ((ep_e_resolution/5)*ep_zero_range_value)) diff3 = (PowerOnZero - ((ep_e_resolution/5)*ep_zero_range_value));
                else diff3=0;
                if( (adc1_buf < (PowerOnZero + (ep_e_resolution/5)*ep_zero_range_value)) && (adc1_buf > diff3) ) // limit +-2%
                {
                    if( autozero_count > g_az_ok )
                    {
                        if(ep_tare_flag == 0)
                        {
                            ex_adc_org = adc1_buf;
                        }
                        else
                        {
                            if ( adc1_buf > ep_tare_value)
                            {
                                ex_adc_org =  ex_adc_org + (adc1_buf   - ep_tare_value); 
                                ep_tare_value = ep_tare_value + (adc1_buf - ep_tare_value);
                            }
                            else                           	
                            {
                                ex_adc_org =  ex_adc_org - (ep_tare_value - adc1_buf);
                                ep_tare_value =  ep_tare_value - (ep_tare_value - adc1_buf);
                            }
                        }
                        autozero_count = 0;
                        g_prev_adc1 = adc1_buf;
                    } 
                }
                else
                {
                    autozero_count=0;
                }
            }
        }
        else if(ep_tare_flag && diff4<=g_az_scope && ex_stab_flag )
        {
            if((adc1_buf <= FirstZeroPlusLimit) &&  (adc1_buf >= FirstZeroMinusLimit)) //calibration zero 10% 
            {
                if(PowerOnZero >= ((ep_e_resolution/5)*ep_zero_range_value)) diff3 = (PowerOnZero - ((ep_e_resolution/5)*ep_zero_range_value));
                else diff3=0;
                if( (adc1_buf < (PowerOnZero + (ep_e_resolution/5)*ep_zero_range_value)) && (adc1_buf > diff3) ) // limit +-2%
                {
                    if( autozero_count > g_az_ok )
                    {
                        if ( adc1_buf > ex_adc_org)
                        { 
                            ep_tare_value = ep_tare_value + (adc1_buf - ex_adc_org);
                        }
                        else                           	
                        {
                            ep_tare_value =  ep_tare_value - (ex_adc_org - adc1_buf);
                        }
                        ex_adc_org =  adc1_buf;
                        /*
                        if(ep_tare_flag == 0)
                        {
                            ex_adc_org = adc1_buf;
                        }
                        else
                        {
                            if ( adc1_buf > ep_tare_value)
                            {
                                ex_adc_org =  ex_adc_org + (adc1_buf   - ep_tare_value); 
                                ep_tare_value = ep_tare_value + (adc1_buf - ep_tare_value);
                            }
                            else                           	
                            {
                                ex_adc_org =  ex_adc_org - (ep_tare_value - adc1_buf);
                                ep_tare_value =  ep_tare_value - (ep_tare_value - adc1_buf);
                            }
                        }*/
                        autozero_count = 0;
                        g_prev_adc1 = adc1_buf;
                    } 
                }
                else
                {
                    autozero_count=0;
                }
            }
        }
        else 
        {
            autozero_count = 0;
        }
	}
      
	g_prev_adc1 = adc1_buf;
	////////////////////////////////////////////////////////////////////////////

    //	Check whether stable	////////////////////////////////////////////////
    if(diff2 > 9) 
    {
        ex_stb_count = 0;
        ex_stab_flag=0; 
    }
    else
    {
        if(ex_stb_count > (ep_stable * 250) )
        {
            ex_stab_flag=1; 
            ex_stb_count=0;
        }
    }
	
    g_prev_adc1 = adc1_buf;
	////////////////////////////////////////////////////////////////////////////

	// Calcurate Weight
	if(adc1_buf >= ex_adc_org) 
	{ 
		adc3_buf = adc1_buf-ex_adc_org; 
		ex_sign_flag=0; 
	} 
	else                        
	{ 
		adc3_buf = ex_adc_org - adc1_buf;  
		if( adc3_buf > 8) 	ex_sign_flag=1;
		else                ex_sign_flag=0;
	}
	
	weight_temp_long = adc3_buf;


	if(ep_tare_flag==0) 
	{
		adc3_buf = weight_temp_long;
	}
	else
    { 
		if(adc1_buf >= ep_tare_value) 
		{
            
			adc3_buf = adc1_buf - ep_tare_value; 
			ex_sign_flag=0; 
		} 
      	else                           
      	{ 
      	 	adc3_buf = ep_tare_value - adc1_buf;
			if( adc3_buf > 8) 	ex_sign_flag=1;
			else                ex_sign_flag=0;
		} 
	} 

	//adc3_buf =  (long)( (float)adc3_buf * (float)ep_gravity_factor1/(float)ep_gravity_factor2 ); // 중력보상

    // zero lamp
    if(adc3_buf <= 10) 	{ 	ex_zero_flag=1; ex_weight_add_tick=0;}	// zero lamp on
    else             	{  	ex_zero_flag=0; }	// zero lamp off

    ex_e_value = (adc3_buf + 10) / 20;
    
    if( weight_temp_long > ((ep_e_resolution+9)*20) )	// Over_Weight  9 Division
    {
      	//stt_out('G', 5);
  		LAMP_LCD3_K_OFF;
		LAMP_LCD3_G_OFF;
		LAMP_LCD3_STABLE_OFF;
		LAMP_LCD3_LB_OFF;
		LAMP_LCD3_ZERO_OFF;

		ex_weight_over_tick=700;
		adc_initial(); 
    }
    else if(ex_sign_flag && (adc1_buf < (ex_adc_org - (19*20))) && ep_MinusDiplayEnable == 0)	// minus weight limit  19 Division
    {
        LAMP_LCD3_K_OFF;
		LAMP_LCD3_G_OFF;
		LAMP_LCD3_STABLE_OFF;
		LAMP_LCD3_LB_OFF;
		LAMP_LCD3_ZERO_OFF;
        
        ex_weight_minus_over_tick=700;
        adc_initial(); 
    }
    else
    {
      	ex_e_value = ex_e_value * (long)weight_division;
//        if(ex_e_value>=ep_DualRangeBoundary)
//        {
//            adc3_buf= (long)((float)adc3_buf / ep_res_factor);
//            adc3_buf= (long)((float)adc3_buf * ep_res_factor_dual);
//            ex_e_value = ((adc3_buf) + 10) / 20;
//            ex_e_value = (ex_e_value * (long)weight_division_dual);
//            if(ep_maximum_capacity == 30) ex_e_value = ex_e_value * 10;
//        }

      	if(ex_e_value<=weight_division*ep_hold_zero) 				//zero range
      	{ 
      	 	stb_stt=1; 
			auto_flag=0;
      	 	if(ex_auto_hold_on==0) 
			{ 
				//lampdsp[8]=0; 
				ex_hold_flag=0;
				ex_hold_on=0; 
				ex_hold_count=0; 
				ex_hold_finish=0;
                ex_hold_bar=0;
                ex_hold_time = 0;
			} 
      	} 
      	else
		{
			auto_flag=1;
      	}

		//	자동 홀드 ON이면 hold flag up
      	if(ex_auto_hold_on) 
		{ 
			//lampdsp[8]=1;
			ex_hold_flag=1; 
		}

		//auto hold상태에서 무게가 0~9e미만이면 초기화
      	if(ex_auto_hold_on && (ex_e_value<=(weight_division*ep_hold_zero))) 		
      	{ 
			//auto hold에서 초기 10%이상 변화를 체크하기 위함
			ex_hold_start_delay_timer=HOLD_START_DELAY_TIME;				

			ex_hold_count=0; 
			ex_hold_on=0; 
			ex_weight_hold_value=0; 
			ex_hold_finish=0;
            ex_hold_bar=0;
            ex_hold_time = 0;
		}

		// (-)일 때 홀드 진입 못하게 함 
      	if(ex_sign_flag)
      	{
      	 	ex_weight_hold_value=0;
			ex_hold_on=0; 
			ex_hold_count=0; 
			ex_hold_finish=0;
            ex_hold_bar=0;
            ex_hold_time = 0;
			
      	 	if(ex_auto_hold_on==0) 
			{ 
				//lampdsp[8]=0;
				ex_hold_flag=0;
			}
      	}


		switch(ep_backlight)
		{
			case 0:
				//if(ex_back_light_flag)	{	BackLight_Off;	 ex_back_light_flag=0;		}
				//else					{	BackLight_On;	 ex_back_light_flag=1;		}
				break;
			
			case 1: 												// 자동 모드 일 경우
				if( (ex_e_value==0)||(ex_sign_flag==1) )	BackLight_Off;					// 현재 무게가 '0' 일경우 
				else										BackLight_On;					// 현재 무게가 '0' 아닐 경우 
		
				
		
				if( (ex_weight_sum_blink_flag==1) ) BackLight_On;	
				if( (ex_weight_sum_lamp_flag==1)  ) BackLight_On;	
				
				break;
		
			case 2: 												// 항상 ON 일 경우	
				BackLight_On; 
				break;
		
			default:
				break;
		}

      	if(ex_hold_on==0)													// Normal Mode 일 경우
      	{
      	 	//fast_evalue(v_e_value);
      	 	//lampdsp[14]=1; 
			//lampdsp[15]=1;
        	//if(v_rs_form==1)	pc_send(v_e_value);
			
      	}
      	else 															// Hold Mode 일 경우
		{
			if(ex_hold_time < h_speed_tmp)
			{
				ex_weight_hold_value = ex_weight_hold_value + ex_e_value;
                ex_hold_count++;
				if(ex_hold_time%2000 < 1000)ex_hold_bar=1;// "  --  "
                else ex_hold_bar=5;                       // " -----"
			}
			else if(ex_hold_time>=h_speed_tmp && ex_hold_count && !ex_hold_finish)
			{
				ex_weight_hold_value = ex_weight_hold_value / ex_hold_count;
//                if(ep_DualRangeBoundary >= ex_weight_hold_value)
//                {
//                    ex_weight_hold_value = ex_weight_hold_value - ex_weight_hold_value % weight_division;
//                }
//                else
//                {
//                    ex_weight_hold_value = ex_weight_hold_value - ex_weight_hold_value % weight_division_dual;
//                }
				ex_hold_finish=1;
                ex_hold_bar=0;
			}
			
			
			#if 0 	// 남문 버전 
			switch(ex_hold_step)
			{
				case 0:	hold_mode_step_0();	break;

				case 1:	hold_mode_step_1();	break;

				case 2:
					if(ep_hold_range_value==0)	// 홀드 Range가 무제한 일 경우, 현재 무게와 동일해야만 홀드 진행 됨 
					{	 
						hold_bar_ing();
					}
					else	// 홀드 Range가 제한이 있을 경우 
					{
						hold_old_value=(ex_weight_hold_value/ex_hold_count);

						if(hold_old_value >= (float)ep_hold_range_value)
						{
							if((ex_e_value<(unsigned long)(hold_old_value-(float)(ep_hold_range_value*weight_division))) )
							{

								ex_hold_step=1;
							}
							else if( (ex_e_value>(unsigned long)(hold_old_value+(float)(ep_hold_range_value*weight_division))) )
							{
								ex_hold_step=1;
							}
							else
							{
								hold_bar_ing();
							}
						}
						else
						{
							if(ex_e_value>(unsigned long)(hold_old_value+(float)(ep_hold_range_value*weight_division)))
							{
								ex_hold_step=1;
							}
							else
							{
								hold_bar_ing();
							}
						}
					}

					break;

				case 3:
					hold_mode_step_3();
					ex_hold_step=4;
					break;

				case 4:
					//weight_display(ex_weight_hold_value);

					switch(ep_wireless)
					{
						case 1:
						case 3:
							//rs_com(hold_value);
							break;
					
						case 2:
							//tf200_send(hold_value);
							break;
					
						default:
							break;
					}
					
					
					if(ex_hold_finish)
					{ 
						if(ep_rs_form==4)	  	{ /* rs_print(hold_value); */	ex_hold_finish=0;	}
						else if(ep_rs_form==5) 	{ /* rs_send(hold_value);  */	ex_hold_finish=0;	}
						else					{								ex_hold_finish=0;	}					
					}
					
					break;


				default:
					break;
			}
			#endif 
      	}
    }

}

void zero_function_key(void)
{
	volatile unsigned long 	zero_temp_long=0;
    long diff3=0;
	unsigned char	x_key=0;
	unsigned char	key_cnt=0;
    char key_check = 0;
	
	ex_weight_zero_flag=0;

	do 
	{
		zero_temp_long = read_filtered_adc(); 
	}while(ex_ad_flag==0);

	//ep_zero = zero_temp_long;
	zero_temp_long = (unsigned long) (((float) zero_temp_long) * ep_res_factor);

    if((zero_temp_long <= FirstZeroPlusLimit) &&  (zero_temp_long >= FirstZeroMinusLimit)) //calibration zero 10%
    {
        if(PowerOnZero >= ((ep_e_resolution/5)*ep_zero_range_value)) diff3 = (PowerOnZero - ((ep_e_resolution/5)*ep_zero_range_value));
        else diff3=0;
        if((zero_temp_long <= (PowerOnZero + (ep_e_resolution/5)*ep_zero_range_value)) 
            &&  (zero_temp_long >= diff3))   //Power zero 2%
        {
            tone_type=SHORT_TONE_PLAY;
            key_check = 1;
            if(ep_tare_flag==1)
            {
                ep_tare_value = ex_adc_org;
                ep_tare_flag=0; 
                eeprom_write(); 
                DelayMs(10);
                //;return;
            }
        }
            
        if(zero_temp_long > (ex_first_zero + ep_e_resolution/5*ep_zero_range_value)) 							// +2%
        { 
            tone_type=FOUR_TONE_PLAY;
            /*
            String_Display(WEIGHT, "CH 06");
            while(1)
            {
                do zero_temp_long = read_filtered_adc(); while(ex_ad_flag==0);
                zero_temp_long = (unsigned long) (((float) zero_temp_long) * ep_res_factor);
                if(zero_temp_long <= (ex_first_zero + ep_e_resolution/5*ep_zero_range_value)) 
              {
                break;
              }
              if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
              {
                x_key=Key_Data_Read();
                switch(x_key) 
                    {
                        case ENT_K:							//	1번 키 
                            key_check = 1;
                            goto ESCP;
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
                }*/
            }
	
        /*
        if(ex_first_zero > (ep_e_resolution/5*ep_zero_range_value))											// -2%
        { 
            if(zero_temp_long < (ex_first_zero - (ep_e_resolution/5*ep_zero_range_value))) 
            { 
                tone_type=FOUR_TONE_PLAY;

                String_Display(WEIGHT, "CH 06");
                while(1)
                {
                    do zero_temp_long = read_filtered_adc(); while(ex_ad_flag==0);
                    zero_temp_long = (unsigned long) (((float) zero_temp_long) * ep_res_factor);
                    if(zero_temp_long >= (ex_first_zero - ep_e_resolution/5*ep_zero_range_value)) 
                    {
                        break;
                    }
                    if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
                    {
                        x_key=Key_Data_Read();
                        switch(x_key) 
                        {
                            case ENT_K: 						//	1번 키 
                                key_check = 1;
                                goto ESCP;
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
                //return;
            } 
        }
     */
    }
ESCP:
    if(key_check)
    {
        ex_zero_tick=700;
        ex_adc_org = zero_temp_long;
    }
	//eeprom_write(); 
	//DelayMs(10); 
	return;
}




void key_tare_function_key(void)
{
	unsigned long tare_temp_long=weight_temp_long;

	
	ex_weight_key_tare_flag=0;

	
	tone_type=SHORT_TONE_PLAY;									
	
	tare_temp_long=0;
	tare_temp_long = five_byte(tare_temp_long);
	
	if(tare_temp_long)
	{
		ep_tare_value = (long)((float)(((tare_temp_long/weight_division)*20))/((float)ep_gravity_factor1/(float)ep_gravity_factor2))+ex_adc_org;
		ep_tare_flag=1;
	}
	else
	{
		ep_tare_value = ex_adc_org;
		ep_tare_flag=0;
	}
	
	ex_key_tare_tick=700;
	
	eeprom_write();
	
	DelayMs(10);
}



void tare_function_key(void)
{
	unsigned long tare_temp_long=weight_temp_long;

	
	ex_weight_tare_flag=0;


#if 0	// 최근 꺼 
	tare_x=0;
	while((key_code == HOLD_KEY) && tbi(key_status,key_press_f)) 
	{

		if(++tare_x > LONG_KEY_3SEC_TIME) 
		{	
			tone_type=SHORT_TONE_PLAY;									
		
			tare_temp_long=0;
			tare_temp_long = five_byte(tare_temp_long);
		
			if(tare_temp_long)
			{
				ep_tare_value = (long)((float)(((tare_temp_long/weight_division)*20))/((float)ep_gravity_factor1/(float)ep_gravity_factor2))+ex_adc_org;
				ep_tare_flag=1;
			}
			else
			{
				ep_tare_value = ex_adc_org;
				ep_tare_flag=0;
			}
		
			ex_key_tare_tick=700;
		
			eeprom_write();
			
			DelayMs(10);
		
			return;
		}

		else
		{
		}

		DelayMs(100);
	}
#endif 

#if 0
	ep_key_tare_set=0;	// kdell
	
	if(ep_key_tare_set)
	{
		x=0;
		while((key_code == HOLD_KEY) && tbi(key_status,key_press_f)) 
		{
			if(ex_delay_tick==0)
			{
				ex_delay_tick=100;

				if(++x > LONG_KEY_3SEC_TIME) 
				{	
					tone_type=SHORT_TONE_PLAY;									

					tare_temp_long=0;
					tare_temp_long = five_byte(tare_temp_long);

					if(tare_temp_long)
					{
						ep_tare_value = (long)((float)(((tare_temp_long/weight_division)*20))/((float)ep_gravity_factor1/(float)ep_gravity_factor2))+ex_adc_org;
						ep_tare_flag=1;
					}
					else
					{
						ep_tare_value = ex_adc_org;
						ep_tare_flag=0;
					}

					ex_key_tare_tick=700;

					eeprom_write();
					
					DelayMs(10);

					return;
				}
			}
		}
	}
#endif 



	

	if( (ex_sign_flag==1)&&(ep_tare_flag==0) )	// error -> (-)	
	{
		tone_type=FOUR_TONE_PLAY;									
		return;
	}

	if(ex_e_value==0)							// error -> zero 						
	{
		tone_type=FOUR_TONE_PLAY;									
		return;
	}

	if( (ex_sign_flag==1)&&(ep_tare_flag==1) )	// error -> (-)
	{ 
		ep_tare_value = ex_adc_org;
		ep_tare_flag=0; 
		eeprom_write(); 
		DelayMs(10);
		return;
	}

	if( tare_temp_long > (ep_e_resolution * 20) )	// error -> excess
	{ 
		tone_type=FOUR_TONE_PLAY;									
		return;
	}

	do											// read adc
	{
		tare_temp_long = read_filtered_adc(); 
	}while(ex_ad_flag==0);

	weight_temp_long = (unsigned long) ( ((float) tare_temp_long) * ep_res_factor );
	ep_tare_value = g_prev_adc1;
	ep_tare_flag=1;

	ex_key_tare_tick=700;

	eeprom_write(); 
	DelayMs(10);
}

void hold_function_key(void)
{
	
	ex_weight_hold_flag=0;
	ex_weight_hold_value =0; ex_hold_finish=0;
	if((ex_hold_time >=(ep_holdspeed*200)) && (ex_hold_on==1)) //기존 홀드 클리어
	{ 
		//lampdsp[8]=0; 
		ex_hold_flag=0; 
		ex_hold_on=0; 
		ex_hold_count=0;
        ex_hold_bar=0;
        ex_hold_time = 0;
	}
	ex_hold_count=0;
    ex_hold_time = 0;
	if((ex_e_value==0) && (ex_auto_hold_on==0)) //자동 홀드가 아닐때 자동 홀드로 설정
	{ 
		ex_auto_hold_on_tick=700;
		//ex_hold_flag=1; 
		ex_auto_hold_on=1; 
		ex_hold_on=0; 
	}
	else if((ex_e_value==0) && (ex_auto_hold_on==1))  //자동홀드 설정일때 자동홀드 해제
	{ 
		ex_auto_hold_off_tick=700;
		//ex_hold_flag=0;
		ex_auto_hold_on=0; 
		ex_hold_on=0; 
	}
	else if((ex_e_value>0) && !ex_sign_flag && (!ex_auto_hold_on) )  //수동 홀드
	{ 
        ex_hold_time = 0;
        ex_hold_finish = 0;
		ex_hold_flag=1; 
		ex_hold_on=1; 
	}
	//DelayMs(700);

	
	#if 0	// 남문 용 
	ex_weight_hold_flag=0;

	ex_weight_hold_value =0; 
	ex_hold_finish=0;
	if((ex_hold_count >= (ep_holdspeed/HOLD_COUNT_DIVIDE)) && (ex_hold_on==1)) 
	{
		//lampdsp[8]=0; 
		ex_hold_flag=0; 
		ex_hold_on=0; 
		ex_hold_count=0; 
		return; 
	}
	
	ex_hold_count=0;

	if(ex_e_value==0)
	{
		if(ex_auto_hold_on==0)
		{
			//stt_out('A', 15);
			//char_display(" AH ON");
			ex_auto_hold_on_tick=700;
			//lampdsp[8]=1; 
			ex_hold_flag=1; 
			ex_auto_hold_on=1; 
			ex_hold_on=0; 
		}
		else
		{
			//stt_out('B', 15);
			//char_display(" AHOFF"); 
			ex_auto_hold_off_tick=700;
			//lampdsp[8]=0; 
			ex_hold_flag=0; 
			ex_auto_hold_on=0; 
			ex_hold_on=0; 
		}

	}
	else
	{
		if(ex_sign_flag)									// '-' 무게일 경우 
		{
			if(ex_auto_hold_on==0)
			{
				//stt_out('A', 15);
				//char_display(" AH ON"); 
				//lampdsp[8]=1; 
				ex_hold_flag=1; 
				ex_auto_hold_on=1; 
				ex_hold_on=0; 
			}
			else
			{
				//stt_out('B', 15);
				//char_display(" AHOFF"); 
				//lampdsp[8]=0; 
				ex_hold_flag=0; 
				ex_auto_hold_on=0; 
				ex_hold_on=0; 
			}
		}
		else											// '+' 무게일 경우 
		{
			if(ex_e_value > (weight_division*ep_hold_zero))
			{
				if(ex_auto_hold_on==0)
				{
					//stt_out('C', 15);
					//char_display("  HOLD"); 
					//lampdsp[8]=1;				// 'HOLD' Icon Diplay : On
					ex_hold_flag=1; 
					ex_hold_on=1; 
					ex_hold_step=0;
				}
			}
			else
			{
				tone_type=TWO_TONE_PLAY;									
			}
		}
	}
	#endif 
}

//void hold_mode_step_0(void)
//{
//	filter_degree_temp = ep_filter_degree;	// 	다시 원래값으로 복귀 하기 위함 
//	ep_filter_degree=6;						// 	홀드일때는 6으로 변경 
//
//	adc_initial(); 							//	ad7190 초기화 
//
//	ex_hold_step=1;							//	다음 단계로 넘어감 
//}
//
//void hold_mode_step_1(void)
//{
//	ex_weight_hold_value_temp=ex_weight_hold_value=ex_e_value; 		// 	현재 무게 값을 hold value에 넣음 
//	ex_hold_count=1;					
//	ex_hold_step=2;							//	다음 단계로 넘어감 
//
//	//stt_out('J', 2);
//	ex_hold_bar=1;
//}

//void hold_bar_ing(void)
//{
//	while(ex_hold_interval_timer);
//	ex_hold_interval_timer = HOLD_INTERVAL_VALUE;	
//
//	if(		ex_hold_count == (char)((weight_h_speed*1.0)+0.5)){ /*stt_out('K', 2);*/ ex_hold_bar=2;	}
//	else if(ex_hold_count == (char)((weight_h_speed*2.0)+0.5)){ /*stt_out('L', 2);*/ ex_hold_bar=3; }
//	else if(ex_hold_count == (char)((weight_h_speed*3.0)+0.5)){ /*stt_out('M', 2);*/ ex_hold_bar=4; }
//	else if(ex_hold_count == (char)((weight_h_speed*4.0)+0.5)){ /*stt_out('N', 2);*/ ex_hold_bar=5; }
//
//	if(ex_hold_count == (ep_holdspeed/HOLD_COUNT_DIVIDE))
//	{
//		ex_hold_step=3;
//	}
//	else
//	{
//		ex_weight_hold_value = ex_weight_hold_value + ex_e_value; 
//		ex_hold_count++;
//	}
//}
//void hold_mode_step_3(void)
//{
//	ex_weight_hold_value = ex_weight_hold_value / ex_hold_count;				
//	ex_weight_hold_value = ex_weight_hold_value - (ex_weight_hold_value % weight_division);
//	ex_hold_finish=1;
//
//	ex_e_value=ex_weight_hold_value;							// 홀드 완료된 무게를 최대한 빨리  Display 하기 위함
//
//	ex_hold_bar=0;
//
//
//	ep_filter_degree=filter_degree_temp;				// 원래 값으로 복귀
//	adc_initial();
//}

void MCU_ADC_INIT(void)
{
	AD1CON1bits.SAMP = 0;		//ADC 샘플/홀드 홀딩
	AD1CON1bits.ASAM = 1;		//ADC 변환 즉시 샘플링 시작 설정
	AD1CON1bits.SSRC = 7;		//샘플 클럭 설정: 내부 카운터 샘플링 끝나고 변환시작
	AD1CON1bits.FORM = 0;		//데이터 출력 형식: Integer 
	AD1CON1bits.MODE12 = 0;		// 10-bit, 1-channel ADC operation
	AD1CON1bits.DMAEN = 0;
	AD1CON1bits.DMABM = 0;
	AD1CON1bits.ADSIDL = 0;

	AD1CON2bits.BUFM=0;			//항상 0번지 버퍼부터 변환 값 채우기 시작 설정
	AD1CON2bits.SMPI=15;		//매 16번 AD변환 완료 후, 인터럽트 발생 또는DMA 어드레스 증가시킴
	AD1CON2bits.CSCNA=1;		//CH0+ 입력에 대한 스캔 입력 설정: Scan inputs
	AD1CON2bits.PVCFG = 0;		//AD변환 기준 전압 설정: Vreg+ = Vdd, Vreg- = Vss

	AD1CON3bits.ADCS = 0x3F;	//ADC 변환 클럭 설정 : 최대치 설정 
	AD1CON3bits.SAMC=31; 		//Auto Sample Time = 31*Tad
	AD1CON3bits.ADRC=0;			//ADC Clock을 시스템 클럭 사용 설정

	AD1CON4bits.DMABL=0;		//아날로그 입력당 DMA 버퍼 영역 설정: 1 워드 설정

	AD1CHS0=0;					//디폴트 값
	AD1CSSL = 0x0800;			// AN11 입력 핀 설정
	IFS0bits.AD1IF = 0;			//ADC1 인터럽트 플래그 클리어
	IEC0bits.AD1IE = 1;			//ADC1 인터럽트 시작 설정
	AD1CON1bits.ADON = 1;		//ADC1 컨버터 동작 
}

void Low_Batt_check(void)
{
	if(low_battery==2)
	{
		String_Display(PAY, "  BAT ");	tone_type=SHORT_TONE_PLAY;	DelayMs(700);
		String_Display(PAY, " B A T");	tone_type=SHORT_TONE_PLAY;	DelayMs(700);
		String_Display(PAY, "  BAT "); 	tone_type=SHORT_TONE_PLAY;	DelayMs(700);
		
		BuzzerOff;
		PowerOff;
	}
}

long data_avg(long data, char init)
{
    static long Data[100], total;
    static unsigned char AvgCntRef, AvgCnt;
    unsigned char i;
    
    AvgCntRef = ep_filter_degree;
    if(AvgCnt >= AvgCntRef)AvgCnt = 0;
    Data[AvgCnt] = data;
    AvgCnt++;  
    
    if(!init)
    {
        total= 0;
        for(i=0; i<AvgCntRef; i++)
        {
            total = total + Data[i];
        }
    }
    else if(init == 1)
    {
        for(i=0; i<AvgCntRef; i++)
        {
            Data[i] = data;
        }
    }
    return total/AvgCntRef;
}

void AdcDataInit(void)
{
    long temp_long;
    
    do
    { 
        temp_long = read_filtered_adc(); 
    } 
    while(ex_ad_flag==0);
    data_avg(temp_long, 1);
    
}
