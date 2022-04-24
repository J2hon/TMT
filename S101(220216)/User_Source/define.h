#ifndef _DEFINE_H_
#define _DEFINE_H_

#define YES                     1
#define NO                      0
//#define ModelRecognition         YES  //형싱승인용
#define ModelRecognition        NO  //일반용

// EEPROM
#define EP_RES_FACTOR			10	// 16 bit
#define EP_ZERO					2	// 16 bit
#define EP_MAXIMUM_CAPACITY		4	// 16 bit
#define EP_E_RESOLUTION			6	// 16 bit
#define EP_TARE_VALUE		    8	// 16 bit
#define EP_SHORT_PLU			12	// 16 bit

#define EP_RES_FACTOR_DUAL		14
#define EP_E_RESOLUTION_DUAL    16
#define EP_MINIMUM_DIVISION_DUAL    18
#define EP_DUAL_RANGE_BOUNDARY      20

#define EP_GRAVITY_FACTOR1  	30
#define EP_GRAVITY_FACTOR2		31
#define EP_MINIMUM_DIVISION		32
#define EP_CAL_NO				33

#define EP_FILTER_DEGREE		34
#define EP_STABLE				35
#define EP_AZ_DIGIT				36
#define EP_AZ_SECOND			37
#define EP_HOLDSPEED			38
#define EP_HOLD_START			39
#define EP_HOLD_ZERO			40
#define EP_STAR					41
#define EP_ID					42
#define EP_RS_FORM				43
#define EP_PRT_FORM				44
#define EP_FEED					45
#define EP_WEI_NO				46
#define EP_WIRELESS				47
#define EP_BACKLIGHT			48
#define EP_BRIGHT				49
#define EP_TARE_FLAG			50
#define EP_WEI_BACKUP			51

#define EP_UNIT					52
#define EP_HOLD_RANGE		    53

#define EP_ZERO_RANGE_VALUE		54 

#define EP_PLU_UNIT				57
#define	EP_RTC_HOUR				58
#define	EP_RTC_MINUTE			59

#define EP_LONG_KEY_TIME		61
#define EP_KEY_TARE_ENABLE      62
#define EP_MINUS_DISPLAY        63
#define EP_CIPHER               64
#define EP_ROUND_OFF            65
#define EP_ENTER_KEY_SET        66

#define CAPA_INPUT              1
#define LOAD_INPUT              2


#define	PowerOn					sbi(LATB,POWER_ON)	// Power On
#define	PowerOff				cbi(LATB,POWER_ON)	// Power Off




// HOLD
#define HOLD_COUNT_DIVIDE		2					// v_hold_speed / 2
#define HOLD_START_DELAY_TIME	1000				// 1000 x 1ms = 1.5sec
#define HOLD_INTERVAL_VALUE		100					// 100 x 1ms = 100 ms










// LCD
#define	PAY			1
#define UNIT_PRICE 	2
#define WEIGHT 	   	3
#define PLU	4

#define WRITE_MODE 0xA0








#define	sbi(d,b)	d|=(0x0001<<b)
#define	cbi(d,b)	d&=~(0x0001<<b)
#define	tgbi(d,b)	d^=(0x0001<<b)
#define	BV(x)    	(1 << (x))
#define  tbi(d,b)	((d)&(1<<(b)))


// Define Key
#define	ZERO_KEY			1	//			1번 키 
#define	HOLD_KEY      		2	//			2번 키 
#define	ADD_KEY      		3	//			3번 키 
#define	SUM_KEY				4	//			4번 키 
#define	SAVE_KEY       		5	//			5번 키 
#define	MULTI_KEY			6	//		//	6번 키 
#define	F1_KEY				7	//			7번 키 
#define	F2_KEY				8
#define	F3_KEY				9
#define	CLR_K				0
#define	ENT_K				10
#define CAL_K				11

//#define	SET_K       		11
//#define	CLR_K       		12
#define	CHECK_K       		13
#define	POWER_K     		99


