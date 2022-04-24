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





// LCD
#define LCD_IRQ_1	3 // RD3
#define LCD_IRQ_2	4 // RD4
#define LCD_CS_1	5 // RD5
#define LCD_CS_2	6 // RD6
#define LCD_WR      7 // RD7
#define LCD_DATA	8 // RD8



unsigned long i=0;

const char LCD_COMMAND[5] = 
{ 
	0x29, 
	0x04,
	0x05,
	0x01,
	0x03
};

//********************************************************************* CONSTANT 
unsigned char lampdsp[16] = 
{
	0,	//	[0]	batt
	0,	//	[1]	high
	0,	//	[2]	ok
	0,	//	[3]	low
	0,	//	[4]	zero
	0,	//	[5]	s1
	0,	//	[6]	tare
	0,	//	[7] 	s2
	0,	//	[8]	hold
	0,	//	[9]	s3
	0,	//	[10]	net
	0,	//	[11]	wireless
	0,	//	[12]	st
	0,	//	[13]	lb
	0,	//	[14]	k
	0	//	[15]	g
}; 
//****************************************************************************//


//****************************************************************************//
const char ASCII_FONT[59]=
{    
	0x00,	//	[0] SPACE
	0x00,	//	[1] DUMM
	0x00,	//	[2] DUMM
	0x00,	//	[3] DUMM
	0x00,	//	[4] DUMM
	0x00,	//	[5] DUMM
	0x00,	//	[6] DUMM
	0x00,	//	[7] DUMM
	0x00,	//	[8] DUMM
	0x00,	//	[9] DUMM
	0x00,	//	[10] DUMM
	0x00,	//	[11] DUMM
	0x00,	//	[12] DUMM
	0x04,	//	[13] -
	0x00,	//	[14] DUMM
	0x00,	//	[15] DUMM
	0xEB, 	//	[16] 0
	0x60, 	//	[17] 1
	0xC7, 	//	[18] 2
	0xE5, 	//	[19] 3
	0x6C, 	//	[20] 4
	0xAD, 	//	[21] 5
	0xAF, 	//	[22] 6
	0xE8, 	//	[23] 7
	0xEF, 	//	[24] 8
	0xED,	//	[25] 9
	0x00,	//	[26] DUMM
	0x00,	//	[27] DUMM
	0x00,	//	[28] DUMM
	0x05,	//	[29] =
	0x00,	//	[30] DUMM
	0x00,	//	[31] DUMM
	0x00,	//	[32] DUMM
	0xEE,	//	[33] A
	0x2F,	//	[34] B
	0x8B,	//	[35] C
	0x67,	//	[36] D
	0x8F,	//	[37] E
	0x8E,	//	[38] F
	0xAB,	//	[39] G
	0x2E,	//	[40] H
	0x20,	//	[41] I
	0x63,	//	[42] J
	0x0E,	//	[43] K
	0x0B,	//	[44] L
	0xA6,	//	[45] M
	0x26,	//	[46] N
	0x27,	//	[47] O
	0xCE,	//	[48] P
	0xEC,	//	[49] Q
	0x06,	//	[50] R
	0xAD,	//	[51] S
	0x0F, 	//	[52] T
	0x6B,	//	[53] U
	0x23,	//	[54] V
	0xA3,	//	[55] W
	0x6E,	//	[56] X
	0x6D,	//	[57] Y
	0xC7  	//	[59] Z
};
//****************************************************************************//


//****************************************************************************//
#if 0
const char POINT_DATA[21] = 
{ 
	0x81, 0x82, 0x85, 0x61, 0x62, 0x65,
	0x41, 0x42, 0x45, 0x21, 0x22, 0x25,
	0x01, 0x02, 0x05, 0x09, 0x0A, 0x0D, 
	0x11, 0x12, 0x15
};
#endif 

#if 0
const char POINT_DATA[21] = 
{ 
	0x81, 														// *10000
	0x82, 0x85, 		// 0.0002[1]	0.0005[2]				// *10000
	0x61, 0x62, 0x65,	// 0.001[3]		0.002[4]	0.005[5]	// *1000
	0x41, 0x42, 0x45, 	// 0.01[6]		0.02[7]		0.05[8]		// *100
	0x21, 0x22, 0x25,	// 0.1[9]		0.2	[10]	0.5[11]		// *10
	0x01, 0x02, 0x05, 	// 1[12]		2[13]		5[14]
	0x09, 0x0A, 0x0D, 	// 10[15]		20[16]		50[17]
	0x11, 				// 100[18]
	0x12, 0x15  
};
#endif 
//****************************************************************************//

const unsigned char WEIGHT_NUMBER[10] = 
{
	// 0  1     2     3     4     5     6     7     8     9
	0xEB, 0x60, 0xC7, 0xE5, 0x6C, 0xAD, 0xAF, 0xE8, 0xEF, 0xED
};

const unsigned char DISPLAY_FONT[] =
{

};



void weight_up_lamp(void)	// 
{
	unsigned char	loop=0;
	unsigned char	weight_lamp_position[4]={0x38, 0x3C, 0x40, 0x48};	
	unsigned char	u_p_lamp_position[4]={0x44, 0x4C, 0x78, 0x7C};

#if 1
	for(loop=0; loop<4; loop++)
	{
		sbi( LATD, LCD_CS_2 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_2 );							

		lcd_command(WRITE_MODE, 3); 					// command 101
		lcd_command(weight_lamp_position[loop],6); 		// address
		lcd_command(lamp_lcd3[loop], 4); 					// number

		sbi( LATD, LCD_CS_2 ); 
	}
#endif 

#if 1
	for(loop=0; loop<4; loop++)
	{
		sbi( LATD, LCD_CS_2 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_2 );							

		lcd_command(WRITE_MODE, 3); 					// command 101
		lcd_command(u_p_lamp_position[loop],6); 		// address
		lcd_command(lamp_lcd2[loop], 4); 					// number

		sbi( LATD, LCD_CS_2 ); 
	}
#endif 
}

