#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t ADValue;
float Voltage;

/********************引脚配置*****************
//OLED(4针)-oled.c
SCL-PB8
SDA-PB9

//ADC-ad.c
input1-PA0

**********************************************/

// ADC单通道采集
// 2023.1.30
int main(void)
{
	OLED_Init();
	AD_Init();
	OLED_ShowString(1, 1, "ADC Single Ch:");	
	OLED_ShowString(2, 1, "ADValue:");
	OLED_ShowString(3, 1, "Volatge:0.00V");
	
	while (1) {
		ADValue = AD_GetValue(ADC_Channel_7);
		Voltage = (float)ADValue / 4095 * 3.3;
		
		OLED_ShowNum(2, 9, ADValue, 4);
		OLED_ShowNum(3, 9, Voltage, 1);
		OLED_ShowNum(3, 11, (uint16_t)(Voltage * 100) % 100, 2);
		
		Delay_ms(100);
	}
}
