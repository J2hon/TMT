#include <p24FJ64GA306.h>
#include "ex_var.h"



_CONFIG1(JTAGEN_OFF & FWDTEN_WDT_SW & WDTPS_PS32 & FWPSA_PR128);  	// JTAG off, Emulator PGEC1/PGED1, Watchdog timer active 128mS
_CONFIG2(FNOSC_FRC & POSCMD_NONE & FCKSM_CSDCMD & OSCIOFCN_ON);   	// Primary HS, 8Mhz 
_CONFIG3(BOREN_ON & SOSCSEL_ON); 										// Brown out reset ON
_CONFIG4(DSWDTPS_DSWDTPS0 & DSBOREN_OFF & DSWDTEN_OFF & DSSWEN_OFF); 	// Brown out reset ON







const char POINT_DATA[21]=
{
	0x81,	//	[0]		0.0001
	0x82,	//	[1]		0.0002
	0x85,	//	[2]		0.0005
	0x61,	//	[3]		0.001
	0x62,	//	[4]		0.002
	0x65,	//	[5]		0.005						
	0x41,	//	[6]		0.01
	0x42,	//	[7]		0.02
	0x45, 	//	[8]		0.05
	0x21, 	//	[9]		0.1
	0x22, 	//	[10]		0.2
	0x25,	//	[11]		0.5				
	0x01, 	//	[12]		1
	0x02, 	//	[13]		2
	0x05, 	//	[14]		5
	0x09, 	//	[15]		10
	0x0A, 	//	[16]		20
	0x0D,	//	[17]		50			
	0x11,	//	[18]		100
	0x12,	//	[19]		200
	0x15 	//	[20]		500			
};





// flag
unsigned char 	ex_zero_flag=0;
unsigned char 	poff_sig=0;



// Key 
unsigned  char 	key_status=0;
unsigned  char 	key_code=0;
unsigned short 	key_scan_code=0;
unsigned char	ex_main_first_key_f=1;

unsigned char 	key_task_first_input_f=0;
unsigned char	key_task_key_f=0;
unsigned long	key_task_number=0;
unsigned long	key_task_number_tmp=0;





// IR Key 
unsigned char 	ir_key_release_timer=0;
unsigned char 	ir_key_status=0;
unsigned char 	ir_key_scan_code=0;
unsigned char 	capture_count=0;
unsigned char 	ir_key=0;
unsigned char 	ic=0;

unsigned short	edge_low=0;
unsigned short	edge_high=0;
unsigned short	IC_buffer[24]={0};

unsigned int 	capture_interval_timer=0;
unsigned int 	capture_scan_data=0;
unsigned int 	capture_first_data=0;
unsigned int 	capture_second_data=0;
unsigned int 	capture_pulse_width=0;



// Buzzer 
unsigned int 	tone_type=0;
unsigned int 	buzzer_play_timer=0;



// Load Cell

unsigned long weight_temp_long;
unsigned char weight_division, weight_division_dual;

float weight_h_speed;
char filter_degree_temp;			// Hold 기능 동작할 때 v_filter_degree 값을 '6'으로 하여 안정적으로 AD값을 획득한후 원래값으로 복귀하기 위함 

float 	ep_res_factor=0;
float 	ep_res_factor_dual=0;

unsigned  long 	ep_tare_value=0;
unsigned  long 	ep_e_resolution=0;
unsigned  long 	ep_e_resolution_dual=0;
unsigned  long 	ep_maximum_capacity=0;
unsigned  long 	ep_zero=0;
unsigned  long  ep_DualRangeBoundary=0;


unsigned  long 	ex_weight_temp_long=0;
unsigned  long 	ex_e_value=0;
unsigned  long 	ex_adc_org=0;
unsigned  long 	ex_adc_org_dual=0;
unsigned  long 	ex_first_zero=0;
long            PowerOnZero=0;
long            FirstZeroPlusLimit=0;
long            FirstZeroMinusLimit=0;

unsigned short 	ep_gravity_factor1=0;
unsigned short 	ep_gravity_factor2=0;


unsigned  char 	ep_minimum_division=0;
unsigned  char 	ep_minimum_division_dual=0;
unsigned  char 	ep_cal_no=0;
unsigned  char 	ep_tare_flag=0;

unsigned  char 	ex_ad_flag=0;
unsigned  char 	ex_stab_flag=0;

unsigned  char 	ep_plu_no[100]={0};
unsigned  long 	ep_plu_no_value[100]={0};

unsigned char 	ex_temp_plu_no=0;
unsigned long 	ex_temp_u_p_value=0;

unsigned char 	ex_hold_finish=0;
unsigned char 	ex_hold_count=0;
unsigned char 	ex_hold_on=0;
unsigned char 	ex_hold_flag=0;
unsigned char 	ex_auto_hold_on=0;
unsigned char 	ex_weight_hold_flag=0;
unsigned char 	ex_hold_step=0;

unsigned short 	ex_hold_start_delay_timer=0;
unsigned short 	ex_hold_interval_timer=0;
unsigned short 	ex_weight_sum_blink_timer=0;

unsigned long 	ex_weight_hold_value=0;


unsigned short 	ex_weight_sum_blink_cnt=0;

unsigned char	ex_weight_task_en_f=0;

long            TestTemp1;
long            TestTemp2;
long            TestTemp3;
float            Testfloat1;




// Flash
unsigned long 	ex_page_addr=0;   
unsigned long 	ex_byte_addr=0;
unsigned long 	ex_scan_page=0;
unsigned long 	ex_scan_byte=0;

unsigned long	f_w_page_addr=0;
unsigned char	f_w_byte_addr=0;
unsigned long	f_r_scan_addr=0;
unsigned char	f_r_byte_addr=0;



