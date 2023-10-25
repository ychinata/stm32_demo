#include "adc.h"
 /**************************************************************************
���ߣ�ƽ��С��֮�� 
�Ա����̣�http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
�������ܣ�ADC��ʼ����ص�ѹ���
��ڲ�������
����  ֵ����
��    �ߣ�ƽ��С��֮��
**************************************************************************/
void  Baterry_Adc_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE ); //ʹ��GPIOAʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;        //PA6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE );  //ʹ��ADC1ʱ��
  ADC_DeInit(ADC1);        //ADC1��λ
	
 	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//��ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}		

/**************************************************************************
�������ܣ�AD����
��ڲ�����ADC1 ��ͨ��
����  ֵ��ADת�����
��    �ߣ�ƽ��С��֮��
**************************************************************************/
u16 Get_Adc(u8 ch)   
{
	
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
	
}

/**************************************************************************
�������ܣ���ȡ��ص�ѹ 
��ڲ�������
����  ֵ����ص�ѹ ��λMV
��    �ߣ�ƽ��С��֮��
**************************************************************************/
int Get_battery_volt(void)   
{  
	int Volt;                                          //��ص�ѹ
	Volt=Get_Adc(Battery_Ch)*3.3*11*100/1.0/4096;		 //�����ѹ���������ԭ��ͼ�򵥷������Եõ�
	return Volt;
}

///**************************************************************************
//�������ܣ���λ�ƴ�������ʼ��
//��ڲ�������
//����  ֵ����
//��    �ߣ�ƽ��С��֮��
//**************************************************************************/
//void  Angle_Adc_Init(void)
//{    
////�ȳ�ʼ��IO��
// 	RCC->APB2ENR|=1<<2;    //ʹ��PORTA��ʱ�� 
//	GPIOA->CRL&=0X0FFFFFFF;//PA7 anolog���� 	 		 
//	RCC->APB2ENR|=1<<9;    //ADC1ʱ��ʹ��	  
//	RCC->APB2RSTR|=1<<9;   //ADC1��λ
//	RCC->APB2RSTR&=~(1<<9);//��λ����	    
//	RCC->CFGR&=~(3<<14);   //��Ƶ��������	
//	//SYSCLK/DIV2=12M ADCʱ������Ϊ12M,ADC���ʱ�Ӳ��ܳ���14M!
//	//���򽫵���ADC׼ȷ���½�! 
//	RCC->CFGR|=2<<14;      	 

//	ADC1->CR1&=0XF0FFFF;   //����ģʽ����
//	ADC1->CR1|=0<<16;      //��������ģʽ  
//	ADC1->CR1&=~(1<<8);    //��ɨ��ģʽ	  
//	ADC1->CR2&=~(1<<1);    //����ת��ģʽ
//	ADC1->CR2&=~(7<<17);	   
//	ADC1->CR2|=7<<17;	   //�������ת��  
//	ADC1->CR2|=1<<20;      //ʹ�����ⲿ����(SWSTART)!!!	����ʹ��һ���¼�������
//	ADC1->CR2&=~(1<<11);   //�Ҷ���	 
//	ADC1->SQR1&=~(0XF<<20);
//	ADC1->SQR1&=0<<20;     //1��ת���ڹ��������� Ҳ����ֻת����������1 			   
//	//����ͨ��7�Ĳ���ʱ��
//	ADC1->SMPR2&=0X0FFFFFFF;//ͨ��7����ʱ�����	  
//	ADC1->SMPR2|=7<<21;      //ͨ��7  239.5����,��߲���ʱ�������߾�ȷ��	 

//	ADC1->CR2|=1<<0;	    //����ADת����	 
//	ADC1->CR2|=1<<3;        //ʹ�ܸ�λУ׼  
//	while(ADC1->CR2&1<<3);  //�ȴ�У׼���� 			 
//    //��λ��������ò���Ӳ���������У׼�Ĵ�������ʼ�����λ��������� 		 
//	ADC1->CR2|=1<<2;        //����ADУ׼	   
//	while(ADC1->CR2&1<<2);  //�ȴ�У׼����
//	delay_ms(1);
//}		

//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
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
	 








