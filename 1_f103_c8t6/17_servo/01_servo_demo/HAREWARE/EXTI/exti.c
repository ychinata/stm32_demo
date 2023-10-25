#include "exti.h"
#include "key.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
函数功能：外部中断初始化
入口参数：无
返回  值：无 
**************************************************************************/
void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
	KEY_Init();	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //外部中断，需要使能AFIO时钟
	
	if(PAin(12)==0)    
	{
		Menu_MODE=2;   //按住A12按键启动，进入速度模式
		TIM_SetCounter(TIM4,0);
}

		//GPIOA.12 中断线以及中断初始化配置
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
  EXTI_InitStructure.EXTI_Line=EXTI_Line12;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器	
	
	//GPIOA.5 中断线以及中断初始化配置
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);
  EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	//GPIOA.7 中断线以及中断初始化配置
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource7);
  EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	//GPIOA.11 中断线以及中断初始化配置
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);
  EXTI_InitStructure.EXTI_Line=EXTI_Line11;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
//	//GPIOA.12 中断线以及中断初始化配置
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
//  EXTI_InitStructure.EXTI_Line=EXTI_Line12;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	//设置IO口与中断线的映射关系
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;         //使能按键所在的外部中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;            //使能外部中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;  //抢占优先级2 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;         //响应优先级2
	NVIC_Init(&NVIC_InitStruct);
	//设置IO口与中断线的映射关系
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;       //使能按键所在的外部中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;            //使能外部中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;  //抢占优先级2 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;         //响应优先级2
	NVIC_Init(&NVIC_InitStruct);  
}

//外部中断9~5服务程序
void EXTI9_5_IRQHandler(void)
{			
	delay_ms(4);     //消抖		
	
   if(KEY5==0)	   //开/关电机
	{
		Flag_Stop=!Flag_Stop; 
	}		
	 if(KEY7==0)	//选择要调的P/I/D参数
	{
		if(Menu_MODE==1)
		{	
		if(++Menu_PID>3) Menu_PID=1;
		}
		else
		{	
		if(++Menu_PID>2) Menu_PID=1;
		}	
	}		
	EXTI_ClearITPendingBit(EXTI_Line5);  //清除LINE5上的中断标志位 
	EXTI_ClearITPendingBit(EXTI_Line7);  //清除LINE7上的中断标志位 
}
//外部中断15~10服务程序
void EXTI15_10_IRQHandler(void)
{			
	delay_ms(4);   //消抖		
	if(Menu_MODE==1)  //PID控制
	{	
			if(KEY11==0)	//PID-
			{
				      if(Menu_PID==1)  Position_KP-=Amplitude_PKP; //P的值一次减20
				else	if(Menu_PID==2)  Position_KI-=Amplitude_PKI; //I的值一次减0.1
				else	if(Menu_PID==3)  Position_KD-=Amplitude_PKD; //D的值一次减25
			}		
			 if(KEY12==0)	//PID+ 
			{
							if(Menu_PID==1)  Position_KP+=Amplitude_PKP; //P的值一次加20
				else	if(Menu_PID==2)  Position_KI+=Amplitude_PKI; //I的值一次加0.1
				else	if(Menu_PID==3)  Position_KD+=Amplitude_PKD; //D的值一次加25
			}		
  }
	else            //PI控制
	{	
			if(KEY11==0)	//PI-
			{
				      if(Menu_PID==1)  Velocity_KP-=Amplitude_VKP; //P的值一次减2
				else	if(Menu_PID==2)  Velocity_KI-=Amplitude_VKI; //I的值一次减3
			}		
			 if(KEY12==0)	//PI+ 
			{
							if(Menu_PID==1)  Velocity_KP+=Amplitude_VKP; //P的值一次加2
				else	if(Menu_PID==2)  Velocity_KI+=Amplitude_VKI; //I的值一次加3
			}		
  }
	if(Velocity_KP<=0) Velocity_KP=0;
	if(Velocity_KI<=0) Velocity_KI=0;
	if(Position_KD<=0) Position_KD=0;
	if(Position_KP<=0) Position_KP=0;
	if(Position_KI<=0) Position_KI=0;  //参数不小于0
	EXTI_ClearITPendingBit(EXTI_Line11);  //清除LINE11上的中断标志位 
	EXTI_ClearITPendingBit(EXTI_Line12);  //清除LINE12上的中断标志位 

}





