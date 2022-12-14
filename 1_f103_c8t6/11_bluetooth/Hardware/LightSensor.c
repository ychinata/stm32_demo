#include "stm32f10x.h"                  // Device header
#include "AD.h"

/**
  * @brief  LightSensor初始化
  * @param  无
  * @retval 无
  */
void LightSensor_Init(void)
{
	AD_Init();
}

/**
  * @brief  LightSensor获取亮度
  * @param  无
  * @retval Luminance 亮度值
  */
uint8_t LightSensor_GetNum(void)
{	
	uint8_t Luminance;
	Luminance = 100 - (uint8_t)((float)AD_Value[0] / 4095 * 100);
	return Luminance;
}

