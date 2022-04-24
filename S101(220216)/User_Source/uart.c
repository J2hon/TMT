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



#define  BAUDRATE 	38400            	// Baudrate
#define  GetSystemClock()            8000000UL/2



void Init_Uart_1( void )	//	For Zigbee
{
	float 			BRGVAL;
	unsigned short 	u16_y;


	RPINR18bits.U1RXR = 7; // RP7 = RXD1
	RPOR3bits.RP6R    = 3; // RP6 = TXD1

	U1MODEbits.STSEL  = 0; // Stop bit 1
	U1MODEbits.PDSEL  = 0; // Non parity, Data 8bit
	U1MODEbits.ABAUD  = 0; // Stop autobaud
	U1MODEbits.UARTEN = 1; // UART1 enable
	U1MODEbits.BRGH   = 1; // High speed

	BRGVAL = (((float)GetSystemClock())/((float)BAUDRATE)/4.0) - 1.0;
	u16_y = BRGVAL;

	if ((BRGVAL - u16_y) < 0.5) U1BRG = u16_y; 
	else                        U1BRG = u16_y+1; 

	U1STAbits.UTXEN   = 1; // UART1 Tx enable

	IFS0bits.U1TXIF   = 0; // UART1 tx interrupt flag clr
	IFS0bits.U1RXIF   = 0; // UART1 rx interrupt flag clr

	IEC0bits.U1TXIE = 0;   // UART1 tx interrupt disable
	IEC0bits.U1RXIE = 1;   // UART1 rx interrupt enable

	IEC4bits.U1ERIE = 0;   // UART1 error interrupt disable
}


void Initial_UART2( void ) // Uart Debug
{
  float 			BRGVAL;
  unsigned short 	u16_y;

  RPINR19bits.U2RXR = 8; // RP8 = RXD2
  RPOR4bits.RP9R    = 5; // RP9 = TXD2

  U2MODEbits.STSEL  = 0; // Stop bit 1
  U2MODEbits.PDSEL  = 0; // Non parity, Data 8bit
  U2MODEbits.ABAUD  = 0; // Stop autobaud
  U2MODEbits.UARTEN = 1; // UART2 enable
  U2MODEbits.BRGH   = 1; // High speed

  BRGVAL = (((float)GetSystemClock())/((float)9600)/4.0) - 1.0;
  u16_y = BRGVAL;
  if ((BRGVAL - u16_y) < 0.5) U2BRG = u16_y;
  else                        U2BRG = u16_y+1;

  U2STAbits.UTXEN   = 1; // UART2 Tx enable

  IFS1bits.U2TXIF   = 0; // UART2 tx interrupt flag clr
  IFS1bits.U2RXIF   = 0; // UART2 rx interrupt flag clr

  IEC1bits.U2TXIE = 0;   // UART2 tx interrupt disable
  IEC1bits.U2RXIE = 1;   // UART2 rx interrupt enable

  IEC4bits.U2ERIE = 0;   // UART2 error interrupt disable
}

void Initial_UART3( void ) // For EX_Uart
{
	float 			BRGVAL;
	unsigned short 	u16_y;


	RPINR17bits.U3RXR = 23; // RP23 = RXD3
	RPOR12bits.RP24R  = 28; // RP24 = TXD3

	U3MODEbits.STSEL  = 0; // Stop bit 1
	U3MODEbits.PDSEL  = 0; // Non parity, Data 8bit
	U3MODEbits.ABAUD  = 0; // Stop autobaud
	U3MODEbits.UARTEN = 1; // UART3 enable
	U3MODEbits.BRGH   = 1; // High speed

#if 0
	if(!v_baud_rate)        BRGVAL = 2400;
	else if(v_baud_rate==1) BRGVAL = 4800;
	else if(v_baud_rate==2) BRGVAL = 9600;
	else if(v_baud_rate==3) BRGVAL = 19200;
	else if(v_baud_rate==4) BRGVAL = 38400;

	BRGVAL = (((float)GetSystemClock())/((float)BRGVAL)/4.0) - 1.0;
	u16_y = BRGVAL;
#endif 

	BRGVAL = (((float)GetSystemClock())/((float)9600)/4.0) - 1.0;
	u16_y = BRGVAL;

	if ((BRGVAL - u16_y) < 0.5) U3BRG = u16_y; 
	else                        U3BRG = u16_y+1; 

	U3STAbits.UTXEN   = 1; // UART3 Tx enable

	IFS5bits.U3TXIF   = 0; // UART3 tx interrupt flag clr
	IFS5bits.U3RXIF   = 0; // UART3 rx interrupt flag clr

	IEC5bits.U3TXIE = 0;   // UART3 tx interrupt disable
	IEC5bits.U3RXIE = 1;   // UART3 rx interrupt enable

	IEC5bits.U3ERIE = 0;   // UART3 error interrupt disable
}


void Initial_UART4( void ) // For BlueTooth
{
	float BRGVAL;
	unsigned int u16_y;

	RPINR27bits.U4RXR = 30; // RP30 = RXD4
	RPOR8bits.RP16R  = 30;  // RP16 = TXD4

	U4MODEbits.STSEL  = 0; // Stop bit 1
	U4MODEbits.PDSEL  = 0; // Non parity, Data 8bit
	U4MODEbits.ABAUD  = 0; // Stop autobaud
	U4MODEbits.UARTEN = 1; // UART4 enable
	U4MODEbits.BRGH   = 1; // High speed

	BRGVAL = (((float)GetSystemClock())/((float)9600)/4.0) - 1.0;
	u16_y = BRGVAL;
	if ((BRGVAL - u16_y) < 0.5) U4BRG = u16_y; 
	else                        U4BRG = u16_y+1; 

	U3STAbits.UTXEN   = 1; // UART3 Tx enable

	IFS5bits.U4TXIF	= 0; // UART4 tx interrupt flag clr
	IFS5bits.U4RXIF	= 0; // UART4 rx interrupt flag clr

	IEC5bits.U4TXIE	= 0;   // UART4 tx interrupt disable
	IEC5bits.U4RXIE	= 1;   // UART4 rx interrupt enable

	
	IEC5bits.U4ERIE = 0;   // UART4 error interrupt disable
}







