#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "hc-sr04.h"
#include "oled.h"
#include "led.h"
#include "sys.h"

u32 Distance;                               //超声波测距


int main(void)
{
	delay_init();	    //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口     
	OLED_Init();
    HCSR04_Init(0XFFFF, 72-1);  // 超声波测距模块
    
	
	OLED_ShowChar(1, 1, 'A');
    OLED_ShowString(1, 1, "Hello,4Pin OLED!");
    OLED_ShowString(3, 1, "2022.5.1");    
	// OLED_ShowString(1, 3, "HelloWorld!");
	// OLED_ShowNum(2, 1, 12345, 5);
	// OLED_ShowSignedNum(2, 7, -66, 2);
	// OLED_ShowHexNum(3, 1, 0xAA55, 4);
	// OLED_ShowBinNum(4, 1, 0xAA55, 16);
	
	while(1) {
		LED0=0;
		LED1=1;
        
        HCSR04_GetDistane();
        OLED_ShowNum(OLED_LINE2, 1, Distance, 8);
        
        //OLED_ShowString(OLED_LINE2, 1, "123456--");  
		//delay_ms(300);	 //延时300ms
        
		//LED0=1;
		//LED1=0;
        //OLED_ShowString(2, 1, "->abcdef");
		delay_ms(300);	//延时300ms
	}
    
}