void test_lcd_seg_on(void)
{
	unsigned char 	i=0;
	unsigned char	lcd2_3_position=0x00;
	unsigned char	lcd1_position=0x00;

	
	for(i=0; i<16; i++)
	{
		sbi( LATD, LCD_CS_2 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_2 );							

		lcd_command(WRITE_MODE, 3); 					// command 101
		lcd_command(lcd2_3_position,6); 				// address
		lcd_command(0xFF, 8); 							// number

		sbi( LATD, LCD_CS_2 ); 

		lcd2_3_position += 8;
	}

	for(i=0; i<16; i++)
	{
		sbi( LATD, LCD_CS_1 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_1 );

		lcd_command(WRITE_MODE, 3); 					// command 101
		lcd_command(lcd1_position,6); 					// address
		lcd_command(0xFF,  8); 							// number

		lcd1_position += 8;

		sbi( LATD, LCD_CS_1 ); 
	}
}

void test_lcd_part_off(void)
{
	unsigned char 	i=0;
	unsigned char	lcd2_3_position[3]={0x44, 0x4C, 0x70};
	unsigned char	lcd1_position[6]={0x28, 0x30, 0x34, 0x38, 0x3C, 0x40};
	unsigned char	data=0;

	
	for(i=0; i<3; i++)
	{
		if(i==0)		data=0x00;
		else if(i==1)	data=0x50;
		else if(i==2)	data=0xE0;
		
		sbi( LATD, LCD_CS_2 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_2 );							

		lcd_command(WRITE_MODE, 3); 					// command 101
		lcd_command(lcd2_3_position[i],6); 				// address
		lcd_command(data, 4); 							// number

		sbi( LATD, LCD_CS_2 ); 
	}

	for(i=0; i<6; i++)
	{
		if(i==0)		data=0xE0;
		else if(i==6)	data=0x00;
		else			data=0x10;
		
		sbi( LATD, LCD_CS_1 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_1 );

		lcd_command(WRITE_MODE, 3); 					// command 101
		lcd_command(lcd1_position[i],6); 				// address
		lcd_command(data,  4); 							// number

		sbi( LATD, LCD_CS_1 ); 
	}
}

void test_lcd_keytest_off(void)
{
	unsigned char 	i=0;
	unsigned char	lcd2_3_position[10]={0x38, 0x3C, 0x40, 0x44, 0x48, 0x4C, 0x70, 0x74, 0x78, 0x7C};
	unsigned char	lcd1_position[6]={0x30, 0x34, 0x38, 0x3C, 0x40, 0x70};
	unsigned char	data=0;

	
	for(i=0; i<10; i++)
	{
		if(i==0)		data=0x00;
		else if(i==1)	data=0x50;
		else if(i==2)	data=0xE0;
		
		sbi( LATD, LCD_CS_2 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_2 );							

		lcd_command(WRITE_MODE, 3); 					// command 101
		lcd_command(lcd2_3_position[i],6); 				// address
		lcd_command(0x00, 4); 							// number

		sbi( LATD, LCD_CS_2 ); 
	}

	for(i=0; i<6; i++)
	{
		if(i==0)		data=0xE0;
		else if(i==6)	data=0x00;
		else			data=0x10;
		
		sbi( LATD, LCD_CS_1 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_1 );

		lcd_command(WRITE_MODE, 3); 					// command 101
		lcd_command(lcd1_position[i],6); 				// address
		lcd_command(0x00,  4); 							// number

		sbi( LATD, LCD_CS_1 ); 
	}
}




