#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
 
/************************************************
Date: 2022.3.8
Author: h00421956
Func:  PWM���ʵ�� 
		led0���˺����Ƶ�Ч����Ϊʲô��led0����Ϊ
		�� TIM3_CH2 ֱ��ӳ�䵽�� PB5 �ϣ�PB5������led0
		��ʵ�鲻��Ҫʹ���ж�
************************************************/

 int main(void)
 {		
 	u16 led0pwmval=0;
	u8 dir=1;	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
 	TIM3_PWM_Init(899,0);	 //����Ƶ��PWMƵ��=72000000/900=80Khz
   	while(1) {
 		delay_ms(10);	 
		if (dir) {
			led0pwmval++;
		} else {
			led0pwmval--;
		}

 		if (led0pwmval>300) {
			dir=0; // ��ת�������䰵
		}
		if (led0pwmval==0) {
			dir=1; // ��ת���ɰ�����
		}			
		TIM_SetCompare2(TIM3,led0pwmval);		   
	}	 
 }

