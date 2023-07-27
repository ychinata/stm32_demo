#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"  
#include "24cxx.h" 
#include "myiic.h"
#include "usmart.h"
#include "vl53l0x.h"

/************************************************
 ALIENTEK MiniV3 STM32开发板扩展实验21
 ATK-VL53L0X激光测距模块 综合测试实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
   	
 int main(void)
 { 
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组2
	delay_init();	                               //延时函数初始化	  
	uart_init(115200);	                           //串口初始化为115200
	usmart_dev.init(72);                           //初始化USMART	
	LED_Init();		  	                           //初始化与LED连接的硬件接口
	//LCD_Init();	                                   //LCD初始化
	KEY_Init();			                           //按键初始化		 	
	//AT24CXX_Init();		                           //IIC初始化 

	while(AT24CXX_Check())//检测不到24c02
	{
		LCD_ShowString(30,150,200,16,16,"24C02 Check Failed!");
		delay_ms(500);
		LCD_ShowString(30,150,200,16,16,"Please Check!      ");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	}
	while(1)
	{
		vl53l0x_test();//vl53l0x测试
	}
}
