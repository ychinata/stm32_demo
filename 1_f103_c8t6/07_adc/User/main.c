#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"


/********************引脚配置*****************
//OLED(4针)-oled.c
SCL-PB8
SDA-PB9

//ADC-ad.c
input1-PA0
input2-PA1
input3-PA2
input4-PA3
**********************************************/

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



// ADC多通道采集(采用单次转换+非扫描模式)
// 2023.1.30
int main(void)
{
    uint16_t adVal0, adVal1, adVal2, adVal3; 
    float voltage;
    
	OLED_Init();
	AD_Init();
	
	OLED_ShowString(1, 1, "ADC Multi Ch:");    
	OLED_ShowString(2, 1, "AD0:");
	OLED_ShowString(3, 1, "AD1:");
	OLED_ShowString(4, 1, "AD3:");
    OLED_ShowChar(4, 11, '.');
    OLED_ShowChar(4, 14, 'V');
	//OLED_ShowString(4, 1, "AD3:");
	
	while (1) {
		adVal0 = AD_GetValue(ADC_Channel_0);
		adVal1 = AD_GetValue(ADC_Channel_1);
		adVal2 = AD_GetValue(ADC_Channel_2);
		adVal3 = AD_GetValue(ADC_Channel_3);
		
		OLED_ShowNum(2, 5, adVal0, 4);
		//OLED_ShowNum(3, 5, adVal1, 4);
		//OLED_ShowNum(4, 5, adVal2, 4);
		OLED_ShowNum(4, 5, adVal3, 4);
        
        //显示转换成电压
		voltage = (float)adVal3 / 4095 * 3.3;		
		OLED_ShowNum(4, 10, voltage, 1);     // 显示电压整数部分
		OLED_ShowNum(4, 12, (uint16_t)(voltage * 100) % 100, 2);    // 显示电压小数部分
		
		Delay_ms(100);
	}
}

