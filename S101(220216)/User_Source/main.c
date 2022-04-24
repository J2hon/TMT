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

// Version T215 >> S101 
// Changing the calibration method from the existing T215 system.
// Change it so that you can select print and backlight through the enter key.

int main(void)
{
	Init_Device();

	String_Display(PAY, " -PS- ");
	String_Display(UNIT_PRICE, "S=101");

	DelayMs(700);
	Func_Check();

	Load_Cell_Check();
    AdcDataInit();
	Weight_Init();
    
	while(1)
	{
		Low_Batt_check();
		Display_Task();
		Weight_Task();
		Key_Task();
		Uart_Task();
		Sub_Task();
	}

    return(0);
}



