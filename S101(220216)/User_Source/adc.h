#ifndef _ADC_H_
#define _ADC_H_


void adc_initial(void);
void test_adc_initial(void);
void adc_loop(unsigned char);
unsigned long read_filtered_adc(void);

void Single_Calibration_Mode(void);
void cal_err(void);
void zero_span_set(unsigned char);
void Weight_Task(void);
void Weight_Init(void);
void MCU_ADC_INIT(void);
void Low_Batt_check(void);

long data_avg(long data, char init);
void AdcDataInit(void);



#endif 


