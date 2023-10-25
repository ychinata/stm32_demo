#include "encoder.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
�������ܣ���TIM2��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʹ�ܶ�ʱ��2��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PA�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//�˿����� PA0 PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOA

	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;  //��ʱ��2�ж�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  //ʹ��IRQͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�1 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;       //��Ӧ���ȼ�3
	NVIC_Init(&NVIC_InitStruct);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ؼ���ģʽ 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);  //��ʼ����ʱ��

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3

	TIM_ICStructInit(&TIM_ICInitStructure); //��TIM_ICInitStruct �е�ÿһ��������ȱʡֵ����
	TIM_ICInitStructure.TIM_ICFilter = 10;  //�����˲�������
	TIM_ICInit(TIM2, &TIM_ICInitStructure);//���� TIM_ICInitStruct �Ĳ�����ʼ������	TIMx

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//ʹ�ܶ�ʱ���ж�
	TIM_SetCounter(TIM2,0);
	TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��

}
/**************************************************************************
�������ܣ���TIM4��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM4(void)
	{
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//ʹ�ܶ�ʱ��4��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PB�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//�˿����� PB6 PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);					      //�����趨������ʼ��GPIOB  

	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;             //TIM4�ж�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             //ʹ��IRQͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ� 1
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;          //��Ӧ���ȼ�3       
	NVIC_Init(&NVIC_InitStruct);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ؼ���ģʽ 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);  //��ʼ����ʱ��

	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3

	TIM_ICStructInit(&TIM_ICInitStructure); //��TIM_ICInitStruct �е�ÿһ��������ȱʡֵ����
	TIM_ICInitStructure.TIM_ICFilter = 10;  //�����˲�������
	TIM_ICInit(TIM4, &TIM_ICInitStructure);//���� TIM_ICInitStruct �Ĳ�����ʼ������	TIMx

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//ʹ�ܶ�ʱ���ж�

	TIM_SetCounter(TIM4,10000);//λ��ģʽ��ʼ����λ��Ϊ10000

	TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ4
}
/**************************************************************************
�������ܣ���λʱ���ȡ����������
��ڲ�������ʱ��
����  ֵ���ٶ�ֵ
**************************************************************************/
int Read_Velocity(u8 TIMX)
{
    int Encoder_TIM;    
   switch(TIMX)
	 {
		case 2:  Encoder_TIM= (short)TIM2 -> CNT; 	TIM2->CNT=0; break;
		case 3:  Encoder_TIM= (short)TIM3 -> CNT; 	TIM3->CNT=0; break;	
		case 4:  Encoder_TIM= (short)TIM4 -> CNT;  	TIM4->CNT=0; break;	
		default: Encoder_TIM=0;
	 }
		return Encoder_TIM;
}
/**************************************************************************
�������ܣ���ȡλ����Ϣ
��ڲ�������ʱ��
����  ֵ��λ��ֵ
**************************************************************************/
int Read_Position(u8 TIMX)
{
    int Encoder_TIM;    
   switch(TIMX)
	 {
		case 2:  Encoder_TIM= (short)TIM2 -> CNT;  break;
		case 3:  Encoder_TIM= (short)TIM3 -> CNT;  break;	
		case 4:  Encoder_TIM= (short)TIM4 -> CNT;  break;	
		default: Encoder_TIM=0;
	 }
		return Encoder_TIM;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void TIM4_IRQHandler(void)//�жϴ�����Ϊ�գ�����жϱ�־λ������ж�
{ 		    		  			    
 if(TIM_GetFlagStatus(TIM4,TIM_FLAG_Update)==SET)//����ж�
 {
	 
 } 
 TIM_ClearITPendingBit(TIM4,TIM_IT_Update); //����жϱ�־λ 	
}


void TIM2_IRQHandler(void)//�жϴ�����Ϊ�գ�����жϱ�־λ������ж�
{ 		    		  			    
 if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)==SET)//����ж�
 {
	 
 } 
 TIM_ClearITPendingBit(TIM2,TIM_IT_Update); //����жϱ�־λ 	  
}
