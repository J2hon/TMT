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



unsigned long tog_cnt=0;


void _ISR __attribute__ ((interrupt,__no_auto_psv__)) _T2Interrupt (void)			// 0.5ms Interrupt
{
	IFS0bits.T2IF = 0;										// Interrupt Flag Clear
	ClrWdt();												// WDT timer Clear

	static unsigned char timer2_job_seq;
	unsigned char i;


/*
[1]		2]		3]		[4]		[5]		[6]		[7]		[8]
0.5		1		1.5		2		2.5		3		3.5		4
4.5		5		5.5		6		6.5		7		7.5		8
8.5		9		9.5		10		10.5	11		11.5	12

tick		-> 0.5mSec

1mSec 	-> 1,3,5,7
2mSec 	-> 2,6
4mSec 	-> 4
reflesh 	-> 8
*/

	if(poff_sig==1) 
	{
		tone_type=0;
		lcd_initial();
		BackLight_Off;
	}

    if(ex_hold_flag)
    {
        if(!ex_hold_finish) ex_hold_time++;
    }
    	
	switch(++timer2_job_seq)	
	{
		case 1: 											
		case 3:
		case 5:
		case 7:
			
			
			if( (key_task_key_f==1)&&(key_task_key_tim>0) )			key_task_key_tim--;
			else if( (key_task_key_f==1)&&(key_task_key_tim==0) )	key_task_key_tim=1000;


			if( (key_task_key_f==1)&&(key_task_keyless_tim>0) )									
			{
				key_task_keyless_tim--;
			}
			else if( (key_task_key_f==1)&&(key_task_keyless_tim==0)&&(key_task_number==key_task_number_tmp) )	
			{
                short_u_p_input();
				key_task_key_f=0;
				key_task_number=0;
			}

			/*
			if(ex_hold_interval_timer)
			{
				ex_hold_interval_timer--;
			}
			*/
			autozero_count++;
			if( autozero_count > 10000) autozero_count = 0; // 10 sec
			
			if(ex_zero_tick)		ex_zero_tick--;
			if(ex_weight_over_tick)	ex_weight_over_tick--;
			if(ex_key_tare_tick)	ex_key_tare_tick--;
			if(ex_delay_tick)		ex_delay_tick--;
			if(ex_auto_hold_on_tick)	ex_auto_hold_on_tick--;
			if(ex_auto_hold_off_tick)	ex_auto_hold_off_tick--;
            if(ex_weight_minus_over_tick) ex_weight_minus_over_tick--;
			//if(ex_weight_add_tick)		ex_weight_add_tick--;

			if(number_two_input_step_timer)	number_two_input_step_timer--;

			
			if(ex_hold_start_delay_timer)
			{
				ex_hold_start_delay_timer--;
			}

			if(ex_weight_sum_blink_flag==1)
			{
				if(ex_weight_sum_blink_timer==0)	
				{
					ex_weight_sum_blink_timer=500;
					ex_weight_sum_blink_cnt++;
				}
				
				ex_weight_sum_blink_timer--;
			}
			
			break;

		case 2: 											
		case 6:
			Tone_Generation();								// Tone-Generation

			ex_stb_count++;
			if( ex_stb_count > 2500) ex_stb_count = 0; 		// 5 sec


			if(ex_hold_start_delay_timer==0)					//auto hold에서 초기 10%이상 변화를 체크하기 위해 Delay 해줌
			{
				if(ex_auto_hold_on && (ex_hold_on==0))
				{
					//lampdsp[8]=1; 
					ex_hold_flag=1; 
					ex_hold_on=1; 
					ex_hold_count=0; 
					ex_hold_step=0;
                    ex_hold_time = 0;
				}
			}

			
			if(divider_timer_500ms++>125)					// 125 x 4ms = 500ms
			{
				divider_timer_500ms=0;


				if(low_battery==1)
				{
					ex_weight_batt_flag=1;
				}
				else if(low_battery==0)
				{
					ex_weight_batt_flag=0;
				}
			}

			break;

		case 4:											// 4*500= 2Sec	
			#if 0
			tog_cnt++;
			if( (tog_cnt%2)==0)	sbi(LATB, 10);		// Delk
			if( (tog_cnt%2)==1)	cbi(LATB, 10);		// Delk
			tgbi(LATB, 10);		// Delk
			#endif 

		
			Key_Scan();


			if(ir_key_release_timer>0)
			{
				--ir_key_release_timer;
			}


			if( ( (((PORTB&(0x01<<POWER_SW))==BV(POWER_SW))==0) || (ir_key == POWER_K) ) && (poff_sig) )
			{
				RCONbits.SWDTEN=0;
				ATflash_Busy_Flag;
				sbi(LATB, BUZZER);	
				cbi(LATB, POWER_ON);
			} 




			break;
			
		case 8: 											
			timer2_job_seq=0;

			if(buzzer_play_timer>0)							// Tone Generation Time Decrement
			{
				--buzzer_play_timer;
			}

			
			break;	

		default:
			timer2_job_seq = 0;
			break;
	}
}




