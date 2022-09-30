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
	delay_ms(300);	 //��ʱ300ms
	
	//LED0=1;
	//LED1=0;
	OLED_ShowString(2, 1, "->abcdef");
	delay_ms(300);	//��ʱ300ms
}

void DrawTestPage(u8 *str)
{
	//���ƹ̶���up
	LCD_Clear(WHITE);
	LCD_Fill(0,0,lcddev.width,20,BLUE);
	//���ƹ̶���down
	LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
	POINT_COLOR=WHITE;
	Gui_StrCenter(0,2,WHITE,BLUE,str,16,1);//������ʾ
	Gui_StrCenter(0,lcddev.height-18,WHITE,BLUE,"www.ftf.com",16,1);//������ʾ
	//���Ʋ�������
	//LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
}

void LCD_MainTest(void)
{
	DrawTestPage("�ۺϲ��Գ���");	
	Gui_StrCenter(0,23,RED,BLUE,"����",16,1);			//������ʾ,������Ҫȡģ֮�������ʾ?
	Gui_StrCenter(0,40,RED,BLUE,"Helloworld����",16,1);	//������ʾ	
	Gui_StrCenter(0,57,GREEN,BLUE,"1.8\" ST7735S",16,1);//������ʾ
	Gui_StrCenter(0,74,GREEN,BLUE,"128x160",16,1);		//������ʾ
	Gui_StrCenter(0,91,BLUE,BLUE,"2022-09-30",16,1);	//������ʾ
	delay_ms(1000);		// ÿ1��ˢ��һ��, ԭ����3��
}

int main(void)
{
	
//	delay_init();	    //��ʱ������ʼ��	  
//	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
//	OLED_Init();	
//	OLED_ShowChar(1, 1, 'A');
//    OLED_ShowString(1, 1, "Hello,4Pin OLED!");
//    OLED_ShowString(3, 1, "2022.5.1");    
	
	// LCD_Init
	SystemInit();//��ʼ��RCC ����ϵͳ��ƵΪ72MHZ
	delay_init();	     //��ʱ��ʼ��
	LCD_Init();	   //Һ������ʼ��	
	
	while(1) {
		// OLED_Test();
		LCD_MainTest();
	}
}
