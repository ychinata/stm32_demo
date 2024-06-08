#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "hc-sr04.h"
#include "oled.h"
#include "led.h"
#include "sys.h"

//2024.6.9说明：基于江协科技STM32-OLED工程修改，增加my_gpio.h

int main(void)
{
    u32 distance_mm = 0xFFFFFFFF;
    //模块初始化
	delay_init();	    //延时函数初始化	  
	//LED_Init();		  	//初始化与LED连接的硬件接口     
	OLED_Init();
    HCSR04_Init(0XFFFF, 72-1);  // 超声波测距模块
    
	// 初始化显示
    OLED_ShowString(OLED_LINE1, 1, "HC-SR04 Demo");
    OLED_ShowString(OLED_LINE2, 5, "(mm)");
    OLED_ShowString(OLED_LINE3, 1, "TrigPB1,EchoPB0");
    OLED_ShowString(OLED_LINE4, 1, "2024.6.9");    
	
	while(1) {        
        distance_mm = HCSR04_GetDistane();          // 获取超声波模块测量的距离
        OLED_ShowNum(OLED_LINE2, 1, distance_mm, 4);// 在OLED显示距离
        
		delay_ms(300);	//延时300ms
	}
    
}
