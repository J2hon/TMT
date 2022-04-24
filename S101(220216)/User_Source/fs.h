#ifndef _FS_H 
#define _FS_H 
 
//AT45DB041D有两个缓冲区Buffer1和Buffer2 (SRAM) 
 
#define Buffer1       1 
#define Buffer2       2 
#define ReadBuffer    1 
#define WriteBuffer   2 
 
/****************************** 
*opcode-操作码 
******************************/ 
//读取状态的操作码 
#define Status_Register_Opcode   0xD7 
//读取ID的操作码 
#define Device_ID_Opcode         0x9F 
 
//Read from buffer 
#define Read_Data_from_Buffer1  0xD4 
#define Read_Data_from_Buffer2  0xD6 
#define Read_Data_from_Buffer1_lower_Frequency  0xD1   //lower frequency 
#define Read_Data_from_Buffer2_lower_Frequency  0xD3   //lower frequency 
 
//Write to buffer 
#define Write_Data_to_Buffer1   0x84 
#define Write_Data_to_Buffer2   0x87 
 
//Read page to buffer 
#define Read_Page_to_Buffer1   0x53 
#define Read_Page_to_Buffer2   0x55 
 
//Write buffer to page 
#define Write_Buffer1_to_Page_whin_Erase   0x83 
#define Write_Buffer2_to_Page_whin_Erase   0x86 
 
//Continuous Array Read 
#define Continuous_Array_Read_Command   0xE8 
 
//Page to Buffer Compare 
#define Page_to_Buffer1_Compare   0x60 
#define Page_to_Buffer2_Compare   0x61 
 
//Main Memory Page Program Through Buffer 
#define Main_Memory_Page_Program_Through_Buffer1   0x82 
#define Main_Memory_Page_Program_Through_Buffer2   0x85 
 
//Main Memory Page Read 
#define Main_Memory_Page_Read_Command   0xD2 
 
#endif