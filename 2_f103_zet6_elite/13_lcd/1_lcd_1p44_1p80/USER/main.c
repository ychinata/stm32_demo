#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h" 
 
#include "lcd.h"
#include "Dis_Picture.h" 
#include "Text.h"	
#include "GBK_LibDrive.h"	 
 
 
/************************************************/
//DevEBox  大越创新 mcudev.taobao.com
//STM32F103 开发板  1.44寸液晶屏 SPI 接口   实验

/* LED */
// LED_D0 -- PB9     --低电平点亮      
// LED_D1 -- PE5	 --低电平点亮    

/* KEY */
// WKUP -- PA0        WKUP 按键 --按下后，高电平  
// K0   -- PE4        K0   按键 --按下后，低电平    

/* USART0串口通信 */
//USART0_TX---PA9     --单片机串口输出
//USART0_RX---PA10    --单片机串口输入

/* SPI TFT LCD屏 */
//LCD_SDA        	//PB15      //数据输入线
//LCD_SCL        	//PB13      //时钟线
//LCD_CS        	//PB12      //片选	
//LCD_SDO/RST     //PB14      //数据输出/复位
//LCD_DC         	//PC5       //命令/数据切换
//LCD_BLK         //PB1    	  //背光控制
/************************************************/

uint16_t t;  
uint16_t len;	
uint16_t times=0;
		
int main(void)
{	    	
	delay_init();	    //SysTick 延时函数初始化		 
	LED_Init();		  	//初始化与LED连接的硬件接口	 
	KEY_Init();       //初始化按键 --模式是IO输入模式  
	USART1_init(115200);          //串口1     初始化函数 	 
	LCD_Init();           //初始化LCD 1.44寸液晶屏初始化  SPI 接口 	 
	GBK_Lib_Init();       //硬件GBK字库初始化--(如果使用不带字库的液晶屏版本，此处可以屏蔽，不做字库初始化）
	 
	while(1) {
		
        LCD_DemoMenu();//演示程序		
		
        if(KEY0_PRES == KEY_Scan(1)) {
//  	    LED0=0;
            LED_D0_toggle;//闪烁LED,提示系统正在运行
            delay_ms(500);
//			LED0=1;
        }
		
		if (USART_RX_STA&0x8000) {					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度			
			printf("\r\n您发送的消息为:\r\n\r\n");			
			for(t=0;t<len;t++) {				
				USART_SendData(USART1, USART_RX_BUF[t]);//向串口0  发送数据				
				while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE));//等待发送结束				
			}
			printf("\r\n\r\n");//插入换行			
			USART_RX_STA=0;			
		} else {
			times++;			
			if(times%500==0) {
				printf("\r\n ST32F103 开发板 串口1 测试\r\n");
				printf("mcudev.taobao.com\r\n\r\n");
			}			
			if(times%200==0)
                printf("请输入数据,以回车键结束\n");			
			if(times%30==0)
                LED_D1_toggle;//闪烁LED,提示系统正在运行			
			delay_ms(10);   
		}				
	}
 }



	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
/************************************************
	
//STM32F103工程模板-库函数版本
//DevEBox  大越创新

//淘宝店铺：mcudev.taobao.com
//淘宝店铺：devebox.taobao.com	

************************************************/

