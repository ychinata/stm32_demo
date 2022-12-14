#include "stm32f10x.h"                  // Device header

//存储ADC转换的数值
uint16_t AD_Value[2];

/**
  * @brief  ADC初始化
  * @param  无
  * @retval 无
  */
void AD_Init(void)
{	
	//ADC、GPIO和DMA时钟配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	//配置ADCCLK分频器
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//GPIO初始化
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//选择输入通道,设置好菜单
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_55Cycles5);
	
	//ADC初始化
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
//	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ADC单次扫描
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ADC连续扫描
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_NbrOfChannel = 2;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	//DMA初始化
	DMA_InitTypeDef DMA_InitStructrue;
	DMA_InitStructrue.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructrue.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructrue.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructrue.DMA_MemoryBaseAddr = (uint32_t)AD_Value;
	DMA_InitStructrue.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructrue.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructrue.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructrue.DMA_BufferSize = 2;
//	DMA_InitStructrue.DMA_Mode = DMA_Mode_Normal;	//DMA单次转运
	DMA_InitStructrue.DMA_Mode = DMA_Mode_Circular;	//DMA循环转运
	DMA_InitStructrue.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructrue.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1,&DMA_InitStructrue);
	
	//使能DMA
	DMA_Cmd(DMA1_Channel1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	//开启ADC的电源
	ADC_Cmd(ADC1,ENABLE);
	
	//进行校准
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
	//ADC触发
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

//ADC连续扫描，DMA循环转运,不需要以下函数
//void AD_GetValue(void)
//{
//	DMA_Cmd(DMA1_Channel1,DISABLE);
//	DMA_SetCurrDataCounter(DMA1_Channel1,4);
//	DMA_Cmd(DMA1_Channel1,ENABLE);
//	
//	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
//	DMA_ClearFlag(DMA1_FLAG_TC1);
//}