void Uart_Task(void)
{
    static unsigned char chk_flg;
    static unsigned long data_long, data_long2;
   
    unsigned char i, comm;
 
    if(print_flag==1)	print(ex_e_value);
    if(ep_wireless==0 || ep_wireless == 3 || ep_wireless == 4)
    {
        if(ep_wireless == 4) comm = 4;      // B/T
        else if(ep_wireless == 0) comm = 3; // RS-232
        else if(ep_wireless == 3) {comm = 1; dst_ne_slave();}  // TF-200 
        
        if(ep_rs_form == 1)  //weight
        {
            if(!ex_hold_finish)com_send(0,comm,ex_e_value, 0);
            else com_send(0,comm,ex_weight_hold_value, 0);
        }
        else if(ep_rs_form == 2)  //price
        {
            com_send(2,comm,0, cut_pay);
        }
        else if(ep_rs_form == 3)  //weight/unit/price
        {
            com_send(1,comm,ex_e_value, cut_pay);
        }
        else if(ep_rs_form == 4)  //stable print
        {
            if(ex_stab_flag && (ex_e_value > weight_division*ep_hold_zero))
            {
                if(ex_hold_finish) 
                {
                    data_long = ex_weight_hold_value;
                    pay_display( (ex_weight_hold_value*ex_atflash_PluWriteValue), 0);
                    data_long2 = cut_pay;
                }
                else 
                {
                    data_long = ex_e_value;
                    data_long2 = cut_pay;
                }

                chk_flg = 1;
            }
            else if(ex_stab_flag && chk_flg  && (ex_e_value<=weight_division*ep_hold_zero))
            {
                //com_send(1,3,data_long);
                com_print_form(comm, data_long, data_long2);
                chk_flg = 0;
            }
        }
        else if(ep_rs_form == 5)  //stable weight
        {
            if(ex_stab_flag && (ex_e_value > weight_division*ep_hold_zero))
            {
                data_long = ex_e_value;
                chk_flg = 1;
            }
            else if(ex_stab_flag && chk_flg  && (ex_e_value<=weight_division*ep_hold_zero))
            {
                com_send(0,comm,data_long, 0);
                chk_flg = 0;
            }
        }
        else if(ep_rs_form == 6)  //stable price
        {
            if(ex_stab_flag && (ex_e_value > weight_division*ep_hold_zero))
            {
                //pay_display( (ex_weight_hold_value*ex_atflash_PluWriteValue), 0);
                data_long = cut_pay;
                chk_flg = 1;
            }
            else if(ex_stab_flag && chk_flg  && (ex_e_value<=weight_division*ep_hold_zero))
            {
                com_send(2,comm,0,data_long);
                chk_flg = 0;
            }
        }
        else if(ep_rs_form == 7)  //stable weight unit price
        {
            if(ex_stab_flag && (ex_e_value > weight_division*ep_hold_zero))
            {
                data_long = ex_e_value;
                data_long2 = cut_pay;
                chk_flg = 1;
            }
            else if(ex_stab_flag && chk_flg  && (ex_e_value<=weight_division*ep_hold_zero))
            {
                com_send(1,comm,data_long,data_long2);
                chk_flg = 0;
            }
        }
        else if(ep_rs_form == 8)  //hold print
        {
            if(ex_hold_finish && !chk_flg && (ex_e_value > weight_division*ep_hold_zero))
            {
                pay_display( (ex_weight_hold_value*ex_atflash_PluWriteValue), 0);
                com_print_form(comm, ex_weight_hold_value, cut_pay);
                chk_flg = 1;
            }
            else if(ex_e_value<=weight_division*ep_hold_zero)
            {
                chk_flg = 0;
            }
        }
        else if(ep_rs_form == 9)  //hold weight
        {
            if(ex_hold_finish && !chk_flg && (ex_e_value > weight_division*ep_hold_zero))
            {
                com_send(0,comm,ex_weight_hold_value,0);
                chk_flg = 1;
            }
            else if(ex_e_value<=weight_division*ep_hold_zero)
            {
                chk_flg = 0;
            }
        }
        else if(ep_rs_form == 10)  //hold price
        {
            if(ex_hold_finish && !chk_flg && (ex_e_value > weight_division*ep_hold_zero))
            {
                pay_display( (ex_weight_hold_value*ex_atflash_PluWriteValue), 0);
                com_send(2,comm,0,cut_pay);
                chk_flg = 1;
            }
            else if(ex_e_value<=weight_division*ep_hold_zero)
            {
                chk_flg = 0;
            }
        }
        else if(ep_rs_form == 11)  //hold weight unit price
        {
            if(ex_hold_finish && !chk_flg && (ex_e_value > weight_division*ep_hold_zero))
            {
                com_send(1,comm,ex_weight_hold_value, cut_pay);
                chk_flg = 1;
            }
            else if(ex_e_value<=weight_division*ep_hold_zero)
            {
                chk_flg = 0;
            }
        }
        else if(ep_rs_form == 12)  // commnad only RS-232(zigbee X)
        {
            if(Rx3_flg)
            {
                switch(sin_buffer3[0])
                {
                    case 'R':

                        if(sin_buffer3[1] == 'W') // read weigth
                        {
                            com_send(0,3,ex_e_value,0);
                        }
                        else if(sin_buffer3[1] == 'U') //read unit
                        {
                            com_send(4,3,ex_atflash_PluWriteValue,0);
                        }
                        else if(sin_buffer3[1] == 'P') // read price
                        {
                            com_send(2,3,0,cut_pay);
                        }
                        else if(sin_buffer3[1] == 'A') //read weight/unit/price
                        {
                            com_send(1,3,ex_e_value,cut_pay);
                        }
                        break;

                    case 'W':
                        if(sin_buffer3[1] == 'Z')  //zero
                        {
                            zero_function_key();
                        }
                        else if(sin_buffer3[1] == 'T') //tare
                        {
                            tare_function_key();
                        }
                        else if(sin_buffer3[1] == 'H') //hold
                        {
                            hold_function_key();
                        }
                        else if(sin_buffer3[1] == 'K' && sin_buffer3[2] == 'T') //key tare
                        {
                            data_long = 0;

                            if(sin_buffer3[3] >= 0x30 && sin_buffer3[3] <= 0x39)
                            {
                                data_long = ((sin_buffer3[3] - 0x30) * 10000);
                            }    
                            if(sin_buffer3[4] >= 0x30 && sin_buffer3[4] <= 0x39)
                            {
                                data_long = data_long + ((sin_buffer3[4] - 0x30) * 1000);
                            }
                            if(sin_buffer3[5] >= 0x30 && sin_buffer3[5] <= 0x39)
                            {
                                data_long = data_long + ((sin_buffer3[5] - 0x30) * 100);
                            }
                            if(sin_buffer3[6] >= 0x30 && sin_buffer3[6] <= 0x39)
                            {
                                data_long = data_long + ((sin_buffer3[6] - 0x30) * 10);
                            }
                            if(sin_buffer3[7] >= 0x30 && sin_buffer3[7] <= 0x39)
                            {
                                data_long = data_long + ((sin_buffer3[7] - 0x30) );
                            }

                            if(!ep_tare_flag)
                            {
                                ep_tare_value = (long)((float)(((data_long/weight_division)*20))/((float)ep_gravity_factor1/(float)ep_gravity_factor2))+ex_adc_org;
                                ep_tare_flag=1;
                            }
                            else
                            {
                                ep_tare_value = ex_adc_org;
                                ep_tare_flag=0;
                            }
                            data_long = 0;
                            ex_key_tare_tick=700;

                            eeprom_write();
                        }
                        else if(sin_buffer3[1] == 'C' && sin_buffer3[2] == 'U') //unit price (0)
                        {
                            data_long = 0;
                            if(sin_buffer3[3] >= 0x30 && sin_buffer3[3] <= 0x39)
                            {
                                data_long = (sin_buffer3[3] - 0x30) * 10000;
                            }    
                            if(sin_buffer3[4] >= 0x30 && sin_buffer3[4] <= 0x39)
                            {
                                data_long = data_long + (sin_buffer3[4] - 0x30) * 1000;
                            }
                            if(sin_buffer3[5] >= 0x30 && sin_buffer3[5] <= 0x39)
                            {
                                data_long = data_long + (sin_buffer3[5] - 0x30) * 100;
                            }
                            if(sin_buffer3[6] >= 0x30 && sin_buffer3[6] <= 0x39)
                            {
                                data_long = data_long + (sin_buffer3[6] - 0x30) * 10;
                            }
                            if(sin_buffer3[7] >= 0x30 && sin_buffer3[7] <= 0x39)
                            {
                                data_long = data_long + (sin_buffer3[7] - 0x30);
                            }
                            ex_atflash_PluNum=0;
                            ex_atflash_PluWriteValue=data_long;
                            unit_price_display(ex_atflash_PluWriteValue);
                            Atflash_PluData_Push(PLU_U_P_HEAD, (ex_atflash_PluNum*4), ex_atflash_PluWriteValue);
                            Atflash_PluData_Push(PLU_CUR, 200, ex_atflash_PluNum);
                            data_long = 0;
                        }
                        else if(sin_buffer3[1] == 'U' && sin_buffer3[2] == 'P') //unit price (1~99))
                        {
                            data_long = 0; data_long2 = 0;
                            if(sin_buffer3[3] >= 0x30 && sin_buffer3[3] <= 0x39)
                            {
                                data_long = (sin_buffer3[3] - 0x30) * 10;
                            }    
                            if(sin_buffer3[4] >= 0x30 && sin_buffer3[4] <= 0x39)
                            {
                                data_long = data_long + (sin_buffer3[4] - 0x30);
                            }           

                            if(sin_buffer3[5] >= 0x30 && sin_buffer3[5] <= 0x39)
                            {
                                data_long2 = ((sin_buffer3[5] - 0x30) * 10000);
                            }    
                            if(sin_buffer3[6] >= 0x30 && sin_buffer3[6] <= 0x39)
                            {
                                data_long2 = data_long2 + ((sin_buffer3[6] - 0x30) * 1000);
                            }
                            if(sin_buffer3[7] >= 0x30 && sin_buffer3[7] <= 0x39)
                            {
                                data_long2 = data_long2 + ((sin_buffer3[7] - 0x30) * 100);
                            }
                            if(sin_buffer3[8] >= 0x30 && sin_buffer3[8] <= 0x39)
                            {
                                data_long2 = data_long2 + ((sin_buffer3[8] - 0x30) * 10);
                            }
                            if(sin_buffer3[9] >= 0x30 && sin_buffer3[9] <= 0x39)
                            {
                                data_long2 = data_long2 + ((sin_buffer3[9] - 0x30));
                            }

                            ex_atflash_PluNum = data_long;
                            ex_atflash_PluWriteValue = data_long2;
                            if(ex_atflash_PluNum<50)	
                                Atflash_PluData_Push(PLU_U_P_HEAD, (ex_atflash_PluNum*4), ex_atflash_PluWriteValue);
                            else					
                                Atflash_PluData_Push(PLU_U_P_TAIL, ( (ex_atflash_PluNum-50)*4 ), ex_atflash_PluWriteValue );

                            Atflash_PluData_Push(PLU_CUR, 200, ex_atflash_PluNum);

                            plu_display( ex_atflash_PluNum );
                            unit_price_display(ex_atflash_PluWriteValue);

                            data_long = 0;
                            data_long2 = 0;
                        }                   
                        else if(sin_buffer3[1] == 'U' && sin_buffer3[2] == 'L') //unit price list read
                        {
                            data_long = 0;
                            if(sin_buffer3[3] >= 0x30 && sin_buffer3[3] <= 0x39)
                            {
                                data_long = (sin_buffer3[3] - 0x30) * 10;
                            }    
                            if(sin_buffer3[4] >= 0x30 && sin_buffer3[4] <= 0x39)
                            {
                                data_long = data_long + (sin_buffer3[4] - 0x30);
                            }


                            ex_atflash_PluNum = (unsigned char)data_long;
                            plu_display( ex_atflash_PluNum );

                            if(ex_atflash_PluNum<50)	
                                ex_atflash_PluReadValue=Atflash_PluData_Pop(PLU_U_P_HEAD, (ex_atflash_PluNum*4) );
                            else				
                                ex_atflash_PluReadValue=Atflash_PluData_Pop(PLU_U_P_TAIL, ( (ex_atflash_PluNum-50)*4) );

                            if(ex_atflash_PluReadValue > 99999) ex_atflash_PluReadValue = 0;
                            ex_atflash_PluWriteValue=ex_atflash_PluReadValue;
                            unit_price_display(ex_atflash_PluWriteValue);

                            Atflash_PluData_Push(PLU_CUR, 200, ex_atflash_PluNum);
                            data_long = 0;
                        }
                        break;


                    default :
                        break;


                }
                for(i=0; i < 15; i++) {sin_buffer3[i]=0;}
                Rx3_flg = 0;
            }

        }
        
        if(ep_wireless == 3)dst_ne_return(); //TF-200
    }
    else if(ep_wireless == 1)  // TMT wireless protocol(weight)
    {
        if(!ex_hold_finish) rs_com(ex_e_value);
        else rs_com(ex_weight_hold_value); 
    }
    else if(ep_wireless == 2) // TMT wireless protocol(price)
    {
        rs_com(cut_pay);
    }
}
/* 
 *data 0 : 18byte
 *data 1 : weight/unit/price
 *data 2 : price (point 0 fix)
 *data 4 : unit price
 *com 1 : zigbee, com 2 : debug, com 3 : PC, com 4 : bt
 *
 */ 
