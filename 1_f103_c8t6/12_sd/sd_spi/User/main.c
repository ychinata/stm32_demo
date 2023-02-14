#include "stm32f10x.h"                  // Device header
#include "AD.h"
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "malloc.h"  
#include "mmc_sd.h"   
#include "OLED.h"
#include "sys.h"

/********************引脚连接*****************
 * OLED(4针)-oled.c
    SCL-PB8
    SDA-PB9

 * SD卡：    
    PA3 -- SD_CS    (mmc_sd.h)可修改
    PA5 -- SPI1_SCK (SPI1_Init)
    PA6 -- SPI1_MISO(SPI1_Init)
    PA7 -- SPI1_MOSI(SPI1_Init)

**********************************************/

//读取SD卡的指定扇区的内容，并通过串口1输出
//sec：扇区物理地址编号
void SD_Read_Sectorx(u32 sec)
{
	u8 *buf;
	u16 i;
	buf = mymalloc(512);				//申请内存
	if(SD_ReadDisk(buf,sec,1)==0)	//读取0扇区的内容
	{	
		//LCD_ShowString(60,190,200,16,16,"USART1 Sending Data...");
		printf("SECTOR 0 DATA:\r\n");
		for(i=0;i<512;i++)
            printf("%x ",buf[i]);//打印sec扇区数据    	   
		printf("\r\nDATA ENDED\r\n");
		//LCD_ShowString(60,190,200,16,16,"USART1 Send Data Over!");
	}
	myfree(buf);//释放内存	
}
	

/*********************
 * Func.: Mini STM32开发板范例代码28 SD卡实验
 * Author:正点原子,xy
 * Date:2023.2.7
    -
 *********************/ 
int main(void)
{ 
	u8 key;		 
	u32 sd_size;
	u8 t=0;	 
    u16 ret;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2	  
	//uart_init(9600);	 	//串口初始化为9600,用于调试打印
	LED_Init();		  		//初始化与LED连接的硬件接口
	OLED_Init();			   	//初始化LCD	
 	KEY_Init();				//按键初始化  
 	mem_init();				//初始化内存池	 
    
    OLED_ShowString(1, 1, "SD Card TEST");
	//LCD_ShowString(60,130,200,16,16,"KEY0:Read Sector 0");	  
    

    //检测不到SD卡
    ret = SD_Initialize();
    //OLED_ShowHexNum(2, 1, ret, 2);
    OLED_ShowString(2, 1, "2023.2.10");
    
 	while(SD_Initialize()) {
        OLED_ShowString(3, 1, "SD Card Error!");
		Delay_ms(500);					
        OLED_ShowString(3, 1, "Please Check!");
		Delay_ms(500);
		LED1_Turn();//DS0闪烁
	}

	//检测SD卡成功 											    
    OLED_ShowString(3, 1, "SD Card Exist!");

	OLED_ShowString(4, 1, "Size:");
	sd_size=SD_GetSectorCount();//得到扇区数
	OLED_ShowNum(4, 6, sd_size, 10);//显示SD卡容量
    
	while(1) {
        /*
		key = KEY_GetNum();
		if(key == KEY0_PRES)
            SD_Read_Sectorx(0);//KEY0按,读取SD卡扇区0的内容
		t++;
		Delay_ms(10);
		if (t==20) {
			LED1_Turn();
			t=0;
		}
        */
	}    	   
}

// ADC单通道采集
// 2023.1.30
int main1(void)
{
    uint16_t ADValue;
    float voltage;
    
	OLED_Init();
	AD_Init();
	OLED_ShowString(1, 1, "ADC Single Ch:");	
	OLED_ShowString(2, 1, "ADValue:");
	OLED_ShowString(3, 1, "Volatge:0.00V");
	
	while (1) {
		ADValue = AD_GetValue(ADC_Channel_0);
		voltage = (float)ADValue / 4095 * 3.3;
		
		OLED_ShowNum(2, 9, ADValue, 4);
		OLED_ShowNum(3, 9, voltage, 1);     // 显示电压整数部分
		OLED_ShowNum(3, 11, (uint16_t)(voltage * 100) % 100, 2);    // 显示电压小数部分
		
		Delay_ms(100);
	}
}

