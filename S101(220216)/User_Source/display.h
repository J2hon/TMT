#ifndef _DISPLAY_H_
#define _DISPLAY_H_

void lcd_initial(void);
void lcd_command(unsigned char, unsigned char);
void lcd_want_display(unsigned char, unsigned char, unsigned char);



void weight_display(unsigned long);
void lcd_command(unsigned char , unsigned char);
void Display_Task(void);
void pay_display(unsigned long,  unsigned char);
void pay_count(unsigned long pay_data);
void unit_price_display(unsigned long);
void plu_display(unsigned char);
unsigned long input_number(unsigned long);
void String_Display(unsigned char,char *);
void Lamp_On(unsigned char);
void Lamp_Off(unsigned char);
void weight_up_lamp(void);	// 
void Lamp_All_Off_weight(void);
void Lamp_All_On_Weight(void);
void Lamp_All_Off_u_p(void);
void Lamp_All_On_u_p(void);
void test_lcd_seg_on(void);
void test_lcd_part_off(void);
void test_lcd_keytest_off(void);




//void lcd_want_display(unsigned char, unsigned char);






#endif 