void weight_display(unsigned long weight_data)	// 150729 - ing
{
	unsigned char	i=0;
	unsigned char	command=0xA0;
	unsigned char	number=0;
	unsigned char	position=32;
	unsigned char	no_weight_position=0x48;
	unsigned char	Point=0;
	unsigned char	lamp_position=0x38;
	unsigned char	lamp_23_position[4]={0x38, 0x3C, 0x40, 0x48};
	
	unsigned long 	weight_backup=0;


	weight_backup=weight_data;


	

	if(none_point_flag==0)
	{
		Point = ep_minimum_division;  
		Point = Point >> 5;
		Point = Point & 0x07;

		if(ep_unit)
		{
			switch(Point)
			{
				case 0:
					weight_backup *= 1000;
					Point=0;
					break;
				case 1:
					weight_backup *= 100;
					Point=0;
					break;
				case 2:
					weight_backup *= 10;
					Point=0;
					break;
				case 3:
					weight_backup *= 1;
					Point=0;
					break;
			}
		}
	}


	if( ((ex_no_weight_flag==0)&&(ex_zero_tick==0)&&(ex_weight_over_tick==0)&&(ex_key_tare_tick==0))||(test_lcd_mode_f==1) )
	{
		for(i=0; i<5; i++)
		{										
			sbi( LATD, LCD_CS_2 ); 
			sbi( LATD, LCD_WR ); 
			sbi( LATD, LCD_DATA ); 
			cbi( LATD, LCD_CS_2 );							

			number = WEIGHT_NUMBER[weight_backup%10];
			
			if( (Point != 0) && (i== Point) )  number = number | 0x10;

			if((i>Point) && (weight_backup==0)) number = 0x00;

			if( (i==0)&&(ex_weight_sum_blink_flag==1) )
			{
				if(ex_weight_sum_blink_cnt%2==1)	number |= (1<<4); 
				else								number &= ~(1<<4);
			}

			if( (i==0)&&(ex_weight_sum_lamp_flag==1) ) 
			{	
				number |= (1<<4); 	// to do dell
			}
			
			if( i==4 )
			{
				if(ex_sign_flag==1)
				{
					number=0x04;
				}
				else
				{

				}
			}


			lcd_command(WRITE_MODE, 3); // command 101
			lcd_command(position,6); // address
			lcd_command(number,  8); // number

			weight_backup /= 10;
			position -= 8;

			sbi( LATD, LCD_CS_2 ); 
		}
	}
	else
	{
		for(i=0; i<5; i++)
		{										
			sbi( LATD, LCD_CS_2 ); 
			sbi( LATD, LCD_WR ); 
			sbi( LATD, LCD_DATA ); 
			cbi( LATD, LCD_CS_2 );							

			lcd_command(WRITE_MODE, 3); // command 101
			lcd_command(no_weight_position,6); // address
			lcd_command(0x00, 8); // number

			position -= 8;

			sbi( LATD, LCD_CS_2 ); 
		}
	}

	for(i=0; i<4; i++)
	{
		sbi( LATD, LCD_CS_2 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_2 );							

		lcd_command(WRITE_MODE, 3); 	// command 101
		lcd_command(lamp_23_position[i],6);	// address
		lcd_command(lamp_lcd3[i], 4);	// number

		sbi( LATD, LCD_CS_2 ); 

		lamp_position += 4;
	}
}


void pay_display(unsigned long pay_data, unsigned char mode)	// 150729 - ing
{
	unsigned char			i=0; 
	unsigned char			command=0xA0;
	unsigned char			number=0;
	unsigned char			position=40;
	unsigned char			pay_comma_flag=0;
	unsigned char			Point=0;
	
	static unsigned long 	pay_backup=0;


	Point = ep_minimum_division;  
	Point = Point >> 5;
	Point = Point & 0x07;

    if(!mode)
    {
        if(!ep_plu_unit)  //100g
        {
            switch(Point)
            {
                case 0:
                    pay_backup=(pay_data*10);
                    break;
                case 1:

                    pay_backup=(pay_data*1);
                    break;
                case 2:
                    pay_backup=(pay_data/10);
                    break;
                case 3: 
                    pay_backup = (pay_data/100);
                    break;
                default: break;
            }
        }
        else   //1kg
        {
            switch(Point)
            {
                case 0:
                    pay_backup=(pay_data*100);
                    break;
                case 1:
                    pay_backup=(pay_data*10);
                    break;
                case 2:
                    pay_backup=(pay_data/100);
                    break;
                case 3:
                    pay_backup=(pay_data/1000);	
                    break;
                default:								break;
            }
        }
    
    
        if(cut_cipher == 1) 
        {
            if(((pay_backup%10)>=5) && round_off) i = 1;  //반올림 체크
            else i = 0;
            if(pay_backup > 10) pay_backup = (pay_backup/10)*10; 
            else pay_backup = 0;
            if(i) pay_backup = pay_backup + 10;  //반올림
        }
        else if(cut_cipher == 2)
        {
            if(((pay_backup%100)>=50) && round_off) i = 1;  //반올림 체크
            else i = 0;
            if(pay_backup > 100) pay_backup = (pay_backup/100)*100;
            else pay_backup = 0;
            if(i) pay_backup = pay_backup + 100;  //반올림
        }
        else if(cut_cipher == 3)
        {
            if(((pay_backup%1000)>=500) && round_off) i = 1;  //반올림 체크
            else i = 0;
            if(pay_backup > 1000) pay_backup = (pay_backup/1000)*1000;
            else pay_backup = 0;
            if(i) pay_backup = pay_backup + 1000;  //반올림
        }
        else if(cut_cipher == 4)
        {
            if(((pay_backup%10000)>=5000) && round_off) i = 1;  //반올림 체크
            else i = 0;
            if(pay_backup > 10000) pay_backup = (pay_backup/10000)*10000;
            else pay_backup = 0;
            if(i) pay_backup = pay_backup + 10000;  //반올림
        }
    }
    else 
    {
        pay_backup=pay_data;
    }
    cut_pay = pay_backup;

    if(pay_backup>999)
	{
		pay_comma_flag=1;
	}
	else
	{
		pay_comma_flag=0;
	}

	if(ex_weight_over_tick)	pay_comma_flag=0;	
	
	for(i=0; i<6; i++)
	{
		sbi( LATD, LCD_CS_1 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_1 );

		number = WEIGHT_NUMBER[pay_backup%10] ;

		if( (i>0)&&(pay_backup==0) )
		{
			number = 0x00;
		}
		else
		{

		}

		if( (i==3)&&(pay_comma_flag==1) ) 		
		{
			number |=0x10;
		}
		else
		{

		}


		lcd_command(WRITE_MODE, 3); // command 101
		lcd_command(position,6); // address
		lcd_command(number,  8); // number

		pay_backup /= 10;
		position -= 8;

		sbi( LATD, LCD_CS_1 ); 
	}

#if 1
	if( pay_comma_flag==1 ) 		
	{
		sbi( LATD, LCD_CS_1 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_1 );

		lcd_command(WRITE_MODE, 3); // command 101
		lcd_command(0x38, 6); // address
		lcd_command(0x10, 8); // number

		sbi( LATD, LCD_CS_1 ); 
	}
	else
	{
		sbi( LATD, LCD_CS_1 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_1 );

		lcd_command(WRITE_MODE, 3); // command 101
		lcd_command(0x38, 6); // address
		lcd_command(0x00, 4); // number

		sbi( LATD, LCD_CS_1 ); 
	}
#endif 

}

