#include "stm32f10x.h"                  // Device header
#include "timer.h"

#define TIM_AUDIO_SAMPLE TIM3           // ����ʱ���ܹ�Ҫ��3��

void PWM_SetCompare1(uint16_t Compare);

uint16_t g_Num = 0;
u8 g_PwmValue = 0;

/* PWM��ʱ��(TIM2)��ʼ�� */
//arr���Զ���װֵ,psc��ʱ��Ԥ��Ƶ��
void TIM2_PwmInit(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;	
    
    //����ʱ��ѡ��ΪAPB1��2��,��APB1Ϊ36M.��TIM2ʱ��ԴΪ72M	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʹ�ܶ�ʱ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��GPIO����
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //����ԭ�����
    //GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE); //����ԭ�����,Timer2������ӳ��.���Ӵ˾�,���º����Ʋ���
	
	//���ø�����Ϊ�����������,���TIM2��PWM���岨��GPIOA.0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		//GPIO_Pin_X;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2); // ����ԭ����
	
	//��ʼ��TIM
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = arr;		//ARR,��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;		//PSC,����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // ����ԭ����
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
    //��������
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR,���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OC1Init(TIM2, &TIM_OCInitStructure); //����Tָ���Ĳ�����ʼ������TIM2 OC1  

	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //����ԭ�����,ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM2, ENABLE);
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2, Compare);
}

/*********************
 * Func.: ��ʱ��3��ʼ��,�������
 * Author:���ƴ��Ի�Э
 * Date:2023.2.9
 *********************/ 
void TIM3_AudioSampleInit(u16 arr,u16 psc)//10000 - 1, 7200 - 1
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;    
    NVIC_InitTypeDef NVIC_InitStructure;    
    //����ʱ��ѡ��ΪAPB1��2��,��APB1Ϊ36M.��TIM2ʱ��ԴΪ72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_InternalClockConfig(TIM_AUDIO_SAMPLE);
	    
    // ��ʱ��ʱ������
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM_AUDIO_SAMPLE, &TIM_TimeBaseInitStructure);
	
    // ��ʱ���ж�����
    TIM_ClearFlag(TIM_AUDIO_SAMPLE, TIM_FLAG_Update);
	TIM_ITConfig(TIM_AUDIO_SAMPLE, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM_AUDIO_SAMPLE, ENABLE);
}

/*********************
 * Func.: ��ʱ��3�жϺ���,����������
 * Author:���ƴ��Ի�Э
 * Date:2023.2.9
 *********************/ 
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM_AUDIO_SAMPLE, TIM_IT_Update) == SET) {
		//�жϺ���ִ������
        g_Num++;
        g_PwmValue += 10;
        if (g_PwmValue >= 100) {
            g_PwmValue = 0;
        }
		PWM_SetCompare1(g_PwmValue);
        //
		TIM_ClearITPendingBit(TIM_AUDIO_SAMPLE, TIM_IT_Update);
	}
}
