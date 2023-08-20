#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
 
/************************************************
Date: 2022.3.8
Author: h00421956
Func:  PWM输出实验 
		led0有了呼吸灯的效果，为什么是led0，因为
		把 TIM3_CH2 直接映射到了 PB5 上，PB5连接了led0
		该实验不需要使用中断
************************************************/

 int main(void)
 {		
 	u16 led0pwmval=0;
	u8 dir=1;	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
 	TIM3_PWM_Init(899,0);	 //不分频。PWM频率=72000000/900=80Khz
   	while(1) {
 		delay_ms(10);	 
		if (dir) {
			led0pwmval++;
		} else {
			led0pwmval--;
		}

 		if (led0pwmval>300) {
			dir=0; // 翻转，由亮变暗
		}
		if (led0pwmval==0) {
			dir=1; // 翻转，由暗变亮
		}			
		TIM_SetCompare2(TIM3,led0pwmval);		   
	}	 
 }