void com_send(unsigned char data, unsigned char com, unsigned long rs_weight, unsigned long pay)
{
    unsigned char buf[40], rs_lamp, v_division, rs, temp, count;
    
    count = 0;
    if(ex_weight_over_tick)
    {
        buf[count] = 'O'; count++;
        buf[count] = 'L'; count++;
    }
    else if(ex_hold_finish)  
    { 
        buf[count] = 'H'; count++;
        buf[count] = 'D'; count++;
    }
    else 
    {
        if(ex_stab_flag)  
        {
            buf[count] = 'S'; count++;
            buf[count] = 'T'; count++;
        }
        else            
        {
            buf[count] = 'U'; count++;
            buf[count] = 'S'; count++;
        }
    } 

    buf[count] = ','; count++;
    buf[count] = ' '; count++;

    if(ex_zero_flag) rs_lamp |= 0x04; // zero  
    if(ep_tare_flag) rs_lamp |= 0x02; // tera
    if(ex_hold_flag) rs_lamp |= 0x01; // hold
    if(ex_sign_flag) rs_lamp |= 0x08;
    if(ex_stab_flag) rs_lamp |= 0x10;

    v_division = ep_minimum_division & 0xF0;

    if(		v_division == 0x20)	rs_lamp |= 0x80;  // 1 com
    else if(v_division == 0x40) rs_lamp |= 0x40;  // 2 com
    else if(v_division == 0x60) rs_lamp |= 0x20;  // 3 com

    buf[count] = rs_lamp; count++;
    buf[count] = ',';  count++; // rs_lamp : 1com/2com/3com/stb/+-/zero/tare/hold

    if(ex_sign_flag) 	{buf[count] = '-'; count++;}
    else            	{buf[count] = '+'; count++;}

    if(data == 2)  
    {
        if(ex_sign_flag) pay = 0;

        pay = pay % 1000000;
        {
            for(rs=7; rs>0; rs--)        
            { 
                temp = (unsigned char)(pay /1000000);

                buf[count] = (temp + '0');
                count++;
                pay = pay % 1000000;
                pay = pay * 10;
            }
        }
        buf[count] = 0x20; count++;
        buf[count] = 0x20; count++;
    }
    else if(data == 4)
    {
        rs_weight = ex_atflash_PluWriteValue;
        for(rs=7; rs>0; rs--)        
        { 
            temp = (unsigned char)(rs_weight /1000000); 

            buf[count] = (temp + '0');
            count++;
            rs_weight = rs_weight % 1000000;
            rs_weight = rs_weight * 10;
        }
        buf[count] = 0x20; count++;
        buf[count] = 0x20; count++;
    }
    else
    {
        if(ex_hold_finish)rs_weight = ex_weight_hold_value;
        if(v_division && data != 2)
        {
            for(rs=7; rs>0; rs--)        
            { 
                if( 	 (rs==2)&&(v_division==0x20) )	{buf[count] = '.'; count++; }
                else if( (rs==3)&&(v_division==0x40) ) 	{buf[count] = '.'; count++; }
                else if( (rs==4)&&(v_division==0x60) ) 	{buf[count] = '.'; count++; }
                else 
                { 
                    temp = (unsigned char)(rs_weight /100000);

                    buf[count] = (temp + '0');
                    count++; 
                    rs_weight = rs_weight % 100000;
                    rs_weight = rs_weight * 10;
                }
            }
        }
        else
        {
            for(rs=7; rs>0; rs--)        
            { 
                temp = (unsigned char)(rs_weight /1000000); 

                buf[count] = (temp + '0');
                 count++; 
                rs_weight = rs_weight % 1000000;
                rs_weight = rs_weight * 10;
            }
        }  

        buf[count] = 'k'; count++;
        buf[count] = 'g'; count++;
    }
    if(data == 1)
    {
        buf[count] = ','; count++;
        rs_weight = ex_atflash_PluWriteValue;
        for(rs=5; rs>0; rs--)        
        { 
            temp = (unsigned char)(rs_weight /10000); 

            buf[count] = (temp + '0');
            count++;
            rs_weight = rs_weight % 10000;
            rs_weight = rs_weight * 10;
        }

        buf[count] = ',';
        count++;
        rs_weight = pay;
        rs_weight = rs_weight % 1000000;
        {
            for(rs=7; rs>0; rs--)        
            { 
                temp = (unsigned char)(rs_weight /1000000);

                buf[count] = (temp + '0');
                count++;
                rs_weight = rs_weight % 1000000;
                rs_weight = rs_weight * 10;
            }
        }

        buf[count] = 13; count++;
        buf[count] = 10; count++;
    }
    else
    {
        buf[count] = 13; count++;
        buf[count] = 10; count++;
    }

    if(com == 1)
    {
        //s_out1('!');
        for(temp=0; temp < count; temp++)
        {
            s_out1(buf[temp]);
        }
    }
    else if(com == 2)
    {
        for(temp=0; temp < count; temp++)
        {
            s_out2(buf[temp]);
        }
    }
    else if(com == 3)
    {
        for(temp=0; temp < count; temp++)
        {
            s_out3(buf[temp]);
        }
    }
    else if(com == 4)
    {
        for(temp=0; temp < count; temp++)
        {
            s_out4(buf[temp]);
        }
    }
}

