#include <stdio.h>
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




////////////////////////////////////////////////////////////////////////////////
void atflash_command(unsigned char command)
{
	unsigned char  	i=0;

	for(i=0; i<8; i++)	// COMMNAND
	{
		if(command & 0x80)	sbi(LATF, F_MOSI); 
		else				cbi(LATF, F_MOSI);
		
		sbi(LATF, F_SCK);
		cbi(LATF, F_SCK);
		
		command <<= 1;
	}	
}
////////////////////////////////////////////////////////////////////////////////
unsigned char atflash_read_byte(void)
{
	unsigned char i=0;
	unsigned char read_byte=0;

    for(i=0; i<8; i++)
	{
		read_byte <<= 1;
		
		if( ((PORTF&(1<<F_MISO))==(1<<F_MISO)) )	read_byte |= 0x01;
		else                                      	read_byte &= 0xFE;

		
		sbi(LATF, F_SCK);
		cbi(LATF, F_SCK);
	} 

	return read_byte;
}
////////////////////////////////////////////////////////////////////////////////
void atflash_dummy_bit(unsigned char dummy_size)
{
	unsigned char i=0;

#if 0
	sbi(LATF, F_MOSI); 
	cbi(LATF, F_SCK);

	for(i=0; i<dummy_size; i++)  
	{
		sbi(LATF, F_SCK);
		cbi(LATF, F_SCK);
	}
#endif 

#if 1
	for(i=0; i<dummy_size; i++)  
	{
		sbi(LATF, F_MOSI); 
		sbi(LATF, F_SCK);
		cbi(LATF, F_SCK);
	}
#endif 

}


////////////////////////////////////////////////////////////////////////////////
void atflash_data_bit(unsigned short data_value, unsigned char data_size)
{
	unsigned char i=0;
	unsigned short compare_msb=0;


	switch(data_size)
	{
		case  8: compare_msb= 0x80;	break;
		case  9: compare_msb=0x100;	break;
		case 11: compare_msb=0x400;	break;
		default:					break;
	}

	for(i=0; i<data_size; i++)	
	{
		if(data_value & compare_msb)	sbi(LATF, F_MOSI); 
		else							cbi(LATF, F_MOSI);
		
		sbi(LATF, F_SCK);
		cbi(LATF, F_SCK);
		
		data_value <<= 1;
	}	

}


////////////////////////////////////////////////////////////////////////////////
void atflash_getmanuid(void)
{
	unsigned char  	i=0;
	unsigned char	ManufacturerID[3]={0};




	cbi(LATF, F_CS);

	atflash_command(ManufacturerandDeviceIDRead);
	ManufacturerID[0]=atflash_read_byte();	// 31=0x1F=Atmel
	ManufacturerID[1]=atflash_read_byte();	// 36=0x24=DataFlash,16-Mbit
	ManufacturerID[2]=atflash_read_byte();	// 00=0x00=1-bit/Cell Technology,Initial Version

	sbi(LATF, F_CS);

}