void pay_count(unsigned long pay_data)	// 150729 - ing
{
	unsigned char			i=0; 
	unsigned char			command=0xA0;
	unsigned char			number=0;
	unsigned char			position=40;
	unsigned char			pay_comma_flag=0;
	unsigned char			Point=0;
	
	static unsigned long 	pay_backup=0;


	pay_backup = pay_data;
	for(i=0; i<6; i++)
	{
		sbi( LATD, LCD_CS_1 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_1 );

		number = WEIGHT_NUMBER[pay_backup%10] ;

        if( (i>0)&&(pay_backup==0) )
		{
			number = 0x00;
		}
		
		lcd_command(WRITE_MODE, 3); // command 101
		lcd_command(position,6); // address
		lcd_command(number,  8); // number

		pay_backup /= 10;
		position -= 8;

		sbi( LATD, LCD_CS_1 ); 
	}
    sbi( LATD, LCD_CS_1 ); 
    sbi( LATD, LCD_WR ); 
    sbi( LATD, LCD_DATA ); 
    cbi( LATD, LCD_CS_1 );

    lcd_command(WRITE_MODE, 3); // command 101
    lcd_command(0x38, 6); // address
    lcd_command(0x00, 4); // number

    sbi( LATD, LCD_CS_1 ); 
}

void unit_price_display(unsigned long u_p_data)	// 150729 - ing
{
	unsigned char 			i=0;
	unsigned char 			command=0xA0;
	unsigned char 			number=0;
	unsigned char 			position=112;
	unsigned char 			Point_flag=0;
	unsigned char 			lamp_position[4]={0x44, 0x4C, 0x78, 0x7C};
	static unsigned long 	u_p_bakcup=0;


	if( (key_task_key_f==1)&&(key_task_key_tim>800) )
	{
		String_Display(UNIT_PRICE, "     ");
	}
	else
	{
		u_p_bakcup=u_p_data;
		

		if(u_p_bakcup>999)	
		{
			Point_flag=1;
		}
		else
		{
			Point_flag=0;
		}

		for(i=0; i<5; i++)
		{
			sbi( LATD, LCD_CS_2 ); 
			sbi( LATD, LCD_WR ); 
			sbi( LATD, LCD_DATA ); 
			cbi( LATD, LCD_CS_2 );

			number = WEIGHT_NUMBER[u_p_bakcup%10] ;

			if( (i==3)&&(Point_flag==1) )
			{
				number = number | 0x10;
			}
			else
			{

			}

			if((i>0) && (u_p_bakcup==0))
			{
				number = 0x00;
			}
			else
			{

			}

			lcd_command(WRITE_MODE, 3); // command 101
			lcd_command(position,6); // address
			lcd_command(number,  8); // number

			u_p_bakcup /= 10;
			position -= 8;

			sbi( LATD, LCD_CS_2 ); 
		}

        
		if(ep_plu_unit==0)
		{
			LAMP_LCD2_1RT_ON;
			LAMP_LCD2_3RT_OFF;
		}
		else if(ep_plu_unit==1)
		{
			LAMP_LCD2_1RT_OFF;
			LAMP_LCD2_3RT_ON;
		}
         
		
		for(i=0; i<4; i++)
		{
			sbi( LATD, LCD_CS_2 ); 
			sbi( LATD, LCD_WR ); 
			sbi( LATD, LCD_DATA ); 
			cbi( LATD, LCD_CS_2 );							

			lcd_command(WRITE_MODE, 3); 	// command 101
			lcd_command(lamp_position[i],6);		// address
			lcd_command(lamp_lcd2[i], 4);	// number

			sbi( LATD, LCD_CS_2 ); 
		}


		if(Point_flag==1)
		{
			sbi( LATD, LCD_CS_2 ); 
			sbi( LATD, LCD_WR ); 
			sbi( LATD, LCD_DATA ); 
			cbi( LATD, LCD_CS_2 );
		
			lcd_command(WRITE_MODE, 3); // command 101
			lcd_command(120,6); 		// address
			lcd_command(0x10, 4);		// number
		
			sbi( LATD, LCD_CS_2 ); 
		}
		else
		{
			sbi( LATD, LCD_CS_2 ); 
			sbi( LATD, LCD_WR ); 
			sbi( LATD, LCD_DATA ); 
			cbi( LATD, LCD_CS_2 );
		
			lcd_command(WRITE_MODE, 3); // command 101
			lcd_command(120,6); 		// address
			lcd_command(0x00, 4);		// number
		
			sbi( LATD, LCD_CS_2 ); 
		}
	}
}

void plu_display(unsigned char plu_data)	// 150729 - ing
{
	unsigned char i=0,command=0xA0, number=0, position=48, Point=0;

	for(i=0; i<2; i++)
	{
		sbi( LATD, LCD_CS_2 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_2 );

		number = WEIGHT_NUMBER[plu_data%10] ;
		if((i>Point) && (plu_data==0)) number = 0x00;

		if( (i==0)&&(ex_weight_batt_flag==1) )		number |= 0x10;
		if( (i==1)&&(ex_weight_wireless_flag==1) )	number |= 0x10;

			


		lcd_command(WRITE_MODE, 3); // command 101
		lcd_command(position,6); // address
		lcd_command(number,  8); // number

		plu_data /= 10;
		position -= 8;

		sbi( LATD, LCD_CS_2 ); 
	}
}