void rs_com(unsigned long rs_weight)
{
  unsigned char rs_lamp=0, i, temp;

  s_out1(0x21);
  s_out1('S'); s_out1('T');
  rs_lamp = 0;
  if(ex_zero_flag) rs_lamp |= 0x04;
  if(ep_tare_flag) rs_lamp |= 0x02;
  if(ex_hold_flag) rs_lamp |= 0x01;
  if(ex_sign_flag) rs_lamp |= 0x08;
  if(ex_stab_flag) rs_lamp |= 0x10;
  if(ep_wireless != 2)
  {
    if( (ep_minimum_division&0x60)==0x60 )      rs_lamp |= 0x20;  // 3 com
    else if( (ep_minimum_division&0x40)==0x40 ) rs_lamp |= 0x40;  // 2 com
    else if( (ep_minimum_division&0x20)==0x20 ) rs_lamp |= 0x80;  // 1 com
  }
  s_out1(rs_lamp);
  for(i=5; i>0; i--)        
  { 
    temp = (unsigned char)(rs_weight /10000); 
    s_out1(temp + '0');
    rs_weight = rs_weight % 10000;
    rs_weight = rs_weight * 10;
  }
  s_out1(0x0D);
  s_out1(0x0A);
}

void com_print_form(unsigned char comm, unsigned long data, unsigned long data2)
{
    unsigned char buf[80], count, rs, temp, v_division, chk;
    unsigned long rs_weight;

    count = 0;

    for(temp=0; temp<24; temp++) {buf[count]= '='; count++;}
    buf[count]= 0x0D; count++;
    buf[count]= 0x0A; count++;
    
    buf[count]= 'U'; count++;
    buf[count]= 'n'; count++;
    buf[count]= 'i'; count++;
    buf[count]= 't'; count++;
    buf[count]= ' '; count++;
    buf[count]= 'P'; count++;
    buf[count]= 'r'; count++;
    buf[count]= 'i'; count++;
    buf[count]= 'c'; count++;
    buf[count]= 'e'; count++;
    buf[count]= ' '; count++;
    buf[count]= ':'; count++;
    buf[count]= ' '; count++;
    
    rs_weight = ex_atflash_PluWriteValue;
    chk = 0;
    for(rs=5; rs>0; rs--)        
    { 
        temp = (unsigned char)(rs_weight /10000); 

        buf[count] = (temp + '0');
        if(buf[count]  == '0' && !chk) 
        {
            buf[count] = ' ';
        }
        else chk=1;
        count++;
        rs_weight = rs_weight % 10000;
        rs_weight = rs_weight * 10;
    }
    buf[count]= ' '; count++;
    buf[count]= '/'; count++;
    buf[count]= ' '; count++;
    if(!ep_plu_unit)
    {
        buf[count]= '1'; count++;
        buf[count]= '0'; count++;
        buf[count]= '0'; count++;
        buf[count]= 'g'; count++;
    }
    else
    {
        buf[count]= '1'; count++;
        buf[count]= 'k'; count++;
        buf[count]= 'g'; count++;
    }
    buf[count]= 0x0D; count++;
    buf[count]= 0x0A; count++;
   
    buf[count]= 'W'; count++;
    buf[count]= 'e'; count++;
    buf[count]= 'i'; count++;
    buf[count]= 'g'; count++;
    buf[count]= 'h'; count++;
    buf[count]= 't'; count++;
    buf[count]= ' '; count++;
    buf[count]= ':'; count++;
    buf[count]= ' '; count++;
   
    rs_weight = data;
    
    v_division = ep_minimum_division & 0xF0;
    
    chk = 0;
    if(v_division)
    {
        for(rs=7; rs>0; rs--)        
        { 
            if((rs==2)&&(v_division==0x20) )	
            {
                if(buf[count-1]  == ' ') {buf[count-1] = '0';}
                buf[count] = '.'; count++;  
            }
            else if( (rs==3)&&(v_division==0x40) )
            {
                if(buf[count-1]  == ' ') {buf[count-1] = '0';}
                buf[count] = '.'; count++;  
            }
            else if( (rs==4)&&(v_division==0x60) )
            {
                if(buf[count-1]  == ' ') {buf[count-1] = '0';}
                buf[count] = '.'; count++;  
            }
            else 
            { 
                temp = (unsigned char)(rs_weight /100000);

                buf[count] = (temp + '0');
                if(buf[count]  == '0' && !chk) 
                {
                    buf[count] = ' ';
                }
                else chk=1;
                
                count++; 
                rs_weight = rs_weight % 100000;
                rs_weight = rs_weight * 10;
            }
        }
    }
    else
    {
        for(rs=7; rs>0; rs--)        
        { 
            temp = (unsigned char)(rs_weight /1000000); 

            buf[count] = (temp + '0');
            if(buf[count]  == '0' && !chk) 
            {
                buf[count] = ' ';
            }
            else chk=1;
             count++; 
            rs_weight = rs_weight % 1000000;
            rs_weight = rs_weight * 10;
        }
    }  

    buf[count] = 'k'; count++;
    buf[count] = 'g'; count++;
  
    buf[count]= 0x0D; count++;
    buf[count]= 0x0A; count++;
    
    buf[count]= 'P'; count++;
    buf[count]= 'r'; count++;
    buf[count]= 'i'; count++;
    buf[count]= 'c'; count++;
    buf[count]= 'e'; count++;
    buf[count]= ' '; count++;
    buf[count]= ':'; count++;
    buf[count]= ' '; count++;
  
    rs_weight = data2 % 1000000;
    {
        chk = 0;
        for(rs=7; rs>0; rs--)        
        { 
            temp = (unsigned char)(rs_weight /1000000);

            buf[count] = (temp + '0');
            if(buf[count]  == '0' && !chk) 
            {
                buf[count] = ' ';
            }
            else chk=1;
            count++;
            rs_weight = rs_weight % 1000000;
            rs_weight = rs_weight * 10;
        }
    }
    buf[count]= 0x0D; count++;
    buf[count]= 0x0A; count++;
    if(comm == 3)
    {
        for(temp=0; temp < count; temp++)
        {
            s_out3(buf[temp]);
        }
        for(temp=0; temp < ep_feed; temp++)  //F09 line feed
        {
            s_out3(0x0A);
        }
    }
    else if(comm == 1)
    {
        for(temp=0; temp < count; temp++)
        {
            s_out1(buf[temp]);
        }
        for(temp=0; temp < ep_feed; temp++)  //F09 line feed
        {
            s_out1(0x0A);
        }
    }
    else if(comm == 4)
    {
        for(temp=0; temp < count; temp++)
        {
            s_out4(buf[temp]);
        }
        for(temp=0; temp < ep_feed; temp++)  //F09 line feed
        {
            s_out4(0x0A);
        }
    }
}


