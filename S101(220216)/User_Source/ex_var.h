#ifndef _EX_VAR_H_
#define _EX_VAR_H_


extern unsigned  char 	ex_zero_flag;
extern unsigned char 	poff_sig;



// Key 
extern unsigned  char 	key_status;
extern unsigned  char 	key_code;
extern unsigned short 	key_scan_code;
extern unsigned char	ex_main_first_key_f;

extern unsigned char 	key_task_first_input_f;
extern unsigned char	key_task_key_f;
extern unsigned long	key_task_number;



// IR Key 
extern unsigned char	ir_key_release_timer;
extern unsigned char	ir_key_status;
extern unsigned char	ir_key_scan_code;
extern unsigned char	capture_count;
extern unsigned char	ir_key;
extern unsigned char	ic;

extern unsigned short	edge_low;
extern unsigned short	edge_high;
extern unsigned short	IC_buffer[24];

extern unsigned int		capture_interval_timer;
extern unsigned int 	capture_scan_data;
extern unsigned int 	capture_first_data;
extern unsigned int 	capture_second_data;
extern unsigned int 	capture_pulse_width;


// Buzzer 
extern unsigned int 	tone_type;
extern unsigned int 	buzzer_play_timer;

extern long            TestTemp1;
extern long            TestTemp2;
extern long            TestTemp3;
extern float            Testfloat1;


// Flash
extern unsigned long 	ex_page_addr;
extern unsigned long 	ex_byte_addr; 
extern unsigned long 	ex_scan_page; 
extern unsigned long 	ex_scan_byte;

extern unsigned long	f_w_page_addr;
extern unsigned char	f_w_byte_addr;
extern unsigned long	f_r_scan_addr;
extern unsigned char	f_r_byte_addr;


extern unsigned char 	ex_fw_buffer[20];
extern unsigned char 	ex_fr_buffer[20];


extern unsigned long 	ex_atflash_PluReadValue;
extern unsigned long 	ex_atflash_PluWriteValue;
extern unsigned char	ex_atflash_PluNum;	

extern unsigned char 	ex_atflash_PluValueAddress[100];

extern unsigned long	ex_read_total_sum;
extern unsigned char	same_product_sum_flag;


// LOAD CELL
extern unsigned long weight_temp_long;
extern unsigned char weight_division, weight_division_dual;

extern float weight_h_speed;
extern char filter_degree_temp;			// Hold 기능 동작할 때 v_filter_degree 값을 '6'으로 하여 안정적으로 AD값을 획득한후 원래값으로 복귀하기 위함

extern 		    float 	ep_res_factor;
extern 		    float 	ep_res_factor_dual;

extern unsigned  long 	ep_tare_value;
extern unsigned  long 	ep_e_resolution;
extern unsigned  long 	ep_e_resolution_dual;
extern unsigned  long 	ep_maximum_capacity;
extern unsigned  long 	ep_zero;


extern unsigned  long 	ex_weight_temp_long;
extern unsigned  long 	ex_e_value;
extern unsigned  long 	ex_adc_org;
extern unsigned  long 	ex_adc_org_dual;
extern unsigned  long 	ex_first_zero;
extern unsigned  long   ep_DualRangeBoundary;
extern long             PowerOnZero;
extern long             FirstZeroPlusLimit;
extern long             FirstZeroMinusLimit;

extern unsigned short 	ep_gravity_factor1;
extern unsigned short 	ep_gravity_factor2;

extern unsigned  char 	ep_minimum_division;
extern unsigned  char 	ep_minimum_division_dual;
extern unsigned  char 	ep_cal_no;
extern unsigned  char 	ep_tare_flag;

extern unsigned  char 	ex_ad_flag;
extern unsigned  char 	ex_stab_flag;

extern unsigned char 	ex_hold_finish;
extern unsigned char 	ex_hold_count;
extern unsigned char 	ex_hold_on;
extern unsigned char 	ex_hold_flag;
extern unsigned char 	ex_auto_hold_on;
extern unsigned char 	ex_weight_hold_flag;
extern unsigned char 	ex_hold_step;

extern unsigned short 	ex_hold_start_delay_timer;
extern unsigned short 	ex_hold_interval_timer;

extern unsigned long 	ex_weight_hold_value;

extern 	unsigned char	ex_weight_task_en_f;






// EEPR



//extern const char POINT_DATA[13];
extern const char 		POINT_DATA[21];


extern unsigned  char 	ep_filter_degree;
extern unsigned  long 	ep_stable;
extern unsigned  char 	ep_az_digit;
extern unsigned  char 	ep_az_second;
extern volatile unsigned  char 	ep_holdspeed;
extern unsigned  char 	ep_hold_start;
extern unsigned  char 	ep_hold_zero;
extern unsigned  char 	ep_star;
extern unsigned  char 	ep_id;
extern unsigned  char 	ep_rs_form;
extern unsigned  char 	ep_prt_form;
extern unsigned  char 	ep_feed;
extern unsigned short 	ep_wei_no;
extern unsigned  char 	ep_wireless;
extern unsigned  char 	ep_backlight;
extern unsigned  char 	ep_bright;
extern unsigned  char 	ep_wei_backup;
extern unsigned  long 	ep_short_plu;
extern unsigned  char 	ep_plu_no[100];
extern unsigned  long 	ep_plu_no_value[100];
extern unsigned  char 	ep_hold_range_value;
extern unsigned  char 	ep_zero_range_value;