unsigned long input_number(unsigned long input_data)
{
	
}


void String_Display(unsigned char mode ,char *font_data)
{
	unsigned char i, command=0xA0, position=0, font=0, loop=0, LCD_CS=0;

	switch(mode)
	{
		case	PAY   		:	position= 0;	loop=6;		LCD_CS=LCD_CS_1;	break;	
		case 	UNIT_PRICE	: 	position=80;	loop=5;		LCD_CS=LCD_CS_2;	break;
		case	WEIGHT		:	position=0;		loop=5;		LCD_CS=LCD_CS_2;	break;
		case	PLU			:	position=40;	loop=2;		LCD_CS=LCD_CS_2;	break;
		default:														break;
	}

	sbi( LATD, LCD_CS );
	sbi( LATD, LCD_WR ); 
	sbi( LATD, LCD_DATA ); 
	cbi( LATD, LCD_CS );

	lcd_command(WRITE_MODE,3); // command 101
	lcd_command(position,6);    // address

	for(i=0; i<loop; i++)
	{
		font = ASCII_FONT[font_data[i]-0x20];
		lcd_command(font,8);
	} 

	position += 8;
	sbi( LATD, LCD_CS );

	if(ex_weight_over_tick)
	{
		sbi( LATD, LCD_CS_1 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_1 );

		lcd_command(WRITE_MODE, 3); // command 101
		lcd_command(0x38, 6); // address
		lcd_command(0x00, 4); // number

		sbi( LATD, LCD_CS_1 ); 
	}
}

void Weight_Lamp_Data(void)
{
	
}


void Lamp_On(unsigned char lamp_value)
{
	unsigned char i=0,command=0xA0, number=0, position=0x7C;
	

	

	for(i=0; i<5; i++)
	{
		sbi( LATD, LCD_CS_1 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_1 );

		lcd_command(WRITE_MODE, 3); // command 101
		lcd_command(position,6); // address
		lcd_command(number,  4); // number

		position += 4;

		sbi( LATD, LCD_CS_1 ); 
	}
}



void Lamp_All_Off_weight(void)
{
	LAMP_LCD3_ZERO_OFF;
	LAMP_LCD3_1RT_OFF;
	LAMP_LCD3_1COMMA_OFF;
	LAMP_LCD3_TARE_OFF;
	LAMP_LCD3_2RT_OFF;
	LAMP_LCD3_2COMMA_OFF;
	LAMP_LCD3_HOLD_OFF;
	LAMP_LCD3_3RT_OFF;
	LAMP_LCD3_3COMMA_OFF;
	LAMP_LCD3_STABLE_OFF;
	LAMP_LCD3_LB_OFF;
	LAMP_LCD3_K_OFF;
	LAMP_LCD3_G_OFF;
}

void Lamp_All_On_Weight(void)
{
	LAMP_LCD3_ZERO_ON;
	LAMP_LCD3_1RT_ON;
	LAMP_LCD3_1COMMA_ON;
	LAMP_LCD3_TARE_ON;
	LAMP_LCD3_2RT_ON;
	LAMP_LCD3_2COMMA_ON;
	LAMP_LCD3_HOLD_ON;
	LAMP_LCD3_3RT_ON;
	LAMP_LCD3_3COMMA_ON;
	LAMP_LCD3_STABLE_ON;
	LAMP_LCD3_LB_ON;
	LAMP_LCD3_K_ON;
	LAMP_LCD3_G_ON;
}

void Lamp_All_Off_u_p(void)
{
	LAMP_LCD2_ZERO_OFF;
	LAMP_LCD2_1RT_OFF;
	LAMP_LCD2_1COMMA_OFF;
	LAMP_LCD2_TARE_OFF;
	LAMP_LCD2_2RT_OFF;
	LAMP_LCD2_2COMMA_OFF;
	LAMP_LCD2_HOLD_OFF;
	LAMP_LCD2_3RT_OFF;
	LAMP_LCD2_3COMMA_OFF;
	LAMP_LCD2_STABLE_OFF;
	LAMP_LCD2_LB_OFF;
	LAMP_LCD2_K_OFF;
	LAMP_LCD2_G_OFF;
}

void Lamp_All_On_u_p(void)
{
	LAMP_LCD2_ZERO_ON;
	LAMP_LCD2_1RT_ON;
	LAMP_LCD2_1COMMA_ON;
	LAMP_LCD2_TARE_ON;
	LAMP_LCD2_2RT_ON;
	LAMP_LCD2_2COMMA_ON;
	LAMP_LCD2_HOLD_ON;
	LAMP_LCD2_3RT_ON;
	LAMP_LCD2_3COMMA_ON;
	LAMP_LCD2_STABLE_ON;
	LAMP_LCD2_LB_ON;
	LAMP_LCD2_K_ON;
	LAMP_LCD2_G_ON;
}



