#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"


/************************************************
Date: 2022.3.5
Author: h00421956
Func:  ��ʱ���ж�ʵ��

led0 200ms��˸һ��
led1 500ms��˸һ��
************************************************/
	
int main(void) {		
	delay_init();	    	 	//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 		//���ڳ�ʼ��Ϊ115200
 	LED_Init();			      	//LED�˿ڳ�ʼ��
	TIM3_Int_Init(4999,7199); 	//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  
   	while (1) {
		LED0 = !LED0;
		delay_ms(200);		   
	}	 
}	 