unsigned char 	ex_fw_buffer[20]={0}; 
unsigned char 	ex_fr_buffer[20]={0};

unsigned long 	ex_atflash_PluReadValue=0;
unsigned long 	ex_atflash_PluWriteValue=0;
unsigned char	ex_atflash_PluNum=0;	
unsigned char 	ex_atflash_PluValueAddress[100]={0};

unsigned long	ex_read_total_sum=0;
unsigned char	same_product_sum_flag=0;



// SET_MODE

unsigned  char 	ep_filter_degree=0;
unsigned  long 	ep_stable=0;
unsigned  char 	ep_az_digit=0;
unsigned  char 	ep_az_second=0;
volatile unsigned  char 	ep_holdspeed=0;
unsigned  char 	ep_hold_start=0;
unsigned  char 	ep_hold_zero=0;
unsigned  char 	ep_star=0;
unsigned  char 	ep_id=0;
unsigned  char 	ep_rs_form=0;
unsigned  char 	ep_prt_form=0;
unsigned  char 	ep_feed=0;
unsigned short 	ep_wei_no=0;
unsigned  char 	ep_wireless=0;
unsigned  char 	ep_backlight=0;
unsigned  char 	ep_bright=0;
unsigned  char 	ep_wei_backup=0;
unsigned  char 	ep_unit=0;
unsigned  char 	ep_hold_range_value=0;
unsigned  char 	ep_zero_range_value=0;
unsigned  char 	ep_plu_unit=0;
unsigned  char  KeyTareEnable=0;
unsigned  char  ep_MinusDiplayEnable=0;
unsigned  char  ep_enter_key_set=0; 

unsigned char 	ep_key_tare_set=0;

unsigned char 	ex_wei_clr=0;


// timer
unsigned short 	autozero_count=0;
unsigned short	key_task_key_tim=0;
unsigned short	key_task_keyless_tim=0;




// memo
unsigned long 	ep_short_plu=0;
unsigned char 	ep_final_plu_no=0;
unsigned char 	ex_weight_add_count=0;










// lamp
unsigned  char 	lamp_lcd3[5]={0};
unsigned  char 	lamp_lcd2[5]={0};
unsigned  char 	ex_no_weight_flag=0;
unsigned  char 	ex_weight_add_flag=0;
unsigned  char 	ex_weight_sum_blink_flag=0;
unsigned  char 	ex_weight_sum_flag=0;
unsigned  char	ex_weight_batt_flag=0;
unsigned  char	ex_weight_wireless_flag=0;
unsigned  char	ex_weight_sum_lamp_flag=0;



// normal
unsigned char 	print_flag=0;
unsigned char 	ex_weight_zero_flag=0;
unsigned char 	ex_weight_tare_flag=0;
unsigned char	ex_weight_key_tare_flag=0;
unsigned char 	ex_sign_flag=0;
unsigned  int 	ex_stb_count=0;
unsigned short 	ex_zero_tick=0;
unsigned short 	ex_weight_over_tick=0;
unsigned short  ex_weight_minus_over_tick=0;

unsigned char 	ex_back_light_flag=0;
unsigned char 	none_point_flag=0;

unsigned char	ex_calib_trace_flag=0;
	
unsigned char 	ex_hold_bar=0;

unsigned char 	ex_key_tare_flag=0;
unsigned short 	ex_key_tare_tick=0;

unsigned short 	ex_delay_tick=0;

unsigned long 	ex_weight_hold_value_temp=0;

unsigned short 	ex_auto_hold_on_tick=0;
unsigned short 	ex_auto_hold_off_tick=0;

unsigned short 	ex_weight_add_tick=0;

unsigned long 	ex_serial_no=0;							//	50000 회까지 카운트 
unsigned long	ex_weight_add_value[500]={0};
unsigned long 	ex_weight_sum_value=0;

unsigned short 	sum_value_page_address=1;

unsigned char 	ex_weight_total_sum_flag=0;




// Input
unsigned short 	number_two_input_step_timer=0;
unsigned char  	number_two_input_step=0;



unsigned char 	f_cnt=0;



sRTC_Struct 	sRTC={0};




// Sub_Fuc
unsigned char 	ex_unit_price_input_flag=0;
unsigned char 	ex_unit_price_call_flag=0;
unsigned char 	ex_same_product_flag=0;

unsigned long 	ex_same_product_sum_final=0;

unsigned long 	ex_money_change_value=0;


//	UART
unsigned char 	sin_buffer1[21]={0};
unsigned char 	sin_buffer3[40];
unsigned char 	v_rs_flag1=0;
unsigned char 	sin_count1=0;
unsigned char 	sin_count3;
unsigned char 	v_echo=0;
unsigned char 	get_char=0;
unsigned char 	v_key_flag=0;
unsigned long 	print_total_money=0;
unsigned short 	print_count=0;
unsigned char   Rx3_flg;



//	TEST MODE
unsigned char	test_lcd_mode_f=0;



// Low_Batt
unsigned char	low_battery=0;
unsigned char	batt_check_sig=0;
unsigned char	batt_count=0;
unsigned short	batt_buf[50]={0};
unsigned short	batt_value=0;
unsigned short	divider_timer_500ms=0;


// RTC
unsigned char	ep_rtc_hour=0;
unsigned char	ep_rtc_minute=0;

// EEPR
unsigned char	ep_long_key_time=0;

unsigned char    cut_cipher = 0;
unsigned char   round_off = 0;

//display
unsigned long   cut_pay = 0;
unsigned long   ex_hold_time = 0;
unsigned long   pay_price = 0;