void buffer1_initial(void)
{
	unsigned char i;


	for(i=0; i<21; i++)
	{
		sin_buffer1[i]=0;
	}
	
	v_rs_flag1=0; 
	sin_count1=0;
}

void s_out1(unsigned char bData)
{
	while(U1STAbits.UTXBF);
	U1TXREG = bData;
}

void s_out2(unsigned char bData)
{
	while(U2STAbits.UTXBF);
	U2TXREG = bData;
}

void s_out3(unsigned char bData)
{
	while(U3STAbits.UTXBF);
	U3TXREG = bData;
}

void s_out4(unsigned char bData)
{
	while(U4STAbits.UTXBF);
	U4TXREG = bData;
}


//******************************************************************************************************/
void dst_slave(void)
{
	unsigned char low, high;


	low = 0x03;
	high = 0x40;
	
	DelayMs(40);
	
	s_out1(0x13);
	s_out1(low); 
	s_out1(high); 
	s_out1(0x13+low+high);  // write
	
	DelayMs(40);
	
	s_out1(0x17);
	s_out1(0x00); 
	s_out1(0x00); 
	s_out1(0x17); // Zigbee retry 9, echo on
	
	DelayMs(40);
	
	s_out1(0x21);
	
	v_echo = 1;
}

//******************************************************************************************************/
void dst_return(void)
{
	unsigned char low, high;


	low = 0x01;
	high = 0x40;

	DelayMs(40);

	s_out1(0x13);
	s_out1(low);
	s_out1(high);
	s_out1((unsigned char)(0x13+low+high));  // write

	DelayMs(40);

	s_out1(0x18);
	s_out1(0x00);
	s_out1(0x00);
	s_out1(0x18); // Zigbee retry off, echo off

	DelayMs(40);

	v_echo = 0;
}