extern unsigned  char 	ep_plu_unit;


extern unsigned char 	ex_temp_plu_no;
extern unsigned long 	ex_temp_u_p_value;


extern unsigned  char 	ep_unit;

extern unsigned char 	ep_key_tare_set;
extern unsigned char 	ex_key_tare_flag;
extern unsigned short 	ex_key_tare_tick;

extern unsigned short 	ex_auto_hold_on_tick;
extern unsigned short 	ex_auto_hold_off_tick;

extern unsigned short 	ex_weight_sum_blink_timer;


extern unsigned short 	ex_weight_sum_blink_cnt;

extern unsigned char    KeyTareEnable;
extern unsigned char    ep_MinusDiplayEnable;
extern unsigned char    ep_enter_key_set;



// timer
extern unsigned short 	autozero_count;
extern unsigned short	key_task_key_tim;
extern unsigned short	key_task_keyless_tim;
extern unsigned long	key_task_number_tmp;

// lamp
extern unsigned  char 	lamp_lcd3[5];
extern unsigned  char 	lamp_lcd2[5];
extern unsigned  char 	ex_no_weight_flag;
extern unsigned  char 	ex_weight_add_flag;
extern unsigned  char 	ex_weight_sum_blink_flag;
extern unsigned  char 	ex_weight_sum_flag;
extern unsigned  char 	ex_weight_batt_flag;
extern unsigned  char 	ex_weight_wireless_flag;
extern unsigned  char 	ex_weight_sum_lamp_flag;


// memo
extern unsigned char 	ep_final_plu_no;
extern unsigned char 	ex_weight_add_count;

// normal
extern unsigned char    print_flag;
extern unsigned char 	ex_weight_zero_flag;
extern unsigned char 	ex_weight_tare_flag;
extern unsigned char	ex_weight_key_tare_flag;

extern unsigned char 	ex_sign_flag;
extern unsigned  int 	ex_stb_count;
extern unsigned short 	ex_zero_tick;
extern unsigned short 	ex_weight_over_tick;
extern unsigned short 	ex_weight_minus_over_tick;

extern unsigned char 	ex_back_light_flag;
extern unsigned char 	none_point_flag;

extern unsigned char 	ex_calib_trace_flag;

extern unsigned char 	ex_hold_bar;

extern unsigned short 	ex_delay_tick;

extern unsigned long 	ex_weight_hold_value_temp;

extern unsigned char 	ex_wei_clr;

extern unsigned short 	ex_weight_add_tick;


extern unsigned long 	ex_serial_no;
extern unsigned long	ex_weight_add_value[500];
extern unsigned long 	ex_weight_sum_value;

extern unsigned short 	sum_value_page_address;
extern unsigned char 	ex_weight_total_sum_flag;


// Input
extern unsigned short 	number_two_input_step_timer;
extern unsigned char  	number_two_input_step;



extern unsigned char 	f_cnt;



// RTC
typedef struct _sRTC_Struct
{
	unsigned char year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
}sRTC_Struct ;

extern sRTC_Struct 		sRTC;


// Sub_Fuc
extern unsigned char 	ex_unit_price_input_flag;
extern unsigned char 	ex_unit_price_call_flag;
extern unsigned char 	ex_same_product_flag;

extern unsigned long 	ex_same_product_sum_final;
extern unsigned long 	ex_money_change_value;


//	UART
extern unsigned char 	sin_buffer1[21];
extern unsigned char 	sin_buffer3[40];
extern unsigned char 	v_rs_flag1;
extern unsigned char 	sin_count1;
extern unsigned char 	sin_count3;
extern unsigned char 	v_echo;
extern unsigned char 	get_char;
extern unsigned char 	v_key_flag;
extern unsigned long 	print_total_money;
extern unsigned short 	print_count;
extern unsigned char    Rx3_flg;

//	TEST
extern unsigned char	test_lcd_mode_f;




// Low_Batt
extern unsigned char	low_battery;
extern unsigned char	batt_check_sig;
extern unsigned char	batt_count;
extern unsigned short	batt_buf[50];
extern unsigned short	batt_value;
extern unsigned short	divider_timer_500ms;


// RTC
extern unsigned char	ep_rtc_hour;
extern unsigned char	ep_rtc_minute;


// EEPR
extern unsigned char	ep_long_key_time;
extern unsigned char    cut_cipher;
extern unsigned char    round_off;

extern unsigned long    cut_pay;
extern unsigned long    ex_hold_time;
extern unsigned long    pay_price;
#endif 
