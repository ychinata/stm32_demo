#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
 
/************************************************
 ALIENTEK精英STM32开发板实验4
 串口 实验    
 作者：正点原子 @ALIENTEK
 usrt和usart有什么区别?
 2022.2.28走读,运行成功
************************************************/
 int main(void)
 {		
 	u16 t;  
	u16 len;	
	u16 times=0;
	// 初始化
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200,入参为波特率
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	
 	while (1) {
		// 如果接收到数据
		if (USART_RX_STA & 0x8000) { // 接收状态标记					   
			len = USART_RX_STA & 0x3fff;//得到此次接收到的数据长度
			printf("\r\n您发送的消息为:\r\n\r\n");
			for (t = 0; t < len; t++) {
				USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
				while (USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			printf("\r\n\r\n");//插入换行
			USART_RX_STA = 0;
		// 如果没接收到数据
		} else {
			times++;
			if (times % 5000 == 0) {
				printf("\r\n精英STM32开发板 串口实验\r\n");
				printf("正点原子@ALIENTEK\r\n\r\n");
			}
			if (times % 200 == 0) {
				printf("请输入数据,以回车键结束\r\n");  
			}
			if (times % 30==0) {
				LED0 =! LED0;//闪烁LED,提示系统正在运行.
			}
			delay_ms(10);   
		}
	}	 
 }