void _ISR __attribute__((interrupt,__no_auto_psv__)) _U1RXInterrupt(void)
{
	get_char = U1RXREG;


	if(v_echo==1)///////////////////////////////////////////////////////////////
	{
		if(sin_count1>7) 
		{
			sin_count1=0;  
		}

		if( (get_char==0x80)&&(sin_count1 != 1) ) 
		{ 
			sin_count1=0; 
			IFS0bits.U1RXIF=0;

			return;
		}

		sin_buffer1[sin_count1]=get_char;
		sin_count1++;

		if(sin_count1==2)
		{
			if(sin_buffer1[1]==0x80) 
			{ 
				v_key_flag=1;
				IFS0bits.U1RXIF=0;

				return;
			}
			else 
			{ 
				buffer1_initial(); 
				IFS0bits.U1RXIF=0; 

				return;
			}
		}
		
		IFS0bits.U1RXIF=0; 
	}
	else if(v_echo==0)//////////////////////////////////////////////////////////
	{
		if(sin_count1>7) sin_count1=0;  

		if(get_char=='S')       
		{ 
			sin_count1=0; 
			IFS0bits.U1RXIF=0; 

			return;
		}
		else if(get_char==0x82) 
		{
			sin_count1=5; 
			IFS0bits.U1RXIF=0; 

			return;
		}

		sin_buffer1[sin_count1]=get_char;
		sin_count1++;

		if(sin_count1==8)
		{
			if( ((sin_buffer1[6]== 0x0D)&&(sin_buffer1[7]== 0x0A))||(sin_buffer1[7]== (unsigned char)(0x82+sin_buffer1[5]+sin_buffer1[6])) ) 
			{ 
				v_rs_flag1=1;
				IFS0bits.U1RXIF=0; 

				return; 
			}
			else 
			{ 
				buffer1_initial(); 
				IFS0bits.U1RXIF=0;

				return;
			}
		}
		
		IFS0bits.U1RXIF=0; 
	}
	else if(v_echo==2)//////////////////////////////////////////////////////////
	{
		if(sin_count1>20) sin_count1=0;  

		if(sin_count1==0) 
		{
			if( ((get_char=='O')||(get_char=='C'))==0 )  
			{ 
				sin_count1=0; 
				IFS0bits.U1RXIF=0; 

				return; 
			}
		}
		
		sin_buffer1[sin_count1]=get_char;
		sin_count1++;    

		if(sin_count1==3)
		{
			if(sin_buffer1[2]==0x0D) 
			{
				v_rs_flag1=1; 
				IFS0bits.U1RXIF=0;

				return;
			}
		}

		if(sin_count1==21)
		{
			if(sin_buffer1[20]==0x0D) 
			{ 
				v_rs_flag1=1; 
				IFS0bits.U1RXIF=0;

				return;
			}
			else 
			{ 
				sin_count1=0; 
				IFS0bits.U1RXIF=0; 

				return;
			}
		}
		
		IFS0bits.U1RXIF=0;
	}
}


void _ISR __attribute__((interrupt,__no_auto_psv__)) _U2RXInterrupt(void)
{
	//unsigned char	get_char2;

	//get_char2 = U2RXREG;

	IFS1bits.U2RXIF=0;
}

void _ISR __attribute__((interrupt,__no_auto_psv__)) _U3RXInterrupt(void)
{
	unsigned char get_char3;

	get_char3 = U3RXREG;

    if(get_char3 == 0x02) 
    {
        sin_count3 = 0;
    }
	else if(get_char3 == 0x03) 
    {
        Rx3_flg = 1;
        sin_count3 = 0;
    }
    else 
    {
        sin_buffer3[sin_count3] = get_char3;
        sin_count3++;
    }

	IFS5bits.U3RXIF=0;
}

void _ISR __attribute__((interrupt,__no_auto_psv__)) _U4RXInterrupt(void)
{
	//s_out3('K');	s_out3('H');	s_out3(U4RXREG);	
	
	IFS5bits.U4RXIF=0;
}





