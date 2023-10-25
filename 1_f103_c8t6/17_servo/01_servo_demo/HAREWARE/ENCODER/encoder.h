#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#define ENCODER_TIM_PERIOD (u16)(65535)   //���ɴ���65535 ��ΪF103�Ķ�ʱ����16λ�ġ�
void Encoder_Init_TIM2(void);             //��TIM2��ʼ��Ϊ�������ӿ�ģʽ
void Encoder_Init_TIM4(void);             //��TIM4��ʼ��Ϊ�������ӿ�ģʽ
int Read_Position(u8 TIMX);               //��ȡλ����Ϣ
int Read_Velocity(u8 TIMX);               //��λʱ���ȡ����������
void TIM4_IRQHandler(void);               //��ʱ��4���жϴ�����
void TIM2_IRQHandler(void);               //��ʱ��2���жϴ�����
#endif
