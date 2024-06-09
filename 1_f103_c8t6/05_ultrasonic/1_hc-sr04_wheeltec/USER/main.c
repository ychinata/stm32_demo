#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "hc-sr04.h"
#include "oled.h"
#include "led.h"
#include "sys.h"

//2024.6.9˵�������ڽ�Э�Ƽ�STM32-OLED�����޸ģ�����my_gpio.h

int main(void)
{
    u32 distance_mm = 0xFFFFFFFF;
    //ģ���ʼ��
	delay_init();	    //��ʱ������ʼ��	  
	//LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�     
	OLED_Init();
    HCSR04_Init(0XFFFF, 72-1);  // ���������ģ��
    
	// ��ʼ����ʾ
    OLED_ShowString(OLED_LINE1, 1, "HC-SR04 Demo");
    OLED_ShowString(OLED_LINE2, 5, "(mm)");
    OLED_ShowString(OLED_LINE3, 1, "TrigPB1,EchoPB0");
    OLED_ShowString(OLED_LINE4, 1, "2024.6.9");    
	
	while(1) {        
        distance_mm = HCSR04_GetDistane();          // ��ȡ������ģ������ľ���
        OLED_ShowNum(OLED_LINE2, 1, distance_mm, 4);// ��OLED��ʾ����
        
		delay_ms(300);	//��ʱ300ms
	}
    
}
