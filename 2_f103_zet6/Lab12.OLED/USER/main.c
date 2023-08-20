#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "oled.h"
 
/************************************************
 ALIENTEK��ӢSTM32������ʵ��12
 OLED��ʾʵ��  
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
	
 int main(void) {	
	u8 t;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 	LED_Init();			     //LED�˿ڳ�ʼ��
	OLED_Init();			//��ʼ��OLED      
	OLED_ShowString(0,0,"ALIENTEK",24);  
	OLED_ShowString(0,24, "0.96' OLED TEST",16);  
 	OLED_ShowString(0,40,"ATOM 2015/1/14",12);  
 	OLED_ShowString(0,52,"ASCII:",12);  
 	OLED_ShowString(64,52,"CODE:",12);  
  
	OLED_Refresh_Gram();		//������ʾ��OLED 
	t=' ';  
	while(1) {		
		OLED_ShowChar(48,48,t,16,1);//��ʾASCII�ַ�	   
		OLED_Refresh_Gram();
		t++;
		if(t>'~') {
			t=' ';
		}
		OLED_ShowNum(103,48,t,3,16);//��ʾASCII�ַ�����ֵ 
		delay_ms(500);
		LED0 = !LED0;
	}	  
	
}