void Lamp_Off(unsigned char select)
{
	unsigned char i=0,command=0xA0, number=0;
	unsigned char position_weight=0x38, position_plu=0x38, position_pay=0x30, position_up=0x78;
	

	if(select==WEIGHT)
	{
		for(i=0; i<5; i++)
		{
			sbi( LATD, LCD_CS_2 ); 
			sbi( LATD, LCD_WR ); 
			sbi( LATD, LCD_DATA ); 
			cbi( LATD, LCD_CS_2 );

			lcd_command(WRITE_MODE, 3); // command 101
			lcd_command(position_weight,6); // address

			if(i==4)
			{
				lcd_command(0x00, 2); // number
			}
			else
			{
				lcd_command(0x00, 4); // number
			}

			position_weight += 4;

			sbi( LATD, LCD_CS_1 ); 
		}

		sbi( LATD, LCD_CS_2 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_2 );
	}
	else if(select==PLU)
	{
		for(i=0; i<2; i++)
		{
			sbi( LATD, LCD_CS_2 ); 
			sbi( LATD, LCD_WR ); 
			sbi( LATD, LCD_DATA ); 
			cbi( LATD, LCD_CS_2 );

			lcd_command(WRITE_MODE, 3); // command 101
			lcd_command(position_plu,6); // address
			lcd_command(0x00,  4); // number

			position_plu += 8;

			sbi( LATD, LCD_CS_2 ); 
		}
	}
	else if(select==UNIT_PRICE)
	{
		for(i=0; i<1; i++)
		{
			sbi( LATD, LCD_CS_2 ); 
			sbi( LATD, LCD_WR ); 
			sbi( LATD, LCD_DATA ); 
			cbi( LATD, LCD_CS_2 );

			lcd_command(WRITE_MODE, 3); // command 101
			lcd_command(position_up,6); // address
			lcd_command(0x00, 3); // number

			sbi( LATD, LCD_CS_2 ); 
		}
	}

	else if(select==PAY)
	{
		for(i=0; i<17; i++)
		{
			sbi( LATD, LCD_CS_1 ); 
			sbi( LATD, LCD_WR ); 
			sbi( LATD, LCD_DATA ); 
			cbi( LATD, LCD_CS_1 );

			lcd_command(WRITE_MODE, 3); // command 101
			lcd_command(position_pay, 6); // address

			if(i==2)
			{
				lcd_command(0x00, 3); // number
			}
			else
			{
				lcd_command(0x00, 4); // number
			}

			position_pay += 4;

			sbi( LATD, LCD_CS_1 ); 
		}

	}
}



