#include "led.h"
#include "delay.h"
#include "oled.h"
#include "sys.h"


/* 引脚分配：

    LED: led.h
    OLED: oled.h
    
 */

int main(void)
    {	
    // 模块初始化bgn
    delay_init();	    	 //延时函数初始化	  
    LED_Init();		  	//初始化与LED连接的硬件接口
    OLED_Init();
    // 模块初始化end
        
    OLED_ShowString(1, 1, "Hello,F103RCT6!");
    OLED_ShowString(3, 1, "2023.7.27");         
        
    while(1)
    {
        LED0=0;
        LED1=1;
        delay_ms(300);	 //延时300ms
        LED0=1;
        LED1=0;
        delay_ms(300);	//延时300ms
    }
}