// LCD Back Light
#define	BackLight_On			OC1CON1bits.OCM=6	// BackLight On
#define	BackLight_Off			OC1CON1bits.OCM=0	// BackLight Off





// Define ETC.
#define  BUZZER      4 // RB4
#define  POWER_ON    3 // RB3
#define  POWER_SW    2 // RB2
#define  BOOT1       0 // RD0
#define  M1_RESET    10 // RD10
#define  IR_SIG      9  // RD9
#define  WAKEUP1     3  // RG3

// io_init
#define OUT 0
#define IN  1
#define SET 1
#define CLR	0
#define DIGITAL 0
#define ANALOG 1


// Key 
#define ir_key_press_f			1					// IR key pressed
#define ir_key_detect_f			0					// IR key detection
#define _ir_key_press_f			0x02
#define _ir_key_detect_f		0x01
#define IR_KEY_RELEASE_TIME		36					// 4ms x 36 = 144ms									



#define key_press_f				1					// key pressed
#define key_detect_f			0					// key detection
#define _key_press_f			0x02
#define _key_detect_f			0x01
#define KEY_DEBOUNCE_TIME		5									
#define KEY_RELEASE_TIME		3									
#define LONG_KEY_2SEC_TIME		20					// 20 x 100ms = 2 sec
#define LONG_KEY_3SEC_TIME		30					// 30 x 100ms = 3 sec


// BUZZER
#define	BuzzerOn			cbi(LATB,BUZZER)//cbi(LATB,BUZZER)		// Buzzer On
#define	BuzzerOff			sbi(LATB,BUZZER)		// Buzzer Off

#define	SHORT_TONE_PLAY		0x01
#define	LONG_TONE_PLAY		0x02
#define	TWO_TONE_PLAY		0x14
#define	THREE_TONE_PLAY		0x12
#define	FOUR_TONE_PLAY		0x10

#define	SHORT_TONE_TIME		20						// 20 x 4ms = 80 ms
#define	LONG_TONE_TIME		60						// 60 x 4ms = 240 ms



// Define ADC
#define  AD_DIN 		   6 // RG6
#define  AD_DOUT	    7 // RG7
#define  AD_SCLK     8 // RG8
#define  AD_CS		     6 // RG9

// Flash
#define  F_CS      1 // RF 1
#define  F_MISO    5 // RF 5
#define  F_SCK     6 // RF 6
#define  F_MOSI    4 // RF 4

#define  F_BUFFER_1_WRITE 0x84
#define  F_BUFFER_2_WRITE 0x87
#define  F_BUFFER_1_TO_PAGE 0x83
#define  F_BUFFER_2_TO_PAGE 0x86

#define ManufacturerandDeviceIDRead 0x9F
#define StatusRegisterRead 0xD7
#define Buffer1Write 0x84
#define Buffer1toMainMemoryPageProgramwithoutErase 0x83
#define MainMemoryPageProgramThroughBuffer2 0x55

#define Buffer2Read 0xD6

#define PageSizeBit (1<<0)
#define Buffer264Byte (1<<0)

#define MainMemoryPageToBuffer1Transfer 0x53



// AT_FLASH_ADDR
#define PLU_CUR			0
#define DTP_SERIAL		0
#define	PLU_DTP_HEAD	1
#define PLU_DTP_TAIL	1000
#define	DAY_DTP_HEAD	1001
#define	DAY_DTP_TAIL	2000
#define PLU_U_P_HEAD	2001
#define PLU_U_P_TAIL	2002
#define PAGE_TEST 		2047

#define	DAY_DTP_HEAD_BYTE	0
#define DAY_DTP_TAIL_BYTE	251-1

#define	PLU_DTP_HEAD_BYTE	0
#define PLU_DTP_TAIL_BYTE	252-2

#define	DAY_DTP_GAP_BYTE	12


#define DTP_SERIAL_BUF	100
#define	PLU_CUR_BUF		200

#define DTP_SERIAL_NO_HEAD	1
#define DTP_SERIAL_NO_TAIL	21000