void Display_Task(void)
{
	if(poff_sig==1)
	{
		#if 0
		Lamp_All_Off_u_p();
		Lamp_All_Off_weight();
  		LAMP_LCD3_K_OFF;
		LAMP_LCD3_G_OFF;
		LAMP_LCD3_STABLE_OFF;
		LAMP_LCD3_LB_OFF;
		LAMP_LCD3_ZERO_OFF;
		LAMP_LCD2_STABLE_OFF;
		LAMP_LCD2_K_OFF;
		LAMP_LCD2_G_OFF;
		LAMP_LCD2_1RT_OFF;
		Lamp_Off(WEIGHT);
		Lamp_Off(UNIT_PRICE);
		#endif 
		lcd_initial();
		BackLight_Off;
		DelayMs(3000);
	}
	else
	{
		if(ex_zero_tick)
		{
			LAMP_LCD3_K_OFF;
			LAMP_LCD3_G_OFF;
			LAMP_LCD3_LB_OFF;
			LAMP_LCD3_STABLE_OFF;
			LAMP_LCD3_ZERO_OFF;

			String_Display(WEIGHT, " ZERO"); 
		}
		else if(ex_weight_over_tick)
		{
			LAMP_LCD3_K_OFF;
			LAMP_LCD3_G_OFF;
			LAMP_LCD3_LB_OFF;
			LAMP_LCD3_STABLE_OFF;
			LAMP_LCD3_ZERO_OFF;

			String_Display(WEIGHT, " OVER"); 
		}
        else if(ex_weight_minus_over_tick)
		{
			LAMP_LCD3_K_OFF;
			LAMP_LCD3_G_OFF;
			LAMP_LCD3_LB_OFF;
			LAMP_LCD3_STABLE_OFF;
			LAMP_LCD3_ZERO_OFF;

			String_Display(WEIGHT, " ---- ");
		}
		else if(ex_key_tare_tick)
		{
			LAMP_LCD3_K_OFF;
			LAMP_LCD3_G_OFF;
			LAMP_LCD3_LB_OFF;
			LAMP_LCD3_STABLE_OFF;
			LAMP_LCD3_ZERO_OFF;

			String_Display(WEIGHT, " TARE"); 
		}
		else if(ex_auto_hold_on_tick)
		{
			LAMP_LCD3_K_OFF;
			LAMP_LCD3_G_OFF;
			LAMP_LCD3_LB_OFF;
			LAMP_LCD3_STABLE_OFF;
			LAMP_LCD3_ZERO_OFF;

			String_Display(WEIGHT, "AH ON"); 
		}
		else if(ex_auto_hold_off_tick)
		{
			LAMP_LCD3_K_OFF;
			LAMP_LCD3_G_OFF;
			LAMP_LCD3_LB_OFF;
			LAMP_LCD3_STABLE_OFF;
			LAMP_LCD3_ZERO_OFF;

			String_Display(WEIGHT, "AHOFF"); 
		}
		else if( (ex_weight_add_tick==1)&&(ex_weight_sum_lamp_flag==0) ) 
		{
			String_Display(UNIT_PRICE, "AD	 ");

			#if 0
			if( (ex_weight_add_count>=0)&&(ex_weight_add_count<=9) )	
			{
				lcd_want_display(UNIT_PRICE, 5, ex_weight_add_count%10);
			}
			else if( (ex_weight_add_count>=10)&&(ex_weight_add_count<=99) )
			{
				lcd_want_display(UNIT_PRICE, 4, ex_weight_add_count/10);
				lcd_want_display(UNIT_PRICE, 5, ex_weight_add_count%10);
			}
			else if( (ex_weight_add_count>=100)&&(ex_weight_add_count<=254) )
			{
				lcd_want_display(UNIT_PRICE, 3, ex_weight_add_count/100);
				lcd_want_display(UNIT_PRICE, 4, (ex_weight_add_count%100)/10);
				lcd_want_display(UNIT_PRICE, 5, (ex_weight_add_count%100)%10);
			}
			#endif 

			#if 1
			lcd_want_display(UNIT_PRICE, 3, ex_weight_add_count/100);
			lcd_want_display(UNIT_PRICE, 4, (ex_weight_add_count%100)/10);
			lcd_want_display(UNIT_PRICE, 5, (ex_weight_add_count%100)%10);
			#endif 

			
			#if 0
			if( (ex_weight_add_count>=100)&&(ex_weight_add_count<=254) )
			{
				lcd_want_display(UNIT_PRICE, 3, ex_weight_add_count/100);
				lcd_want_display(UNIT_PRICE, 4, (ex_weight_add_count%100)/10);
				lcd_want_display(UNIT_PRICE, 5, (ex_weight_add_count%100)%10);
			}
			else if( (ex_weight_add_count>=10)&&(ex_weight_add_count<=99) )
			{
				lcd_want_display(UNIT_PRICE, 4, ex_weight_add_count/10);
				lcd_want_display(UNIT_PRICE, 5, ex_weight_add_count%10);
			}
			if( (ex_weight_add_count>=0)&&(ex_weight_add_count<=9) )	
			{
				lcd_want_display(UNIT_PRICE, 5, ex_weight_add_count%10);
			}
			#endif 
			
			if(ex_hold_flag==1)	
			{
				if(ex_weight_hold_value==0)	weight_display(ex_e_value);
				else						weight_display(ex_weight_hold_value);
			}
			else			
			{
				weight_display(ex_e_value);
			}
		}
		else if( ex_weight_sum_lamp_flag==1 ) 
		{
			
		}
		else if(ex_hold_bar)
		{
			switch(ex_hold_bar)
			{
				case 1:	String_Display(WEIGHT, "  --  ");	break;
				case 2:	String_Display(WEIGHT, " --   ");	break;
				case 3:	String_Display(WEIGHT, " ---  ");	break;
				case 4:	String_Display(WEIGHT, " ---- ");	break;
				case 5:	String_Display(WEIGHT, " -----");	break;
				default:									break;
			}
		}
		else 
		{
			if(ex_zero_flag==1)	LAMP_LCD3_ZERO_ON;
			else				LAMP_LCD3_ZERO_OFF;

			if(ex_stab_flag==1)	LAMP_LCD3_STABLE_ON;	
			else				LAMP_LCD3_STABLE_OFF;

			if(ep_tare_flag==1)	LAMP_LCD3_TARE_ON;
			else				LAMP_LCD3_TARE_OFF;

			if(ex_hold_flag==1)	LAMP_LCD3_HOLD_ON;
			else				LAMP_LCD3_HOLD_OFF;


			

			
			switch(ep_unit)
			{
				case UNIT_KG:	LAMP_LCD3_K_ON; 	LAMP_LCD3_G_ON; 						break;
				case UNIT_G:	LAMP_LCD3_K_OFF;	LAMP_LCD3_G_ON; 						break;
				case UNIT_LB:	LAMP_LCD3_K_OFF;	LAMP_LCD3_G_OFF;	LAMP_LCD3_LB_ON;	break;
				default:																	break;
			}
			

			if(ex_hold_flag==1)	
			{
				if(ex_weight_hold_value==0)	weight_display(ex_e_value);
				else						weight_display(ex_weight_hold_value);
			}
			else			
			{
				weight_display(ex_e_value);
			}

			if(key_task_key_f==1)	String_Display(PLU, "  ");//plu_display(0);
			else					plu_display(ex_atflash_PluNum);

			if(key_task_key_f==1)	unit_price_display(key_task_number);	
			else					unit_price_display(ex_atflash_PluWriteValue);
		}



		if(ex_weight_over_tick)
		{
			String_Display(PAY, " - - -");
		}
		else if(ex_sign_flag==1)
		{
			String_Display(PAY, " - - -");
		}
		else
		{
			if( ex_weight_sum_lamp_flag==1 )
			{
				pay_display(ex_weight_sum_value, 1); 
				String_Display(UNIT_PRICE,	"A-SUM");
				String_Display(PLU, 		"  ");
				String_Display(WEIGHT, 		"     ");

				#if 1
				lcd_want_display(WEIGHT, 3, ex_weight_add_count/100);
				lcd_want_display(WEIGHT, 4, (ex_weight_add_count%100)/10);
				lcd_want_display(WEIGHT, 5, (ex_weight_add_count%100)%10);
				#endif 
			}
			else
			{
                unsigned long i;
                char p;
                p = ep_minimum_division;  
                p = p >> 5;
                p = p & 0x07;
                
                if(!ep_plu_unit)
                {
                    if(!p) i = 1;
                    else if(p == 2) i = 10;
                    else if(p == 3) i = 100;
                }
                else
                {
                    if(!p) i = 10;
                    else if(p == 2) i = 100;
                    else if(p == 3) i = 1000;
                }
                if(ex_hold_flag==1)	
				{
                    
					//if(ex_weight_hold_value)	
                    if(ex_hold_finish)
                    {
                        i = (ex_weight_hold_value*ex_atflash_PluWriteValue)/i;
                        if(i < 1000000) pay_display( (ex_weight_hold_value*ex_atflash_PluWriteValue), 0);
                        else String_Display(PAY, "   E  ");
                        //pay_price = (ex_weight_hold_value*ex_atflash_PluWriteValue);
                    }
                  
					else						
                    {       
                        i = (ex_e_value*ex_atflash_PluWriteValue)/i;
                        if(i < 1000000) pay_display( (ex_e_value*ex_atflash_PluWriteValue), 0);
                        else String_Display(PAY, "   E  ");
                        //pay_price = (ex_e_value*ex_atflash_PluWriteValue);
                    }
					
					if(ex_hold_bar)
					{
						String_Display(PAY, " HOLD ");
					}
				}
				else				
				{
                    i = (ex_e_value*ex_atflash_PluWriteValue)/i;
                    if(i < 1000000)	pay_display( (ex_e_value*ex_atflash_PluWriteValue), 0);
                    else String_Display(PAY, "   E  ");
                    //pay_price = (ex_e_value*ex_atflash_PluWriteValue);
				}
			}
		}
	}
}




