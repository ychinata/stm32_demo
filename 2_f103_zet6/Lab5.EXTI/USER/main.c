#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "exti.h"
#include "beep.h"

/************************************************
h00421956
2022.3.1
************************************************/
	
int main(void) {		
	delay_init();	    	//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口 
	BEEP_Init();		 	//初始化蜂鸣器IO
	EXTIX_Init();         	//初始化外部中断输入 
	LED0 = 0;					//先点亮红灯
	LED1 = 0;
	while(1) {	    
		printf("OK\r\n");	
		delay_ms(1000);	  
	}
}
