#include "exti.h"
#include "key.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
�������ܣ��ⲿ�жϳ�ʼ��
��ڲ�������
����  ֵ���� 
**************************************************************************/
void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
	KEY_Init();	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //�ⲿ�жϣ���Ҫʹ��AFIOʱ��
	
	if(PAin(12)==0)    
	{
		Menu_MODE=2;   //��סA12���������������ٶ�ģʽ
		TIM_SetCounter(TIM4,0);
}

		//GPIOA.12 �ж����Լ��жϳ�ʼ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
  EXTI_InitStructure.EXTI_Line=EXTI_Line12;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���	
	
	//GPIOA.5 �ж����Լ��жϳ�ʼ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);
  EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	//GPIOA.7 �ж����Լ��жϳ�ʼ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource7);
  EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	//GPIOA.11 �ж����Լ��жϳ�ʼ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);
  EXTI_InitStructure.EXTI_Line=EXTI_Line11;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
//	//GPIOA.12 �ж����Լ��жϳ�ʼ������
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
//  EXTI_InitStructure.EXTI_Line=EXTI_Line12;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	//����IO�����ж��ߵ�ӳ���ϵ
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;         //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;            //ʹ���ⲿ�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;         //��Ӧ���ȼ�2
	NVIC_Init(&NVIC_InitStruct);
	//����IO�����ж��ߵ�ӳ���ϵ
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;       //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;            //ʹ���ⲿ�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;         //��Ӧ���ȼ�2
	NVIC_Init(&NVIC_InitStruct);  
}

//�ⲿ�ж�9~5�������
void EXTI9_5_IRQHandler(void)
{			
	delay_ms(4);     //����		
	
   if(KEY5==0)	   //��/�ص��
	{
		Flag_Stop=!Flag_Stop; 
	}		
	 if(KEY7==0)	//ѡ��Ҫ����P/I/D����
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
	EXTI_ClearITPendingBit(EXTI_Line5);  //���LINE5�ϵ��жϱ�־λ 
	EXTI_ClearITPendingBit(EXTI_Line7);  //���LINE7�ϵ��жϱ�־λ 
}
//�ⲿ�ж�15~10�������
void EXTI15_10_IRQHandler(void)
{			
	delay_ms(4);   //����		
	if(Menu_MODE==1)  //PID����
	{	
			if(KEY11==0)	//PID-
			{
				      if(Menu_PID==1)  Position_KP-=Amplitude_PKP; //P��ֵһ�μ�20
				else	if(Menu_PID==2)  Position_KI-=Amplitude_PKI; //I��ֵһ�μ�0.1
				else	if(Menu_PID==3)  Position_KD-=Amplitude_PKD; //D��ֵһ�μ�25
			}		
			 if(KEY12==0)	//PID+ 
			{
							if(Menu_PID==1)  Position_KP+=Amplitude_PKP; //P��ֵһ�μ�20
				else	if(Menu_PID==2)  Position_KI+=Amplitude_PKI; //I��ֵһ�μ�0.1
				else	if(Menu_PID==3)  Position_KD+=Amplitude_PKD; //D��ֵһ�μ�25
			}		
  }
	else            //PI����
	{	
			if(KEY11==0)	//PI-
			{
				      if(Menu_PID==1)  Velocity_KP-=Amplitude_VKP; //P��ֵһ�μ�2
				else	if(Menu_PID==2)  Velocity_KI-=Amplitude_VKI; //I��ֵһ�μ�3
			}		
			 if(KEY12==0)	//PI+ 
			{
							if(Menu_PID==1)  Velocity_KP+=Amplitude_VKP; //P��ֵһ�μ�2
				else	if(Menu_PID==2)  Velocity_KI+=Amplitude_VKI; //I��ֵһ�μ�3
			}		
  }
	if(Velocity_KP<=0) Velocity_KP=0;
	if(Velocity_KI<=0) Velocity_KI=0;
	if(Position_KD<=0) Position_KD=0;
	if(Position_KP<=0) Position_KP=0;
	if(Position_KI<=0) Position_KI=0;  //������С��0
	EXTI_ClearITPendingBit(EXTI_Line11);  //���LINE11�ϵ��жϱ�־λ 
	EXTI_ClearITPendingBit(EXTI_Line12);  //���LINE12�ϵ��жϱ�־λ 

}





