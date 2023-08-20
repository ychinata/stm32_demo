#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "wdg.h"

 
/************************************************
 ALIENTEK精英STM32开发板实验7
 窗口看门狗实验 
 作者：正点原子 @ALIENTEK
 Func：正常喂狗时，led1不停闪烁，LED0亮一下就灭
		没正常喂狗，led0就会闪烁
************************************************/

 int main(void)
 {		
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();
	KEY_Init();          //按键初始化	 
	LED0 = 0; // 亮
	delay_ms(300);	  
	WWDG_Init(0X7F,0X5F,WWDG_Prescaler_8);//计数器值为7f,窗口寄存器为5f,分频数为8	   
 	while (1) {
		 LED0 = 1;	// 灭,如果一直复位，led0就会亮-灭		  	   
	}   
}
  

