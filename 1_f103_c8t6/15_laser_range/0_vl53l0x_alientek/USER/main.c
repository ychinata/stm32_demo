#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "led.h"
#include "sys.h"
#include "vl53l0x.h"

int main(void)
{
	u8 mode_vl53l0x = 2; //0：默认；1：高精度；2：长距离；3：高速度    
    u32 distanceByLaser;
    
    //
	delay_init();	    //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口     
	OLED_Init();
	VL53L0X_MainInit(mode_vl53l0x); 
    //
    
	OLED_ShowChar(1, 1, 'A');
    OLED_ShowString(1, 1, "Hello,4Pin OLED!");
    OLED_ShowString(3, 1, "2022.5.1");    
	// OLED_ShowString(1, 3, "HelloWorld!");
	// OLED_ShowNum(2, 1, 12345, 5);
	// OLED_ShowSignedNum(2, 7, -66, 2);
	// OLED_ShowHexNum(3, 1, 0xAA55, 4);
	// OLED_ShowBinNum(4, 1, 0xAA55, 16);
	
	while(1) {
        distanceByLaser = VL53L0X_GetDistance();
        printf("激光测距d: %4i (mm)\r\n",distanceByLaser);//打印测量距离        
        
        OLED_ShowString(2, 1, "123456--");  
		delay_ms(500);	 //延时300ms
        OLED_ShowString(2, 1, "->abcdef");
		delay_ms(500);	//延时300ms
	}
    
}
