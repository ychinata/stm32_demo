#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "hc-sr04.h"
#include "oled.h"
#include "led.h"
#include "sys.h"

u32 Distance;                               //���������


int main(void)
{
	delay_init();	    //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�     
	OLED_Init();
    HCSR04_Init(0XFFFF, 72-1);  // ���������ģ��
    
	
    OLED_ShowString(OLED_LINE1, 1, "HC-SR04 Demo");
    OLED_ShowString(OLED_LINE2, 5, "(mm)");
    OLED_ShowString(OLED_LINE4, 1, "2024.6.9");    
	// OLED_ShowString(1, 3, "HelloWorld!");
	// OLED_ShowNum(2, 1, 12345, 5);
	// OLED_ShowSignedNum(2, 7, -66, 2);
	// OLED_ShowHexNum(3, 1, 0xAA55, 4);
	// OLED_ShowBinNum(4, 1, 0xAA55, 16);
	
	while(1) {
		LED0=0;
		LED1=1;
        
        HCSR04_GetDistane();
        OLED_ShowNum(OLED_LINE2, 1, Distance, 4);
        
        //OLED_ShowString(OLED_LINE2, 1, "123456--");  
		//delay_ms(300);	 //��ʱ300ms
        
		//LED0=1;
		//LED1=0;
        //OLED_ShowString(2, 1, "->abcdef");
		delay_ms(300);	//��ʱ300ms
	}
    
}
