#ifndef _FLASH_H_
#define _FLASH_H_



void atflash_command(unsigned char);
unsigned char atflash_read_byte(void);
void atflash_dummy_bit(unsigned char);
void atflash_data_bit(unsigned short, unsigned char);

void atflash_getmanuid(void);
unsigned char atflash_readstatus(void);


void atflash_buffer1write(unsigned char, unsigned long);
void atflash_page_write(unsigned short);
void atflash_page_read(unsigned short);
unsigned long atflash_buffer2read(unsigned char);

void Atflash_PluData_Push(unsigned short, unsigned char, unsigned long);
unsigned long Atflash_PluData_Pop(unsigned short, unsigned char);

void atflash_init(void);


void pay_sum_to_flash(void);


void Atflash_Dtp_Day_f_Write(void);
void Atflash_Dtp_Day_b1_Write(void);
#endif 

