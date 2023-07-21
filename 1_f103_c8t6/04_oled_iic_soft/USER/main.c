#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "oled_soft.h"
#include "oled_iic_soft.h"
#include "led.h"
#include "sys.h"

int main(void)
{
	delay_init();	    //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口     
	OLED_Init();	
    //test
    Draw_DATA(0, 0, 1);
	
	while(1) {

	}
    
}