//******************************************************************************************************/
void dst_ne_slave(void)
{
	unsigned char low, high;


	low = 0x03;
	high = 0x40;
	
	DelayMs(40);
	
	s_out1(0x13);
	s_out1(low);
	s_out1(high);
	s_out1((unsigned char)(0x13+low+high));  // write
	
	DelayMs(40);
	
	s_out1(0x21);
	
	v_echo = 0;
}

//******************************************************************************************************/
void dst_ne_return(void)
{
	unsigned char low, high;


	low = 0x01;
	high = 0x40;

	DelayMs(40);
	
	s_out1(0x13);
	s_out1(low);
	s_out1(high);
	s_out1((unsigned char)(0x13+low+high));  // write
	
	DelayMs(40);
	
	v_echo = 0;
}


void tf200_send2(unsigned long rs_weight) // 18 byte (and format)
{
	unsigned char temp, rs, rs_lamp=0, i, v_division;
	unsigned long print_total_money_tmp;
	unsigned long ex_atflash_PluWriteValue_tmp=ex_atflash_PluWriteValue;
	unsigned char print_count_tmp=ex_atflash_PluNum; 

	print_total_money_tmp=(ex_atflash_PluWriteValue_tmp*rs_weight*10);
	dst_ne_slave();



	s_out1(13);
	s_out1(10); 
	

	s_out1('W'); 
	s_out1('E'); 
	s_out1('I');
	s_out1('G');	
	s_out1('H'); 
	s_out1('T');
	s_out1(' ');
	s_out1(':');
	s_out1(' ');
	for(rs=5; rs>0; rs--)        
	{ 
		temp = (unsigned char)(rs_weight /10000); 

		s_out1(temp + '0');

		rs_weight = rs_weight % 10000;
		rs_weight = rs_weight * 10;
	}
	s_out1(' ');
	s_out1('k');
	s_out1('g');
	s_out1(13);
	s_out1(10); 
	///////////////////////////////////////////////////
	s_out1('U'); 
	s_out1('N'); 
	s_out1('I');
	s_out1('T');
	s_out1(' ');
	s_out1(' ');
	s_out1(' ');
	s_out1(':');
	s_out1(' ');

	
	for(rs=5; rs>0; rs--)        
	{ 
		temp = (unsigned char)(ex_atflash_PluWriteValue_tmp /10000); 

		s_out1(temp + '0');

		ex_atflash_PluWriteValue_tmp = ex_atflash_PluWriteValue_tmp % 10000;
		ex_atflash_PluWriteValue_tmp = ex_atflash_PluWriteValue_tmp * 10;
	}
	s_out1(' ');
	s_out1('W');
	s_out1('o');
	s_out1('n');

	s_out1(13);
	s_out1(10); 

	for(i=0; i<13; i++)		s_out1(' '); 

	s_out1('/');
	s_out1('1');
	s_out1('0');
	s_out1('0');
	s_out1('g');


	s_out1(13);
	s_out1(10); 
	///////////////////////////////////////////////////
	s_out1('T'); 
	s_out1('O'); 
	s_out1('T');
	s_out1('A');	
	s_out1('L'); 
	s_out1(' ');
	#if 0
	s_out1(' ');
	s_out1('P');
	s_out1('R');
	s_out1('I');
	s_out1('C');
	s_out1('E');
	#endif 

	s_out1(' ');
	s_out1(':');
	s_out1(' ');
	for(rs=6; rs>0; rs--)        
	{ 
		temp = (unsigned char)(print_total_money_tmp /100000); 

		s_out1(temp + '0');

		print_total_money_tmp = print_total_money_tmp % 100000;
		print_total_money_tmp = print_total_money_tmp * 10;
	}
	s_out1(' ');
	s_out1('W');
	s_out1('o');
	s_out1('n');
	{s_out1(13);
	s_out1(10); }
	for(i=0; i<20; i++)		s_out1('-'); 

	for(i=0; i<ep_feed; i++)	
	{s_out1(13);
	s_out1(10); }
	///////////////////////////////////////////////////


	


	
	dst_ne_return();
	
	buffer1_initial();
}
/*
	Total Weight:	kg
	Money		:	won
*/
void tf200_send(unsigned long rs_weight) // 18 byte (and format)
{
	unsigned char temp, rs, rs_lamp=0, i, v_division;
	unsigned long print_total_money_tmp=print_total_money;
	unsigned long ex_atflash_PluWriteValue_tmp=ex_atflash_PluWriteValue;
	unsigned short print_count_tmp=print_count; 

	//print_total_money_tmp=(ex_atflash_PluWriteValue*(unsigned long)print_count_tmp);
	print_total_money_tmp*=10;
	dst_ne_slave();


#if 0
	if(ex_hold_finish)  
	{ 
		s_out1('S'); 
		s_out1('T');  
	}
	else 
	{
		if(ex_stab_flag)  
		{
			s_out1('S');
			s_out1('T'); 
		}
		else            
		{
			s_out1('U');
			s_out1('S');
		}
	} 
	
	s_out1(','); 
	s_out1(' ');

	if(ex_zero_flag) rs_lamp |= 0x04; // zero  
	if(ep_tare_flag) rs_lamp |= 0x02; // tera
	if(ex_hold_flag) rs_lamp |= 0x01; // hold
	if(ex_sign_flag) rs_lamp |= 0x08;
	if(ex_stab_flag) rs_lamp |= 0x10;

	v_division = ep_minimum_division & 0xF0;

	if(		v_division == 0x20)	rs_lamp |= 0x80;  // 1 com
	else if(v_division == 0x40) rs_lamp |= 0x40;  // 2 com
	else if(v_division == 0x60) rs_lamp |= 0x20;  // 3 com

	s_out1(rs_lamp);
	s_out1(','); // rs_lamp : 1com/2com/3com/stb/+-/zero/tare/hold

	if(ex_sign_flag) 	s_out1('-');
	else            	s_out1('+');

	if(v_division)
	{
		for(rs=7; rs>0; rs--)        
		{ 
			if( 	 (rs==2)&&(v_division==0x20) )	s_out1('.');
			else if( (rs==3)&&(v_division==0x40) ) 	s_out1('.');
			else if( (rs==4)&&(v_division==0x60) ) 	s_out1('.');
			else 
			{ 
				temp = (unsigned char)(rs_weight /100000);

				s_out1(temp + '0');
				
				rs_weight = rs_weight % 100000;
				rs_weight = rs_weight * 10;
			}
		}
	}
	else
	{
		for(rs=7; rs>0; rs--)        
		{ 
			temp = (unsigned char)(rs_weight /1000000); 

			s_out1(temp + '0');

			rs_weight = rs_weight % 1000000;
			rs_weight = rs_weight * 10;
		}
	}  
	
	s_out1('k');
	s_out1('g');

	s_out1(13);
	s_out1(10); 
	#endif 

	//////////////////////////////////////////////////
	//s_out1(13);
	//s_out1(10); 
	//for(i=0; i<24; i++)	s_out1('-');
	s_out1(13);
	s_out1(10); 
	s_out1('W'); 
	s_out1('E'); 
	s_out1('I');
	s_out1('G');	
	s_out1('H'); 
	s_out1('T');
	s_out1(' ');
	s_out1(':');
	s_out1(' ');
	for(rs=5; rs>0; rs--)        
	{ 
		temp = (unsigned char)(rs_weight /10000); 

		s_out1(temp + '0');

		rs_weight = rs_weight % 10000;
		rs_weight = rs_weight * 10;
	}
	s_out1(' ');
	s_out1('k');
	s_out1('g');
	s_out1(13);
	s_out1(10); 
	///////////////////////////////////////////////////
	//for(i=0; i<24; i++)	s_out1('-');
	s_out1('U'); 
	s_out1('N'); 
	s_out1('I');
	s_out1('T');
	s_out1(' ');
	s_out1(' ');
	#if 0
	s_out1(' '); 
	s_out1('P');
	s_out1('R');
	s_out1('I');
	s_out1('C');
	s_out1('E');
	#endif 
	s_out1(' ');
	s_out1(':');
	s_out1(' ');

	
	for(rs=5; rs>0; rs--)        
	{ 
		temp = (unsigned char)(ex_atflash_PluWriteValue_tmp /10000); 

		s_out1(temp + '0');

		ex_atflash_PluWriteValue_tmp = ex_atflash_PluWriteValue_tmp % 10000;
		ex_atflash_PluWriteValue_tmp = ex_atflash_PluWriteValue_tmp * 10;
	}
	s_out1(' ');
	s_out1('W');
	s_out1('o');
	s_out1('n');
	s_out1(13);
	s_out1(10); 

	for(i=0; i<13; i++)		s_out1(' '); 

	s_out1('/');
	s_out1('1');
	s_out1('0');
	s_out1('0');
	s_out1('g');


	s_out1(13);
	s_out1(10); 
	///////////////////////////////////////////////////
	//for(i=0; i<24; i++)	s_out1('-');
	s_out1('C'); 
	s_out1('O'); 
	s_out1('U');
	s_out1('N');	
	s_out1('T'); 
	s_out1(' ');
	s_out1(' ');
	s_out1(':');
	s_out1(' ');
	for(rs=3; rs>0; rs--)        
	{ 
		temp = (unsigned char)(print_count_tmp /100); 

		s_out1(temp + '0');

		print_count_tmp = print_count_tmp % 100;
		print_count_tmp = print_count_tmp * 10;
	}
	s_out1(' ');
	s_out1('E');
	s_out1('A');
	s_out1(13);
	s_out1(10); 
	///////////////////////////////////////////////////
	s_out1('T'); 
	s_out1('O'); 
	s_out1('T');
	s_out1('A');	
	s_out1('L'); 
	s_out1(' ');
	#if 0
	s_out1(' ');
	s_out1('P');
	s_out1('R');
	s_out1('I');
	s_out1('C');
	s_out1('E');
	#endif 

	s_out1(' ');
	s_out1(':');
	s_out1(' ');
	for(rs=6; rs>0; rs--)        
	{ 
		temp = (unsigned char)(print_total_money_tmp /100000); 

		s_out1(temp + '0');

		print_total_money_tmp = print_total_money_tmp % 100000;
		print_total_money_tmp = print_total_money_tmp * 10;
	}
	s_out1(' ');
	s_out1('W');
	s_out1('o');
	s_out1('n');
	s_out1(13);
	s_out1(10); 
	for(i=0; i<20; i++)		s_out1('-'); 

	for(i=0; i<ep_feed; i++)	
	{s_out1(13);
	s_out1(10); }
	///////////////////////////////////////////////////


	


	
	dst_ne_return();
	
	buffer1_initial();
}




