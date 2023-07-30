#include "stm32f10x.h" //stm32头文件
#include "usart.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "vl53l0x.h"
#include "vl53l0x_gen.h"
#include "GUI.h"

/*可以实现测距，测距精度还没有确定
https://gitee.com/tiramisu-yang/vl53-x-series-project/tree/master

RCT6 MiniPro板接线：
SCL-->PA3
SDA-->PA2
INT-->不接
XSHUT-->PA5
见：vl53l0x_init
VL53L0X_i2c_init


F103精英板接线：
SCL-->PB11
SDA-->PB10
XSHUT-->PA15

*/


int main(void)
{      
	u8 mode_vl53l0x = 2; //0：默认；1：高精度；2：长距离；3：高速度
	u32 i;
    u32 distanceByLaser;
	//
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//分组2
	uart_init(9600);  //串口1
	delay_init();
	LED_Init();
    Lcd_Init();	 //液晶屏初始化(1.44寸)
	VL53L0X_MainInit(mode_vl53l0x);    
	delay_ms(200);
    //
    Lcd_Clear(GRAY0);//清屏
    Gui_DrawFont_GBK16(0,16,RED,GRAY0, "STM32F103RCT6");
    Gui_DrawFont_GBK16(0,32,BLUE,GRAY0,"2023.7.30"); 
    Gui_DrawFont_GBK16(0,48,BLUE,GRAY0,"VL53L0X Range:"); 
    //Gui_DrawFont_GBK16(0,64,BLUE,GRAY0,"mcudev.taobao.com");
    //Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"2023.7.29");    
		
    while(1)
    {        
        distanceByLaser = VL53L0X_GetDistance();
        printf("激光测距d: %4i (mm)\r\n",distanceByLaser);//打印测量距离
        //激光测距任务适当延时
        i++;
        if (i>=10) {
            i=0;
            LED0=!LED0;
            delay_ms(1000);
        }
    }
}
/***************************************************************************/
/***************************************************************************/



