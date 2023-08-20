#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "wdg.h"

 
/************************************************
 ALIENTEK��ӢSTM32������ʵ��7
 ���ڿ��Ź�ʵ�� 
 ���ߣ�����ԭ�� @ALIENTEK
 Func������ι��ʱ��led1��ͣ��˸��LED0��һ�¾���
		û����ι����led0�ͻ���˸
************************************************/

 int main(void)
 {		
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();
	KEY_Init();          //������ʼ��	 
	LED0 = 0; // ��
	delay_ms(300);	  
	WWDG_Init(0X7F,0X5F,WWDG_Prescaler_8);//������ֵΪ7f,���ڼĴ���Ϊ5f,��Ƶ��Ϊ8	   
 	while (1) {
		 LED0 = 1;	// ��,���һֱ��λ��led0�ͻ���-��		  	   
	}   
}
  

