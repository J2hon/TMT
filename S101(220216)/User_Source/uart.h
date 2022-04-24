#ifndef _UART_H_
#define _UART_H_

void Init_Uart_1(void);
void Initial_UART2(void); // Uart Debug
void Initial_UART3(void); // For EX_Uart
void Initial_UART4(void); // For BlueTooth



void Uart_Task(void);

void rs_com(unsigned long);
void com_send(unsigned char, unsigned char, unsigned long, unsigned long);
void com_print_form(unsigned char, unsigned long, unsigned long);
void buffer1_initial(void);

void s_out1(unsigned char bData);
void s_out2(unsigned char bData);
void s_out3(unsigned char bData);
void s_out4(unsigned char bData);


void dst_slave(void);
void dst_return(void);
void dst_ne_slave(void);
void dst_ne_return(void);
void tf200_send(unsigned long);  // 18 byte (and format)
void netid_mode(void);
void tf200_send2(unsigned long); // 18 byte (and format)
void print(unsigned long rs_weight);


#endif 