//******************************************************************************************************/
void netid_mode(void)
{
  	unsigned char 	i;
	unsigned char	low;
	unsigned char	high;
	unsigned char 	sum;
	unsigned char	x_key=0;
	unsigned char	key_cnt=0;
	unsigned char	netid_mode_exit_f=0;


	buffer1_initial();
  	String_Display(PAY, " RFSET"); 

  
  	if( ep_wireless )
  	{
	  	String_Display(PAY, " RETRY"); 

	   	while(v_rs_flag1==0)
	   	{
      		s_out1(0x1F); 
			s_out1(0x12); 
			s_out1(0x00); 
			s_out1(0x1F+0x12);  // READ COMMAND	
			
      		DelayMs(40);

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

	  	String_Display(PAY, " RFSET"); 

		

    	low = sin_buffer1[5];
    	high = sin_buffer1[6];
    	sum = sin_buffer1[7];
		
    	buffer1_initial();
		
    	DelayMs(40);
  
    	s_out1(0x12);
		s_out1(0x00);
		s_out1(0x00);
		s_out1(0x12);  // write
		
    	DelayMs(40);

		while(netid_mode_exit_f==0)
		{
			if((tbi(key_status, key_detect_f))||(tbi(ir_key_status, ir_key_detect_f)))
			{
				x_key=Key_Data_Read();
				switch(x_key)
				{
					case ENT_K:
						tone_type=SHORT_TONE_PLAY;
						
						// id send		  
						s_out1(0x21);
						s_out1('S'); 
						s_out1('T');
						s_out1(low); 
						s_out1(high); 
						s_out1(sum);
						
						for(i=0; i<3; i++)
						{
							s_out1('0');
						}
						
						s_out1(0x0D); 
						s_out1(0x0A); 
						
						DelayMs(40);
						
						s_out1(0x12); 
						s_out1(low); 
						s_out1(high);
						s_out1((unsigned char)(0x12+low+high));  // write
						
						DelayMs(40);
						netid_mode_exit_f=1;
						break;

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
	
  	String_Display(PAY, "   END"); 
	DelayMs(700);
}

void print(unsigned long rs_weight)
{
	unsigned char temp, rs, i, first_number_zero_f=0;
	unsigned long ex_atflash_PluWriteValue_tmp=ex_atflash_PluWriteValue;	// 단위당 가격
	unsigned long print_total_money_tmp;					// 총 가격
	

	print_flag = 0;
    
    print_total_money_tmp = (ex_atflash_PluWriteValue_tmp*rs_weight);//*10

	for(i=0; i<24; i++)		s_out3('-');	// /
	s_out3(0x0D); s_out3(0X0A);	// 줄 변경
	
	s_out3(0xB9);	// 무 
	s_out3(0xAB);	//  
	s_out3(0xB0);	// 게  
	s_out3(0xD4);	// 
	s_out3(' ');	// sp
	s_out3(':');	// :
	s_out3(' ');	// sp
	for(rs=5; rs>0; rs--)       // 무게
	{ 
		temp = (unsigned char)(rs_weight /10000);

		if(ep_maximum_capacity>50)		// CAPA 60kg, 150kg		표시 자릿수가 다름,  .위치선택을 위해 2가지로 출력
			{	
				if(rs==3)	first_number_zero_f=1;
				if(first_number_zero_f==0)	
					{
						if(temp==0)		s_out3(0x00);								
						else			
							{
								s_out3(temp + '0');
								if(rs==3)	s_out3('.');
								first_number_zero_f=1;
							}
					}
				else	
					{
						s_out3(temp + '0');
						if(rs==3)	s_out3('.');
					}
			}
		else							// CAPA 15kg, 30kg
			{	
				if(rs==4)	first_number_zero_f=1;
				if(first_number_zero_f==0)	
					{
						if(temp==0)		s_out3(0x00);								
						else			
							{
								s_out3(temp + '0');
								if(rs==4)	s_out3('.');
								first_number_zero_f=1;
							}
					}
				else	
					{
						s_out3(temp + '0');
						if(rs==4)	s_out3('.');
					}
			}

		rs_weight = rs_weight % 10000;
		rs_weight = rs_weight * 10;
				
	}
	s_out3(' ');	// sp
	s_out3('k');	// k
	s_out3('g');	// g
	s_out3(0x0D); s_out3(0X0A);	// 줄 변경

	first_number_zero_f=0;
/**************************************************************************************/
	s_out3(0xB4);	// 단
	s_out3(0xDC);	// 
	s_out3(0xB0);	// 가 
	s_out3(0xA1);	//  
	s_out3(' ');	// sp
	s_out3(':');	// :
	s_out3(' ');	// sp
	for(rs=5; rs>0; rs--)		// 단가당 가격
	{ 
		temp = (unsigned char)(ex_atflash_PluWriteValue_tmp /10000); 

		if(first_number_zero_f==0)	
			{
				if(temp==0)		s_out3(0x00);								
				else			
					{
						s_out3(temp + '0');
						if(rs==4)	s_out3(',');
						first_number_zero_f=1;
					}
			}
		else	
			{
				s_out3(temp + '0');
				if(rs==4)	s_out3(',');
			}

		ex_atflash_PluWriteValue_tmp = ex_atflash_PluWriteValue_tmp % 10000;
		ex_atflash_PluWriteValue_tmp = ex_atflash_PluWriteValue_tmp * 10;
	}
	s_out3(0XBF);	// 원 
	s_out3(0XF8);	//
	s_out3(' ');	// Sp
	s_out3('/');	// /
	s_out3(' ');	// SP

	if(ep_plu_unit==0)
		{
			s_out3('1');	//
			s_out3('0');	//
			s_out3('0');	//
			s_out3('g');	//
		}
	else
		{
			s_out3('1');	//
			s_out3('k');	//
			s_out3('g');	//
		}
	s_out3(0x0D); s_out3(0X0A);	// 줄 변경

	first_number_zero_f=0;
/**************************************************************************************/
	s_out3(0xB0);	// 가
	s_out3(0xA1);	// 
	s_out3(0xB0);	// 격 
	s_out3(0xDD);	//  
	s_out3(' ');	// sp
	s_out3(':');	// :
	s_out3(' ');	// sp
	for(rs=6; rs>0; rs--)		// 총 가격 
	{ 
		temp = (unsigned char)(print_total_money_tmp /100000); 

		if(first_number_zero_f==0)	
			{
				if(temp==0)		s_out3(0x00);								
				else			
					{
						s_out3(temp + '0');
						if(rs==4)	s_out3(',');
						first_number_zero_f=1;
					}
			}
		else	
			{
				s_out3(temp + '0');
				if(rs==4)	s_out3(',');
			}		
		
		print_total_money_tmp = print_total_money_tmp % 100000;
		print_total_money_tmp = print_total_money_tmp * 10;
	}
	s_out3(0XBF);	// 원 
	s_out3(0XF8);	//
	s_out3(0x0D); s_out3(0X0A);
	
	for(i=0; i<24; i++)		s_out3('-');	// /
	
	for(i=0; i<ep_feed; i++)		// 피드 횟수
	{s_out3(0x0D); s_out3(0X0A);}
	
	DelayMs(40);
	
	

}
