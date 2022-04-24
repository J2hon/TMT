#ifndef _RTC_H 
#define _RTC_H 
 
void RTCCUnlock(void);
void RTCCInit(void);
void rtc_read(void);
void rtc_set(void);
void clock_disp(unsigned char);
unsigned char bcd_to_decimal(unsigned char);
unsigned char decimal_to_bcd(unsigned char);
void rtc_test(void);
void clock_epr_set(void);



#endif
