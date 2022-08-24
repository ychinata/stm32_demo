#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "tpad.h"

/************************************************
 ALIENTEK��ӢSTM32������ʵ��11
 ���ݴ�������ʵ�� 
 Func:
 LED0һֱ��˸������������
 ���񵽣���LED1��˸
 2022.3.15
************************************************/
 
 int main(void) {	
 	u8 t=0;  	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	TPAD_Init(6);			//��ʼ����������
   	while(1) {				
		//�ɹ�������һ��������(�˺���ִ��ʱ������15ms)
 		if (TPAD_Scan(0)) { // ��֧������
			LED1 =! LED1;		//LED1ȡ��
		}
		t++;
		if (t==15) {
			t=0;
			LED0=!LED0;		//LED0ȡ��,��ʾ������������
		}
		delay_ms(10);
	}
 }

