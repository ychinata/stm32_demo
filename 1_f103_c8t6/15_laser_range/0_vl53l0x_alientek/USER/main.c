#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "led.h"
#include "sys.h"
#include "vl53l0x.h"

int main(void)
{
	u8 mode_vl53l0x = 2; //0��Ĭ�ϣ�1���߾��ȣ�2�������룻3�����ٶ�    
    u32 distanceByLaser;
    
    //
	delay_init();	    //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�     
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
        printf("������d: %4i (mm)\r\n",distanceByLaser);//��ӡ��������        
        
        OLED_ShowString(2, 1, "123456--");  
		delay_ms(500);	 //��ʱ300ms
        OLED_ShowString(2, 1, "->abcdef");
		delay_ms(500);	//��ʱ300ms
	}
    
}
