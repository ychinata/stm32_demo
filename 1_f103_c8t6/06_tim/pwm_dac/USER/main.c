#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "rtc.h" 
#include "wkup.h"
#include "adc.h"	 
#include "dac.h"
#include "timer.h"
#include "usmart.h"

 
uint8_t i; 
 
/************************************************
 ALIENTEKս��STM32������ʵ��21
 PWM DAC ʵ��  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//���������ѹ
//vol:0~330,����0~3.3V
void PWM_DAC_Set(u16 vol)
{
	float temp=vol;
	temp/=100;
	temp=temp*256/3.3;
	TIM_SetCompare1(TIM1,temp);
}

 int main1(void)
 {	 
	u16 adcx;
	float temp;
 	u8 t=0;	 
	u16 pwmval=0;
	u8 key;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	KEY_Init();				  //KEY��ʼ��
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();			 	 //LCD��ʼ��
    Adc_Init();		  		//ADC��ʼ��
	TIM1_PWM_Init(255,0);	//TIM1 PWM��ʼ��, 8λ,Fpwm=72M/256=281.25Khz.
    TIM_SetCompare1(TIM1,100);//��ʼֵΪ0	
	     

    //��ʾ��ʾ��Ϣ	
	LCD_ShowString(60,70,200,16,16,"PWM DAC TEST");											     
	LCD_ShowString(60,150,200,16,16,"PWM VAL:");	      
	LCD_ShowString(60,170,200,16,16,"DAC VOL:0.000V");	      
	LCD_ShowString(60,190,200,16,16,"ADC VOL:0.000V");
	
	TIM_SetCompare1(TIM1,pwmval);//��ʼֵ	    	      
	while(1) {
		t++;
		key=KEY_Scan(0);			  
		if(key==WKUP_PRES) {		 
			if(pwmval<250)pwmval+=10;
			TIM_SetCompare1(TIM1,pwmval); 		//���
		}else if(key==KEY1_PRES){
			if(pwmval>10)pwmval-=10;
			else pwmval=0;
			TIM_SetCompare1(TIM1,pwmval); 		//���
		}	 
		if(t==10||key==KEY1_PRES||key==WKUP_PRES) { 		//WKUP/KEY1������,���߶�ʱʱ�䵽��		
			adcx=TIM_GetCapture1(TIM1);
			LCD_ShowxNum(124,150,adcx,4,16,0);     	//��ʾDAC�Ĵ���ֵ
			temp=(float)adcx*(3.3/256);				//�õ�DAC��ѹֵ
			adcx=temp;
 			LCD_ShowxNum(124,170,temp,1,16,0);     	//��ʾ��ѹֵ��������
 			temp-=adcx;
			temp*=1000;
			LCD_ShowxNum(140,170,temp,3,16,0x80); 	//��ʾ��ѹֵ��С������
 			adcx=Get_Adc_Average(ADC_Channel_1,20);  		//�õ�ADCת��ֵ	  
			temp=(float)adcx*(3.3/4096);			//�õ�ADC��ѹֵ
			adcx=temp;
 			LCD_ShowxNum(124,190,temp,1,16,0);     	//��ʾ��ѹֵ��������
 			temp-=adcx;
			temp*=1000;
			LCD_ShowxNum(140,190,temp,3,16,0x80); 	//��ʾ��ѹֵ��С������
			t=0;
			LED0=!LED0;	   
		}	    
		delay_ms(10);	

	}
 }

