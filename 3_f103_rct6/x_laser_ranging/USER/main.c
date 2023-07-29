#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"  
#include "24cxx.h" 
#include "myiic.h"
#include "usmart.h"
#include "vl53l0x.h"

/************************************************
 ALIENTEK MiniV3 STM32��������չʵ��21
 ATK-VL53L0X������ģ�� �ۺϲ���ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
   	
 int main(void)
 { 
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����2
	delay_init();	                               //��ʱ������ʼ��	  
	uart_init(115200);	                           //���ڳ�ʼ��Ϊ115200
	usmart_dev.init(72);                           //��ʼ��USMART	
	LED_Init();		  	                           //��ʼ����LED���ӵ�Ӳ���ӿ�
	//LCD_Init();	                                   //LCD��ʼ��
	KEY_Init();			                           //������ʼ��		 	
	//AT24CXX_Init();		                           //IIC��ʼ�� 

	while(AT24CXX_Check())//��ⲻ��24c02
	{
		LCD_ShowString(30,150,200,16,16,"24C02 Check Failed!");
		delay_ms(500);
		LCD_ShowString(30,150,200,16,16,"Please Check!      ");
		delay_ms(500);
		LED0=!LED0;//DS0��˸
	}
	while(1)
	{
		vl53l0x_test();//vl53l0x����
	}
}
