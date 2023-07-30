#include "stm32f10x.h" //stm32头文件
#include "usart.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "vl53l0x.h"
#include "vl53l0x_gen.h"
//
#include "GUI.h"
#include "Lcd_Driver.h"
#include "TFT_demo.h"


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



/******************************************************************************/
extern VL53L0X_Dev_t vl53l0x_dev;//设备I2C数据参数
/******************************************************************************/
int main(void)
{
	uint8_t data;
	uint8_t len;
	uint16_t word;
	uint8_t data1;
    u32 delayTime;
    
    uint8_t range[20];
    
	static char buf[VL53L0X_MAX_STRING_LENGTH];//测试模式字符串字符缓冲区
	VL53L0X_Error Status=VL53L0X_ERROR_NONE;//工作状态
	u8 mode=2; //0：默认；1：高精度；2：长距离；3：高速度
	u32 i;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//分组2
	uart_init(9600);  //串口1
	delay_init();
	LED_Init();
    Lcd_Init();	 //1.44寸液晶屏--初始化配置
	delay_ms(200);
    
    Lcd_Clear(GRAY0);//清屏
    Gui_DrawFont_GBK16(0,16,RED,GRAY0, "STM32F103RCT6");
    Gui_DrawFont_GBK16(0,32,BLUE,GRAY0,"2023.7.30"); 
    Gui_DrawFont_GBK16(0,48,BLUE,GRAY0,"VL53L0X Range:"); 
    //Gui_DrawFont_GBK16(0,64,BLUE,GRAY0,"mcudev.taobao.com");
    //Gui_DrawFont_GBK16(0,80,BLUE,GRAY0,"2023.7.29");    
	
	if(vl53l0x_init(&vl53l0x_dev))     //vl53l0x初始化
	 {
		printf("VL53L0X_Init Error!!!\r\n");
		delay_ms(200);
	 }
	 else
	 {
		 printf("VL53L0X_Init OK\r\n");
		 
		 VL53L0X_RdByte(&vl53l0x_dev,0xC0,&data);
		 printf("regc0 = 0x%x\n\r",data);
		
		 VL53L0X_RdByte(&vl53l0x_dev,0xC1,&data1);
		 printf("regc1 = 0x%x\n\r",data1);
		 
		 VL53L0X_RdByte(&vl53l0x_dev,0xC2,&len);
		 printf("regc2 = 0x%x\n\r",len); 
		 
		 Status = VL53L0X_WrByte(&vl53l0x_dev,0x51,0x12);
		 VL53L0X_RdByte(&vl53l0x_dev,0x51,&len);  //高四位没法读出来
		 printf("reg51 = 0x%x\n\r",len);
		 
		 Status = VL53L0X_WrWord(&vl53l0x_dev,0x62,0x9e12);
		 VL53L0X_RdWord(&vl53l0x_dev,0x62,&word);
		 printf("reg61 = 0x%x\n\r",word);
	 }
	
	if(vl53l0x_set_mode(&vl53l0x_dev,mode))   //配置测量模式
		{
			printf("Mode Set Error!!!\r\n");
		}
	else
		printf("Mode Set OK!!!\r\n");
	
	
    while(1)
    {
        Status = vl53l0x_start_single_test(&vl53l0x_dev,&vl53l0x_data,buf);//执行一次测量
        if(Status==VL53L0X_ERROR_NONE) {
            delayTime = 100;
            // 模块未校正有误差，手动修正30mm.临时补丁20230730
            if (Distance_data > 30) {
                Distance_data -= 30;
            }
            
            sprintf((char*)range,"%4d mm", Distance_data);   
            Gui_DrawFont_GBK16(0,64,BLUE,GRAY0, range);
            printf("d: %4imm\r\n",Distance_data);//打印测量距离
        } else {
            delayTime = 100;
            printf("Measurement is Error!!!\r\n");
        }
        //delay_ms(100);
        i++;
        if(i>=10)
        {
            i=0;
            LED0=!LED0;
            delay_ms(delayTime);
        }
    }
}
/***************************************************************************/
/***************************************************************************/