#define	ATflash_Busy_Flag while((atflash_readstatus() & 0x80) == 0x00)






// LAMP
////////////////////////////////////////////////////////////////////////
#define	LAMP_LCD3_ZERO		0x80
#define LAMP_LCD3_1RT		0x40
#define	LAMP_LCD3_1COMMA	0x10

#define LAMP_LCD3_TARE		0x40
#define LAMP_LCD3_2RT		0x20
#define	LAMP_LCD3_2COMMA	0x10

#define LAMP_LCD3_HOLD		0x80
#define	LAMP_LCD3_3RT		0x40
#define	LAMP_LCD3_3COMMA	0x10

#define LAMP_LCD3_STABLE 	0x80
#define LAMP_LCD3_LB		0x40
#define LAMP_LCD3_K			0x20
#define LAMP_LCD3_G			0x10


#define LAMP_LCD3_ZERO_ON		lamp_lcd3[0] |=  LAMP_LCD3_ZERO
#define LAMP_LCD3_ZERO_OFF		lamp_lcd3[0] &= ~LAMP_LCD3_ZERO
#define LAMP_LCD3_1RT_ON		lamp_lcd3[0] |=  LAMP_LCD3_1RT
#define LAMP_LCD3_1RT_OFF		lamp_lcd3[0] &= ~LAMP_LCD3_1RT
#define LAMP_LCD3_1COMMA_ON		lamp_lcd3[0] |=  LAMP_LCD3_1COMMA
#define LAMP_LCD3_1COMMA_OFF	lamp_lcd3[0] &= ~LAMP_LCD3_1COMMA

#define LAMP_LCD3_TARE_ON		lamp_lcd3[1] |=  LAMP_LCD3_TARE
#define LAMP_LCD3_TARE_OFF		lamp_lcd3[1] &= ~LAMP_LCD3_TARE
#define LAMP_LCD3_2RT_ON		lamp_lcd3[1] |=  LAMP_LCD3_2RT
#define LAMP_LCD3_2RT_OFF		lamp_lcd3[1] &= ~LAMP_LCD3_2RT
#define LAMP_LCD3_2COMMA_ON		lamp_lcd3[1] |=  LAMP_LCD3_2COMMA
#define LAMP_LCD3_2COMMA_OFF	lamp_lcd3[1] &= ~LAMP_LCD3_2COMMA

#define LAMP_LCD3_HOLD_ON		lamp_lcd3[2] |=  LAMP_LCD3_HOLD
#define LAMP_LCD3_HOLD_OFF		lamp_lcd3[2] &= ~LAMP_LCD3_HOLD
#define LAMP_LCD3_3RT_ON		lamp_lcd3[2] |=  LAMP_LCD3_3RT
#define LAMP_LCD3_3RT_OFF		lamp_lcd3[2] &= ~LAMP_LCD3_3RT
#define LAMP_LCD3_3COMMA_ON		lamp_lcd3[2] |=  LAMP_LCD3_3COMMA
#define LAMP_LCD3_3COMMA_OFF	lamp_lcd3[2] &= ~LAMP_LCD3_3COMMA

#define LAMP_LCD3_STABLE_ON		lamp_lcd3[3] |=  LAMP_LCD3_STABLE
#define LAMP_LCD3_STABLE_OFF	lamp_lcd3[3] &= ~LAMP_LCD3_STABLE
#define LAMP_LCD3_LB_ON			lamp_lcd3[3] |=  LAMP_LCD3_LB
#define LAMP_LCD3_LB_OFF		lamp_lcd3[3] &= ~LAMP_LCD3_LB
#define LAMP_LCD3_K_ON			lamp_lcd3[3] |=  LAMP_LCD3_K
#define LAMP_LCD3_K_OFF			lamp_lcd3[3] &= ~LAMP_LCD3_K
#define LAMP_LCD3_G_ON			lamp_lcd3[3] |=  LAMP_LCD3_G
#define LAMP_LCD3_G_OFF			lamp_lcd3[3] &= ~LAMP_LCD3_G
////////////////////////////////////////////////////////////////////////
#define LAMP_LCD2_STABLE 	0x80
#define LAMP_LCD2_LB		0x40
#define LAMP_LCD2_K			0x20
#define LAMP_LCD2_G			0x10