//******************************************************************************
void lcd_initial(void)
{
	unsigned char i, command;
	unsigned char position=0x78;

	sbi( LATD, LCD_CS_1 );
	sbi( LATD, LCD_WR );
	sbi( LATD, LCD_DATA );
	cbi( LATD, LCD_CS_1 );
	lcd_command(0x80,3);

	for(i=0; i<5; i++)
	{
		command = LCD_COMMAND[i];
		lcd_command(command,8);
		lcd_command(0x00,1);
	}
	
	sbi( LATD, LCD_CS_1 );
	sbi( LATD, LCD_WR );
	sbi( LATD, LCD_DATA );


#if 1
	for(i=0; i<16; i++)
	{
		sbi( LATD, LCD_CS_1 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_1 );

		lcd_command(WRITE_MODE, 3); // command 101
		lcd_command(position,6); // address
		lcd_command(0x00,  8); // number

		position -= 8;

		sbi( LATD, LCD_CS_1 ); 
	}
#endif


	

	//Lamp_Off(PAY);
	//String_Display(PAY, "      ");




	///////////////////////////////////////
	position=0x78;

	sbi( LATD, LCD_CS_2 );
	sbi( LATD, LCD_WR );
	sbi( LATD, LCD_DATA );
	cbi( LATD, LCD_CS_2 );
	lcd_command(0x80,3);

	for(i=0; i<5; i++)
	{
		command = LCD_COMMAND[i];
		lcd_command(command,8);
		lcd_command(0x00,1);
	}
	
	sbi( LATD, LCD_CS_2 );
	sbi( LATD, LCD_WR );
	sbi( LATD, LCD_DATA );

#if 1
	for(i=0; i<16; i++)
	{
		sbi( LATD, LCD_CS_2 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_2 );

		lcd_command(WRITE_MODE, 3); // command 101
		lcd_command(position,6); // address
		lcd_command(0x00,  8); // number

		position -= 8;

		sbi(LATD, LCD_CS_2); 
	}
#endif




}
//******************************************************************************


//******************************************************************************
void lcd_command(unsigned char lcd_data, unsigned char k)
{
	unsigned char i;


	for(i=0; i<k; i++)
	{
		cbi( LATD, LCD_WR );

		if(lcd_data & 0x80) sbi( LATD, LCD_DATA );
		else                cbi( LATD, LCD_DATA );

		sbi( LATD, LCD_WR ); 
		lcd_data = lcd_data << 1;
	}  
}
//******************************************************************************





//******************************************************************************


//******************************************************************************
void lcd_want_display(unsigned char lcd, unsigned char digit, unsigned char digit_data)
{
	unsigned char address;


	sbi( LATD, LCD_CS_2 ); 
	sbi( LATD, LCD_WR ); 
	sbi( LATD, LCD_DATA ); 
	cbi( LATD, LCD_CS_2 );							

	if(lcd==UNIT_PRICE)
	{
		switch(digit)
		{
			case 1: address=80; 	break;
			case 2: address=88; 	break;
			case 3: address=96;		break;
			case 4: address=104;	break;
			case 5: address=112;	break;
			default:				break;
		}
	}
	else if(lcd==WEIGHT) 
	{
		switch(digit)
		{
			case 1: address= 0; break;
			case 2: address= 8; break;
			case 3: address=16; break;
			case 4: address=24; break;
			case 5: address=32; break;
			default:			break;
		}
	}
	else
	{
		String_Display(PAY, " ERROR");
		DelayMs(700);
	}


	lcd_command(0xA0,3); // command 101
	lcd_command(address,6); // address

	if(digit_data<=10) address = (digit_data%10) +'0';
	else               address = digit_data + '0';

	address = ASCII_FONT[address-' '];

	if( (digit==5)&&(ex_weight_sum_lamp_flag==1) )	address |= (1<<4);
		
	lcd_command(address,8); 
	sbi( LATD, LCD_CS_2 ); 

	////////////////////////////////////////////////////////////////////////////
	if(ex_weight_sum_lamp_flag==1)
	{
		sbi( LATD, LCD_CS_2 ); 
		sbi( LATD, LCD_WR ); 
		sbi( LATD, LCD_DATA ); 
		cbi( LATD, LCD_CS_2 );
		
		lcd_command(WRITE_MODE, 3); // command 101
		lcd_command(0x38,6); // address
		lcd_command(0x00,  8); // number
		//lcd_command(0x40,6); // address
		lcd_command(0x00,  8); // number
		//lcd_command(0x48,6); // address
		lcd_command(0x00,  4); // number

		sbi( LATD, LCD_CS_2 ); 
	}
}
//******************************************************************************


//******************************************************************************
//******************************************************************************

