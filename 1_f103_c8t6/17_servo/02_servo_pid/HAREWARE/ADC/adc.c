#include "adc.h"
 /**************************************************************************
作者：平衡小车之家 
淘宝店铺：http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
函数功能：ADC初始化电池电压检测
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
void  Baterry_Adc_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE ); //使能GPIOA时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;        //PA6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE );  //使能ADC1时钟
  ADC_DeInit(ADC1);        //ADC1复位
	
 	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//初始化外设ADCx的寄存器   

  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
}		

/**************************************************************************
函数功能：AD采样
入口参数：ADC1 的通道
返回  值：AD转换结果
作    者：平衡小车之家
**************************************************************************/
u16 Get_Adc(u8 ch)   
{
	
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
	
}

/**************************************************************************
函数功能：读取电池电压 
入口参数：无
返回  值：电池电压 单位MV
作    者：平衡小车之家
**************************************************************************/
int Get_battery_volt(void)   
{  
	int Volt;                                          //电池电压
	Volt=Get_Adc(Battery_Ch)*3.3*11*100/1.0/4096;		 //电阻分压，具体根据原理图简单分析可以得到
	return Volt;
}

///**************************************************************************
//函数功能：角位移传感器初始化
//入口参数：无
//返回  值：无
//作    者：平衡小车之家
//**************************************************************************/
//void  Angle_Adc_Init(void)
//{    
////先初始化IO口
// 	RCC->APB2ENR|=1<<2;    //使能PORTA口时钟 
//	GPIOA->CRL&=0X0FFFFFFF;//PA7 anolog输入 	 		 
//	RCC->APB2ENR|=1<<9;    //ADC1时钟使能	  
//	RCC->APB2RSTR|=1<<9;   //ADC1复位
//	RCC->APB2RSTR&=~(1<<9);//复位结束	    
//	RCC->CFGR&=~(3<<14);   //分频因子清零	
//	//SYSCLK/DIV2=12M ADC时钟设置为12M,ADC最大时钟不能超过14M!
//	//否则将导致ADC准确度下降! 
//	RCC->CFGR|=2<<14;      	 

//	ADC1->CR1&=0XF0FFFF;   //工作模式清零
//	ADC1->CR1|=0<<16;      //独立工作模式  
//	ADC1->CR1&=~(1<<8);    //非扫描模式	  
//	ADC1->CR2&=~(1<<1);    //单次转换模式
//	ADC1->CR2&=~(7<<17);	   
//	ADC1->CR2|=7<<17;	   //软件控制转换  
//	ADC1->CR2|=1<<20;      //使用用外部触发(SWSTART)!!!	必须使用一个事件来触发
//	ADC1->CR2&=~(1<<11);   //右对齐	 
//	ADC1->SQR1&=~(0XF<<20);
//	ADC1->SQR1&=0<<20;     //1个转换在规则序列中 也就是只转换规则序列1 			   
//	//设置通道7的采样时间
//	ADC1->SMPR2&=0X0FFFFFFF;//通道7采样时间清空	  
//	ADC1->SMPR2|=7<<21;      //通道7  239.5周期,提高采样时间可以提高精确度	 

//	ADC1->CR2|=1<<0;	    //开启AD转换器	 
//	ADC1->CR2|=1<<3;        //使能复位校准  
//	while(ADC1->CR2&1<<3);  //等待校准结束 			 
//    //该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。 		 
//	ADC1->CR2|=1<<2;        //开启AD校准	   
//	while(ADC1->CR2&1<<2);  //等待校准结束
//	delay_ms(1);
//}		

//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_us(200);
	}
	return temp_val/times;
} 
	 








