#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "tpad.h"

/************************************************
 ALIENTEK精英STM32开发板实验11
 电容触摸按键实验 
 Func:
 LED0一直闪烁，程序在运行
 捕获到，则LED1闪烁
 2022.3.15
************************************************/
 
 int main(void) {	
 	u8 t=0;  	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	TPAD_Init(6);			//初始化触摸按键
   	while(1) {				
		//成功捕获到了一次上升沿(此函数执行时间至少15ms)
 		if (TPAD_Scan(0)) { // 不支持连摁
			LED1 =! LED1;		//LED1取反
		}
		t++;
		if (t==15) {
			t=0;
			LED0=!LED0;		//LED0取反,提示程序正在运行
		}
		delay_ms(10);
	}
 }