////////////////////////////////////////////////////////////////////////////////
/* busy state: 
1. Main Memory Page to Buffer Transfer, 
2. Main Memory Page to Buffer Compare,
3. Buffer to Main Memory Page Program,
4. Main Memory Page Program through Buffer, 
5. Page Erase,
6. Block Erase,
7. Sector Erase,
8. Chip Erase and Auto Page Rewrite. */
unsigned char atflash_readstatus(void)
{
	unsigned char	status=0xFF;


	cbi(LATF, F_CS);

	atflash_command(StatusRegisterRead);
	status=atflash_read_byte();

	sbi(LATF, F_CS);


	return status;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
#define ReservedBit4 4
#define ReservedBit5 5
#define DontCareBit11 11
#define BufferAddressBit8 8
#define BufferAddressBit9 9
void atflash_buffer1write(unsigned char buffer_address, unsigned long buffer_data)
{
	unsigned char i=0;
	unsigned char buf_data[4]={0};
	
	buf_data[0] = (unsigned char)(buffer_data >> 24);   
	buf_data[1] = (unsigned char)(buffer_data >> 16);    
	buf_data[2] = (unsigned char)(buffer_data >> 8);    
	buf_data[3] = (unsigned char)(buffer_data >> 0);	

		
	cbi(LATF, F_CS);

	atflash_command(Buffer1Write);
	atflash_dummy_bit(ReservedBit5);
	atflash_dummy_bit(DontCareBit11);
	atflash_data_bit(buffer_address, BufferAddressBit8);


	for(i=0; i<4; i++)
	{
		atflash_data_bit(buf_data[i], 8);
	}
	
	sbi(LATF, F_CS);
}


////////////////////////////////////////////////////////////////////////////////
void atflash_page_write(unsigned short page_address)
{
	ATflash_Busy_Flag;
	cbi(LATF, F_CS);

	atflash_command(Buffer1toMainMemoryPageProgramwithoutErase);
	atflash_dummy_bit(ReservedBit5);
	atflash_data_bit(page_address, 11);
	atflash_dummy_bit(8);

	sbi(LATF, F_CS);
	ATflash_Busy_Flag;
}


////////////////////////////////////////////////////////////////////////////////
void atflash_page_read(unsigned short page_address)
{
	ATflash_Busy_Flag;
	cbi(LATF, F_CS);

	atflash_command(MainMemoryPageProgramThroughBuffer2);
	atflash_dummy_bit(ReservedBit5);
	atflash_data_bit(page_address, 11);
	atflash_dummy_bit(8);

	sbi(LATF, F_CS);
	ATflash_Busy_Flag;
}


////////////////////////////////////////////////////////////////////////////////
unsigned long atflash_buffer2read(unsigned char buffer_address)
{
	unsigned char	read_data[4]={0};
	unsigned long	read_value=0;
	unsigned char	i=0;

	cbi(LATF, F_CS);

	atflash_command(Buffer2Read);
	atflash_dummy_bit(ReservedBit5);
	atflash_dummy_bit(DontCareBit11);						
	atflash_data_bit(buffer_address, BufferAddressBit8);
	atflash_dummy_bit(8);

	for(i=0; i<4; i++)
	{	
		read_data[i]=atflash_read_byte();
	}


	sbi(LATF, F_CS);



	read_value = ( (unsigned long)read_data[0]<<24)
				 |((unsigned long)read_data[1]<<16)
				 |((unsigned long)read_data[2]<<8)
				 |((unsigned long)read_data[3]);

	return read_value;
}

void Atflash_PluData_Push(unsigned short page_address, unsigned char buffer_address, unsigned long buffer_data)
{
	ATflash_Busy_Flag;
	cbi(LATF, F_CS);

	atflash_command(MainMemoryPageToBuffer1Transfer);
	atflash_dummy_bit(ReservedBit5);
	atflash_data_bit(page_address, 11);
	atflash_dummy_bit(8);

	sbi(LATF, F_CS);
	ATflash_Busy_Flag;

	atflash_buffer1write(buffer_address, buffer_data);	
	atflash_page_write(page_address);		
}

unsigned long Atflash_PluData_Pop(unsigned short page_address, unsigned char buffer_address)
{
	unsigned long buffer_data_temp=0;

	
	atflash_page_read(page_address);	
	buffer_data_temp=atflash_buffer2read(buffer_address);	

	return( buffer_data_temp);
}

///////////////////////////////////////////////////////////////////////////////////////////////
unsigned long Atflash_Dtp_Rtc_Pop(unsigned short page_address, unsigned char buffer_address)
{
	unsigned long buffer_data_temp=0;

	
	atflash_page_read(page_address);	
	buffer_data_temp=atflash_buffer2read(buffer_address);	

	return( buffer_data_temp);
}

///////////////////////////////////////////////////////////////////////////////////////////////
unsigned long atflash_buffer2read_1byte(unsigned char buffer_address)
{
	unsigned char	read_data[2]={0};
	unsigned short	read_value=0;
	unsigned char	i=0;

	cbi(LATF, F_CS);

	atflash_command(Buffer2Read);
	atflash_dummy_bit(ReservedBit5);
	atflash_dummy_bit(DontCareBit11);						
	atflash_data_bit(buffer_address, BufferAddressBit8);
	atflash_dummy_bit(8);

	for(i=0; i<2; i++)
	{	
		read_data[i]=atflash_read_byte();
	}


	sbi(LATF, F_CS);



	read_value = ((unsigned long)read_data[0]<<8)
				 |((unsigned long)read_data[1]);

	return read_value;
}

void Atflash_Dtp_Data_Push(unsigned short page_address, unsigned char buffer_address, unsigned long buffer_data)
{
	unsigned char 	x_key=0;
	unsigned char 	key_cnt=0;

	
	ATflash_Busy_Flag;
	cbi(LATF, F_CS);

	atflash_command(MainMemoryPageToBuffer1Transfer);
	atflash_dummy_bit(ReservedBit5);
	atflash_data_bit(page_address, 11);
	atflash_dummy_bit(8);

	sbi(LATF, F_CS);
	ATflash_Busy_Flag;

	atflash_buffer1write(buffer_address, buffer_data);	
	atflash_page_write(page_address);		


	ex_byte_addr+=13;

	if( ex_byte_addr>240)										//	Max.246
	{
		ex_byte_addr=0;
		ex_page_addr++;
	}

	if(ex_page_addr>1999)
	{ 
		while(1) 
		{
			String_Display(PAY, "M-FULL"); 

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
	}

	ex_serial_no++;
	if(ex_serial_no>38000)
	{
		ex_serial_no=1;
	}


	ATflash_Busy_Flag;
	cbi(LATF, F_CS);

	atflash_command(MainMemoryPageToBuffer1Transfer);
	atflash_dummy_bit(ReservedBit5);
	atflash_data_bit(page_address, 11);
	atflash_dummy_bit(8);

	sbi(LATF, F_CS);
	ATflash_Busy_Flag;

	atflash_buffer1write(0, ex_serial_no);	
	atflash_page_write(2010);		

	
}







/* 
@Page 22~23
7bit-1 : the device is not busy state.
7bit-0 : the device is in a busy state.
6bit-1 : then at least one bit of the data in the main memory page does not match the data in the buffer.
6bit-0 : then the data in the main memory page matches the data in the buffer.
1bit-1 : A logic 1 indicates that sector protection has been enabled
1bit-0 : A logic 0 indicates that sector protection has been disabled.
0bit-1 : the page size is set to 256 bytes.
0bit-0 : then the page size is set to 264 bytes.

@Page 25
Power of 2” binary page size Configuration Register is a user-programmable nonvolatile register
that allows the page size of the main memory to be configured for binary page size
(256 bytes) or the DataFlash standard page size (264 bytes).

1. Program the one-time programmable configuration resister using opcode sequence
3DH, 2AH, 80H and A6H (please see Section 13.1).
2. Power cycle the device (i.e. power down and power up again).
3. The page for the binary page size can now be programmed.
*/
void atflash_init(void)
{
	unsigned char f_read_status=0;

	
	f_read_status=atflash_readstatus();

	//if( (f_read_status&PageSizeBit)==Buffer264Byte)	// 264면 256으로 바꿈
	{
		cbi(LATF, F_CS);
		
		atflash_data_bit(0x3D, 8);
		atflash_data_bit(0x2A, 8);
		atflash_data_bit(0x80, 8);
		atflash_data_bit(0xA6, 8);

		sbi(LATF, F_CS);
	}

	DelayMs(100);
}





#if 0
void f_data_to_flash(void)
{
	Atflash_Dtp_Data_Push(f_page_addr, f_byte_addr, unsigned long buffer_data);

	f_byte_addr=f_byte_addr+12;


	if(f_byte_addr>250)
	{
		f_byte_addr=0;
		f_page_addr=f_page_addr+1;
	}

	if(f_page_addr>2000)
	{
		f_page_addr=2;
	}
#endif 

	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void Atflash_Dtp_Day_f_Write(void)
{
	unsigned char	x_key=0;
	unsigned char	key_cnt=0;
	
	// Read
	ATflash_Busy_Flag;
	cbi(LATF, F_CS);

	atflash_command(MainMemoryPageToBuffer1Transfer);
	atflash_dummy_bit(ReservedBit5);
	atflash_data_bit(f_w_page_addr, 11);
	atflash_dummy_bit(8);

	sbi(LATF, F_CS);




	// Write
	ATflash_Busy_Flag;
	Atflash_Dtp_Day_b1_Write();	
	atflash_page_write((unsigned short)f_w_page_addr);		


	f_w_byte_addr+=DAY_DTP_GAP_BYTE;

	if( ex_byte_addr>DAY_DTP_TAIL_BYTE)										//	Max.251
	{
		f_w_byte_addr=DAY_DTP_HEAD_BYTE;
		f_w_page_addr++;
	}

	if(f_w_page_addr>DAY_DTP_TAIL)
	{ 
		while(1) 
		{
			String_Display(PAY, "M-FULL"); 

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
	}

	ex_serial_no++;
	if(ex_serial_no>DTP_SERIAL_NO_TAIL)
	{
		ex_serial_no=DTP_SERIAL_NO_HEAD;
	}


	ATflash_Busy_Flag;
	cbi(LATF, F_CS);

	atflash_command(MainMemoryPageToBuffer1Transfer);
	atflash_dummy_bit(ReservedBit5);
	atflash_data_bit(DTP_SERIAL, 11);
	atflash_dummy_bit(8);

	sbi(LATF, F_CS);
	ATflash_Busy_Flag;

	atflash_buffer1write(DTP_SERIAL_BUF, ex_serial_no);	
	atflash_page_write(DTP_SERIAL);		

	
}

void Atflash_Dtp_Day_b1_Write(void)
{
	unsigned char dtp_buffer_loop=0;

	
	for(dtp_buffer_loop=0; dtp_buffer_loop<DAY_DTP_GAP_BYTE; dtp_buffer_loop++)
	{
		ex_fw_buffer[dtp_buffer_loop]=0;
	}


	ex_fw_buffer[0]=(sRTC.year);
	ex_fw_buffer[1]=(sRTC.month);
	ex_fw_buffer[2]=(sRTC.day);
	ex_fw_buffer[3]=(sRTC.hour);
	atflash_command(Buffer1Write);

	ex_fw_buffer[4]=(sRTC.minute);
	ex_fw_buffer[5]=(sRTC.second);
	ex_fw_buffer[6]=(unsigned char)(((unsigned short)ex_serial_no)>>8);
	ex_fw_buffer[7]=(unsigned char)(((unsigned short)ex_serial_no)>>0);
	ex_fw_buffer[8]=((unsigned char)(ex_weight_sum_value >> 24));
	ex_fw_buffer[9]=((unsigned char)(ex_weight_sum_value >> 16));
	ex_fw_buffer[10]=((unsigned char)(ex_weight_sum_value >> 8));
	ex_fw_buffer[11]=((unsigned char)(ex_weight_sum_value >> 0));


	cbi(LATF, F_CS);
	atflash_dummy_bit(ReservedBit5);
	atflash_dummy_bit(DontCareBit11);

	
	atflash_data_bit(f_w_byte_addr, BufferAddressBit8);		//	byte address

	for(dtp_buffer_loop=0; dtp_buffer_loop<DAY_DTP_GAP_BYTE; dtp_buffer_loop++)
	{
		atflash_data_bit(ex_fw_buffer[dtp_buffer_loop], 8);
	}
	
	sbi(LATF, F_CS);
}


void scan_Dtp_Day(void)
{

}





