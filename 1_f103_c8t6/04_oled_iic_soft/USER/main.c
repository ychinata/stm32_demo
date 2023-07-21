#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "oled_soft.h"
#include "led.h"
#include "sys.h"

int main(void)
{
	delay_init();	    //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口 
    
	OLED_Init();
	
//	OLED_ShowChar(1, 1, 'A');
//    OLED_ShowChar(0,0, 'A',OLED_FONTSIZE);  //第一行
//    OLED_ShowChar(0,16,'B',OLED_FONTSIZE);  //第二行
//    OLED_ShowChar(0,32,'C',OLED_FONTSIZE);  //第三行
    OLED_ShowString(0,0,(u8 *)"ABCDEFGHIJKLMNO",OLED_FONTSIZE);  //第四行
    OLED_ShowString(0,16,(u8 *)"ABCDEFGHIJKLMNO",OLED_FONTSIZE);  //第四行
    OLED_ShowString(0,32,(u8 *)"IIC SOFT",OLED_FONTSIZE);  //第四行
    OLED_ShowString(0,48,(u8 *)"2023.7.21",OLED_FONTSIZE);  //第四行
    OLED_Refresh_Gram();
    
	
	while(1) {
		LED0=0;
		LED1=1;
        //OLED_ShowString(2, 1, "123456--");  
		delay_ms(300);	 //延时300ms
        
		//LED0=1;
		//LED1=0;
        //OLED_ShowString(2, 1, "->abcdef");
		delay_ms(300);	//延时300ms
	}
    
}
