#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"

/************************************************
 ALIENTEK精英STM32开发板实验10
 输入捕获实验  
 作者：正点原子 @ALIENTEK
 2022.3.14
 Func：摁下key_up键，会捕获按下的时间，并通过串口发送到电脑
************************************************/

extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM5CH1_CAPTURE_VAL;	//输入捕获值	
int main(void) {		
 	u32 temp = 0; 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
 
 	TIM3_PWM_Init(899,0); 		//不分频。PWM频率=72000/(899+1)=80Khz
 	TIM5_Cap_Init(0XFFFF,72-1);	//以1Mhz的频率计数 
   	while(1) {
 		delay_ms(10);
		TIM_SetCompare2(TIM3,TIM_GetCapture2(TIM3)+1);
	
		if (TIM_GetCapture2(TIM3)==300) {
			TIM_SetCompare2(TIM3,0);	
		}
		 		 
 		if (TIM5CH1_CAPTURE_STA & 0X80) { //成功捕获到了一次上升沿
			temp = TIM5CH1_CAPTURE_STA & 0X3F;
			temp *= 65536;//溢出时间总和
			temp += TIM5CH1_CAPTURE_VAL;//得到总的高电平时间
			printf("HIGH:%d us\r\n",temp);//打印总的高点平时间
			TIM5CH1_CAPTURE_STA = 0;//开启下一次捕获
		}
	}
}