void _ISR __attribute__ ((interrupt,__no_auto_psv__)) _IC1Interrupt (void)
{
	unsigned short	pulse_width=0;
	
	IFS0bits.IC1IF = 0;

	if(ic>21) ic=0;

	edge_low = edge_high;
	edge_high = IC1BUF;
	pulse_width = edge_high - edge_low;

	if( (pulse_width>6500)||(pulse_width<900) ) 
	{
		ic=0; return; 
	} 
	
	if(pulse_width > 3000)	IC_buffer[ic] = 1;
	else                   	IC_buffer[ic] = 0;


	if(++ic==22)
	{
		if( (IC_buffer[ 0]==1)&&(IC_buffer[ 2]==1)&&(IC_buffer[ 4]==0)&&(IC_buffer[ 6]==0)&&(IC_buffer[ 8]==1)&&(IC_buffer[10]==0)&&(IC_buffer[12]==0)&&(IC_buffer[14]==0)&&(IC_buffer[16]==0)&&(IC_buffer[18]==0)&&(IC_buffer[20]==1) ) 
		{ 
			ir_key = POWER_K; 
			ir_key_release_timer=IR_KEY_RELEASE_TIME; 
			return; 
		}  
		else if( (IC_buffer[ 0]==1)&&(IC_buffer[ 2]==1)&&(IC_buffer[ 4]==0)&&(IC_buffer[ 6]==1)&&(IC_buffer[ 8]==0)&&(IC_buffer[10]==0)&&(IC_buffer[12]==0)&&(IC_buffer[14]==0)&&(IC_buffer[16]==0)&&(IC_buffer[18]==1)&&(IC_buffer[20]==0) ) 
		{ 
			ir_key = ZERO_KEY;
			ir_key_release_timer=IR_KEY_RELEASE_TIME; 
			return; 
		}  
		else if( (IC_buffer[ 0]==1)&&(IC_buffer[ 2]==1)&&(IC_buffer[ 4]==0)&&(IC_buffer[ 6]==1)&&(IC_buffer[ 8]==0)&&(IC_buffer[10]==0)&&(IC_buffer[12]==0)&&(IC_buffer[14]==0)&&(IC_buffer[16]==0)&&(IC_buffer[18]==0)&&(IC_buffer[20]==1) ) 
		{ 
			ir_key = HOLD_KEY; 
			ir_key_release_timer=IR_KEY_RELEASE_TIME; 
			return; 
		}  
		else if( (IC_buffer[ 0]==1)&&(IC_buffer[ 2]==1)&&(IC_buffer[ 4]==0)&&(IC_buffer[ 6]==1)&&(IC_buffer[ 8]==0)&&(IC_buffer[10]==0)&&(IC_buffer[12]==0)&&(IC_buffer[14]==1)&&(IC_buffer[16]==0)&&(IC_buffer[18]==0)&&(IC_buffer[20]==0) ) 
		{ 
			ir_key = ADD_KEY; 
			ir_key_release_timer=IR_KEY_RELEASE_TIME; 
			return; 
		}  
		else if( (IC_buffer[ 0]==1)&&(IC_buffer[ 2]==1)&&(IC_buffer[ 4]==0)&&(IC_buffer[ 6]==1)&&(IC_buffer[ 8]==0)&&(IC_buffer[10]==0)&&(IC_buffer[12]==0)&&(IC_buffer[14]==0)&&(IC_buffer[16]==1)&&(IC_buffer[18]==0)&&(IC_buffer[20]==0) )
		{ 
			ir_key = ENT_K; 
			ir_key_release_timer=IR_KEY_RELEASE_TIME; 
			return;
		}  
		else if( (IC_buffer[ 0]==1)&&(IC_buffer[ 2]==1)&&(IC_buffer[ 4]==0)&&(IC_buffer[ 6]==1)&&(IC_buffer[ 8]==0)&&(IC_buffer[10]==0)&&(IC_buffer[12]==1)&&(IC_buffer[14]==0)&&(IC_buffer[16]==0)&&(IC_buffer[18]==0)&&(IC_buffer[20]==0) )
		{ 
			ir_key = SAVE_KEY; 
			ir_key_release_timer=IR_KEY_RELEASE_TIME; 
			return; 
		}  
		else 
		{ 
			IC_buffer_clr(); 
			cbi(ir_key_status,ir_key_detect_f);	
			return; 
		}
	}
}


unsigned char	adc1_loop=0;
void __attribute__((interrupt,__no_auto_psv__)) _ADC1Interrupt(void)
{

	batt_buf[batt_count] = ADC1BUF0;
	batt_count++;
	batt_value=0;
	if(batt_count==50) 
	{	
		batt_count=0;
		for(adc1_loop=0; adc1_loop<50; adc1_loop++)
		{
			batt_value = batt_value + batt_buf[adc1_loop];
		}
		
		batt_value = batt_value / 50;

		if(batt_check_sig)
		{
			if(batt_value >= 393)						// Low Battery Mode : 해제
			{
				ex_weight_batt_flag=0;
										
			}
			else if(batt_value>367)						// Low Battery Mode 1 : 배터리 Icon만  깜빡이게 함
			{
				ex_weight_batt_flag = 1;
			}
			else										// Low Battery Mode 2 : Buzzer 발생후 전원 OFF
			{
				ex_weight_batt_flag=1;					// Low Batter Icon : Display On				
				low_battery = 2;
			}
		}
	}

	IFS0bits.AD1IF = 0;
}