#define	LAMP_LCD2_ZERO		0x80
#define LAMP_LCD2_1RT		0x40
#define	LAMP_LCD2_1COMMA	0x10

#define LAMP_LCD2_TARE		0x40
#define LAMP_LCD2_2RT		0x20
#define	LAMP_LCD2_2COMMA	0x10

#define LAMP_LCD2_HOLD		0x80
#define	LAMP_LCD2_3RT		0x40
#define	LAMP_LCD2_3COMMA	0x10


#define LAMP_LCD2_STABLE_ON		lamp_lcd2[0] |=  LAMP_LCD2_STABLE
#define LAMP_LCD2_STABLE_OFF	lamp_lcd2[0] &= ~LAMP_LCD2_STABLE
#define LAMP_LCD2_K_ON			lamp_lcd2[0] |=  LAMP_LCD2_K
#define LAMP_LCD2_K_OFF			lamp_lcd2[0] &= ~LAMP_LCD2_K
#define LAMP_LCD2_G_ON			lamp_lcd2[0] |=  LAMP_LCD2_G
#define LAMP_LCD2_G_OFF			lamp_lcd2[0] &= ~LAMP_LCD2_G
#define LAMP_LCD2_LB_ON			lamp_lcd2[0] |=  LAMP_LCD2_LB
#define LAMP_LCD2_LB_OFF		lamp_lcd2[0] &= ~LAMP_LCD2_LB

#define LAMP_LCD2_ZERO_ON		lamp_lcd2[1] |=  LAMP_LCD2_ZERO
#define LAMP_LCD2_ZERO_OFF		lamp_lcd2[1] &= ~LAMP_LCD2_ZERO
#define LAMP_LCD2_1RT_ON		lamp_lcd2[1] |=  LAMP_LCD2_1RT
#define LAMP_LCD2_1RT_OFF		lamp_lcd2[1] &= ~LAMP_LCD2_1RT
#define LAMP_LCD2_1COMMA_ON		lamp_lcd2[1] |=  LAMP_LCD2_1COMMA
#define LAMP_LCD2_1COMMA_OFF	lamp_lcd2[1] &= ~LAMP_LCD2_1COMMA

#define LAMP_LCD2_TARE_ON		lamp_lcd2[2] |=  LAMP_LCD2_TARE
#define LAMP_LCD2_TARE_OFF		lamp_lcd2[2] &= ~LAMP_LCD2_TARE
#define LAMP_LCD2_2RT_ON		lamp_lcd2[2] |=  LAMP_LCD2_2RT
#define LAMP_LCD2_2RT_OFF		lamp_lcd2[2] &= ~LAMP_LCD2_2RT
#define LAMP_LCD2_2COMMA_ON		lamp_lcd2[2] |=  LAMP_LCD2_2COMMA
#define LAMP_LCD2_2COMMA_OFF	lamp_lcd2[2] &= ~LAMP_LCD2_2COMMA

#define LAMP_LCD2_HOLD_ON		lamp_lcd2[3] |=  LAMP_LCD2_HOLD
#define LAMP_LCD2_HOLD_OFF		lamp_lcd2[3] &= ~LAMP_LCD2_HOLD
#define LAMP_LCD2_3RT_ON		lamp_lcd2[3] |=  LAMP_LCD2_3RT
#define LAMP_LCD2_3RT_OFF		lamp_lcd2[3] &= ~LAMP_LCD2_3RT
#define LAMP_LCD2_3COMMA_ON		lamp_lcd2[3] |=  LAMP_LCD2_3COMMA
#define LAMP_LCD2_3COMMA_OFF	lamp_lcd2[3] &= ~LAMP_LCD2_3COMMA






// UNIT	
#define UNIT_KG	0
#define UNIT_G 1
#define	UNIT_LB 2










#endif

