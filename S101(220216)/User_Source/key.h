#ifndef _KEY_H_
#define _KEY_H_

void Key_Scan(void);
void Key_Task(void);
unsigned char Key_Data_Read(void);
unsigned long Input_Number_Long(unsigned long, unsigned char);
unsigned short Input_Plu_Number_Char(unsigned char);
unsigned long Input_U_P_Number_Long(unsigned long);
unsigned char Input_Number_Weight_ONE(unsigned char);
void Func_Check(void);

unsigned  char two_byte(unsigned char);
unsigned short three_byte(unsigned short);
unsigned long five_byte(unsigned long);

void Initial_IC(void);
void IC_buffer_clr(void);

unsigned char LCD2_2Byte_Input_Blink(unsigned char, unsigned char);
unsigned char LCD3_1Byte_test_mode_Input_Blink(unsigned char, unsigned char, unsigned char);
unsigned char LCD3_2Byte_test_mode_Input_Blink(unsigned char);

unsigned char Pass_Word_Input(unsigned char);

#endif 
