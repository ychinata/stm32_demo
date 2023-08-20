#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"


/************************************************
Date: 2022.3.5
Author: h00421956
Func:  定时器中断实验

led0 200ms闪烁一次
led1 500ms闪烁一次
************************************************/
	
int main(void) {		
	delay_init();	    	 	//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 		//串口初始化为115200
 	LED_Init();			      	//LED端口初始化
	TIM3_Int_Init(4999,7199); 	//10Khz的计数频率，计数到5000为500ms  
   	while (1) {
		LED0 = !LED0;
		delay_ms(200);		   
	}	 
}	 

