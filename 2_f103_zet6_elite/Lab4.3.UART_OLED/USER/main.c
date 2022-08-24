#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "OLED.h"

#define BUFLEN 10

void UART_Task(void);
void UART_Task_onlysend(void);

/************************************************
 串口 实验    
 作者：正点原子 @ALIENTEK
 usrt和usart有什么区别?
 2022.5.14
************************************************/
int main(void) {		
	
	// 初始化
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200,入参为波特率
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	OLED_Init();
	 
	OLED_ShowString(4, 1, "2022.5.14"); 
	
 	while (1) {
		//UART_Task();
		UART_Task_onlysend(); //调试5：串口会刷屏
		//调试6：按键触发时再发送？
	}
 
}

//2022.5.14
void UART_Task_onlysend(void) {
 	u16 t;
	for (t = 0; t < BUFLEN; t++) {
		// 调试4：
		USART_SendData(USART1, 0x61);//向串口1发送数据
		
		while (USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	}
	printf("\r\n");//插入换行
}

//2022.5.14
void UART_Task(void) {
 	u16 t;  
	u16 len;	
	static u16 times=0;
	static u8 send_buf[BUFLEN] = {0x61,0x63,0x65,0x67,0x69,0x6b,0x6d,0x6f,0x71,0x73};	
	
	// 如果接收到数据
	if (USART_RX_STA & 0x8000) { // 接收状态标记					   
		len = USART_RX_STA & 0x3fff;//得到此次接收到的数据长度
		//printf("\r\n您发送的消息为:\r\n\r\n");
		printf("\r\nsend\r\n");
		/* //调试1：测试接到的数据长度，以及数据保存的格式，为ASCII码
		OLED_ShowNum(1, 1, len, 3);
		OLED_ShowNum(2, 1, USART_RX_BUF[0], 3);//接收到的第1个字符丢失了，上位机无法接收到，上位机发送4个字符，只能返回3个字符，待定位
		OLED_ShowHexNum(2, 4, USART_RX_BUF[0], 2);
		OLED_ShowHexNum(3, 1, USART_RX_BUF[1], 2);
		*/
		
		//for (t = 0; t < len; t++) {
		for (t = 0; t < BUFLEN; t++) {
			// 调试1：
			//USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
			// 调试2：
			//USART_SendData(USART1, 0x61+t);//向串口1发送数据
			// 调试3：
			USART_SendData(USART1, send_buf[t]);//向串口1发送数据
			
			while (USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
		}
		printf("\r\n");//插入换行
		USART_RX_STA = 0;
	// 如果没接收到数据
	} else {
		times++;
		if (times % 5000 == 0) {
			//printf("\r\n精英STM32开发板 串口实验\r\n");
			//printf("正点原子@ALIENTEK\r\n\r\n");
			printf("\r\ntest\r\n");				
		}
		if (times % 200 == 0) {
			//printf("请输入数据,以回车键结束\r\n");  
			printf("input\r\n");				
		}
		if (times % 30==0) {
			LED0 =! LED0;//闪烁LED,提示系统正在运行.
		}
		delay_ms(10);   
	}

}

