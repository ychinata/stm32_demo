#include "ENCODER.h"

/*TIM2��ʼ��Ϊ���ֱ������ӿ�*/
void Encoder_Init_TIM2(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����һ�����ų�ʼ���Ľṹ��  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//����һ����ʱ����ʼ���Ľṹ��
    TIM_ICInitTypeDef TIM_ICInitStructure; //����һ����ʱ��������ģʽ��ʼ���Ľṹ��
	
	RCC_APB1PeriphClockCmd(ENCODERL_TIM_RCC, ENABLE); //ʹ��TIM4ʱ��
	RCC_APB2PeriphClockCmd(ENCODERL_RCC, ENABLE); //ʹ��CPIOBʱ��
 
	GPIO_InitStructure.GPIO_Pin = ENCODERL_PIN;	//PB6��PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(ENCODERL_PORT, &GPIO_InitStructure);	//����GPIO_InitStructure�Ĳ�����ʼ��GPIOB0

	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc; // Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(ENCODERL_TIM, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct�Ĳ�����ʼ����ʱ��TIM4
	
	TIM_EncoderInterfaceConfig(ENCODERL_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3��CH1��CH2ͬʱ�������ķ�Ƶ
	TIM_ICStructInit(&TIM_ICInitStructure); //��TIM_ICInitStruct �е�ÿһ��������ȱʡֵ����
	TIM_ICInitStructure.TIM_ICFilter = 10;  //�����˲�������
	TIM_ICInit(ENCODERL_TIM, &TIM_ICInitStructure); //��TIM_ICInitStructure������ʼ����ʱ��TIM4������ģʽ

    // �����ж�
    //TIM_ClearFlag(ENCODERL_TIM, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
    //TIM_ITConfig(ENCODERL_TIM, TIM_IT_Update, ENABLE);
    // �����ж�
    
    //Reset counter
    TIM_SetCounter(ENCODERL_TIM,0);
	TIM_Cmd(ENCODERL_TIM, ENABLE); //ʹ�ܶ�ʱ��4
}

/*TIM4��ʼ��Ϊ���ֱ������ӿ�*/
void Encoder_Init_TIM4(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����һ�����ų�ʼ���Ľṹ��  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//����һ����ʱ����ʼ���Ľṹ��
    TIM_ICInitTypeDef TIM_ICInitStructure; //����һ����ʱ��������ģʽ��ʼ���Ľṹ��
	
	RCC_APB1PeriphClockCmd(ENCODERR_TIM_RCC, ENABLE); //ʹ��TIM4ʱ��
	RCC_APB2PeriphClockCmd(ENCODERR_RCC, ENABLE); //ʹ��CPIOBʱ��
 
	GPIO_InitStructure.GPIO_Pin = ENCODERR_PIN;	//PB6��PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(ENCODERR_PORT, &GPIO_InitStructure);	//����GPIO_InitStructure�Ĳ�����ʼ��GPIOB0

	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc; // Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(ENCODERR_TIM, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct�Ĳ�����ʼ����ʱ��TIM4
	
	TIM_EncoderInterfaceConfig(ENCODERR_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3��CH1��CH2ͬʱ�������ķ�Ƶ
	TIM_ICStructInit(&TIM_ICInitStructure); //��TIM_ICInitStruct �е�ÿһ��������ȱʡֵ����
	TIM_ICInitStructure.TIM_ICFilter = 10;  //�����˲�������
	TIM_ICInit(ENCODERR_TIM, &TIM_ICInitStructure); //��TIM_ICInitStructure������ʼ����ʱ��TIM4������ģʽ

    // �����ж�
    //TIM_ClearFlag(ENCODERR_TIM, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
    //TIM_ITConfig(ENCODERR_TIM, TIM_IT_Update, ENABLE);
    // �����ж�

    //Reset counter
    TIM_SetCounter(ENCODERL_TIM,0);
	TIM_Cmd(ENCODERR_TIM, ENABLE); //ʹ�ܶ�ʱ��4
}

/********************************************
 *@Func: �����������ʼ��
 *@Author: xxy
 *@Date: 2023.9.10
 *********************************************/
void ENCODER_Init(void)
{
    Encoder_Init_TIM2(ENCODER_TIM_PERIOD,0);
    Encoder_Init_TIM4(ENCODER_TIM_PERIOD,0);
}


//��ȡ���ֱ���������
int Read_Encoder_TIM2(void)
{
	int Encoder_TIM;
	Encoder_TIM = TIM2->CNT; //��ȡ����
	if(Encoder_TIM>0xefff)
        Encoder_TIM=Encoder_TIM-0xffff; //ת������ֵΪ�з����ֵ������0��ת��С��0��ת��
	                                                      //TIM4->CNT��ΧΪ0-0xffff����ֵΪ0��
	TIM2->CNT=0; //��ȡ����������
	return Encoder_TIM; //����ֵ
}

//��ȡ���ֱ���������
int Read_Encoder_TIM4(void)
{
	int Encoder_TIM;
	Encoder_TIM = TIM4->CNT; //��ȡ����
	if(Encoder_TIM>0xefff)
        Encoder_TIM=Encoder_TIM-0xffff; //ת������ֵΪ�з����ֵ������0��ת��С��0��ת��
	                                                      //TIM4->CNT��ΧΪ0-0xffff����ֵΪ0��
	TIM4->CNT=0; //��ȡ����������
	return Encoder_TIM; //����ֵ
}

/********************************************
 *@Func: �����������ȡ����ֵ(��Ȥ�̳̰�)
 *@Author: xxy
 *@Date: 2023.9.10
 *********************************************/
int ENCODER_ReadCnt(int encoderId)
{
    int cnt = -1;
    if (encoderId == ENCODER_ID_LEFT) {
        cnt = Read_Encoder_TIM2();
    } else if (encoderId == ENCODER_ID_RIGHT) {
        cnt = Read_Encoder_TIM4();
    } else {}
    return cnt;
}

/**************************************************************************
Function: Read encoder count per unit time(��ȤB570��)
�������ܣ���λʱ���ȡ����������
��ڲ�����TIMX����ʱ��
����  ֵ���ٶ�ֵ
 *@Author: ��Ȥ�Ƽ�
**************************************************************************/
/*
int ENCODER_ReadCnt(u8 encoderId)
{
    int Encoder_TIM;    
    switch(encoderId) {
        case ENCODER_ID_LEFT:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
        //case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;	
        case ENCODER_ID_RIGHT:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;	
        default: Encoder_TIM=0;
    }
    return Encoder_TIM;
}
*/

/**************************************************************************
Function: TIM4 interrupt service function
 *@Author: ��Ȥ�Ƽ�
**************************************************************************/
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//����ж�
	{    				   				     	    	
	}				   
	TIM4->SR&=~(1<<0);//����жϱ�־λ 	    
}

/**************************************************************************
Function: TIM2 interrupt service function
 *@Author: ��Ȥ�Ƽ�
**************************************************************************/
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//����ж�
	{    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//����жϱ�־λ 	    
}
