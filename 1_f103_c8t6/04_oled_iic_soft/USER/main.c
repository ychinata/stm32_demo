#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "oled_iic_soft.h"
#include "led.h"
#include "sys.h"

int main(void)
{
	delay_init();	    //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�     
	OLED_Init();	
    //test
    //Draw_DATA(0, 0, 10);
    OLED_Test();
	
	while(1) {

	}
    
}
