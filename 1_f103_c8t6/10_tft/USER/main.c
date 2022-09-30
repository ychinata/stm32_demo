#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "led.h"
#include "sys.h"
#include "lcd.h"
#include "gui.h"

void OLED_Test()
{
	LED0=0;
	LED1=1;
	OLED_ShowString(2, 1, "123456--");  
	delay_ms(300);	 //延时300ms
	
	//LED0=1;
	//LED1=0;
	OLED_ShowString(2, 1, "->abcdef");
	delay_ms(300);	//延时300ms
}

void DrawTestPage(u8 *str)
{
	//绘制固定栏up
	LCD_Clear(WHITE);
	LCD_Fill(0,0,lcddev.width,20,BLUE);
	//绘制固定栏down
	LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
	POINT_COLOR=WHITE;
	Gui_StrCenter(0,2,WHITE,BLUE,str,16,1);//居中显示
	Gui_StrCenter(0,lcddev.height-18,WHITE,BLUE,"www.ftf.com",16,1);//居中显示
	//绘制测试区域
	//LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
}

void LCD_MainTest(void)
{
	DrawTestPage("综合测试程序");	
	Gui_StrCenter(0,23,RED,BLUE,"电子",16,1);			//居中显示,中文需要取模之后才能显示?
	Gui_StrCenter(0,40,RED,BLUE,"Helloworld程序",16,1);	//居中显示	
	Gui_StrCenter(0,57,GREEN,BLUE,"1.8\" ST7735S",16,1);//居中显示
	Gui_StrCenter(0,74,GREEN,BLUE,"128x160",16,1);		//居中显示
	Gui_StrCenter(0,91,BLUE,BLUE,"2022-09-30",16,1);	//居中显示
	delay_ms(1000);		// 每1秒刷新一次, 原来是3秒
}

int main(void)
{
	
//	delay_init();	    //延时函数初始化	  
//	LED_Init();		  	//初始化与LED连接的硬件接口
//	OLED_Init();	
//	OLED_ShowChar(1, 1, 'A');
//    OLED_ShowString(1, 1, "Hello,4Pin OLED!");
//    OLED_ShowString(3, 1, "2022.5.1");    
	
	// LCD_Init
	SystemInit();//初始化RCC 设置系统主频为72MHZ
	delay_init();	     //延时初始化
	LCD_Init();	   //液晶屏初始化	
	
	while(1) {
		// OLED_Test();
		LCD_MainTest();
	}
}
