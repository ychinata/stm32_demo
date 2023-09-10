#ifndef __TIMER_H
#define __TIMER_H	
#include "sys.h"
#include "stm32f10x_tim.h"


/********************************************
    ���������Ŷ���
 *********************************************/
/* 
MG310�����������
M-
VCC
A�� <-> PA1
B�� <-> PA0
GND
M+
 */
//���� TIM2/PA1/PA0
#define ENCODERL_TIM    TIM2
#define ENCODERL_TIM_RCC    RCC_APB1Periph_TIM2
#define ENCODERL_PORT   GPIOA
#define ENCODERL_RCC    RCC_APB2Periph_GPIOA
#define ENCODERL_PIN    GPIO_Pin_1 | GPIO_Pin_0

//���� TIM4/PB7/PB6
#define ENCODERR_TIM    TIM4
#define ENCODERR_TIM_RCC    RCC_APB1Periph_TIM4
#define ENCODERR_PORT   GPIOB
#define ENCODERR_RCC    RCC_APB2Periph_GPIOB
#define ENCODERR_PIN    GPIO_Pin_7 | GPIO_Pin_6
/********************************************
    ���������Ŷ���
 *********************************************/
 
#define ENCODER_TIM_PERIOD (u16)(65535)   //���ɴ���65535 ��ΪF103�Ķ�ʱ����16λ�ġ�

#define ENCODER_ID_LEFT 0
#define ENCODER_ID_RIGHT 1

// ��������
void ENCODER_Init(void);
int ENCODER_ReadCnt(int encoderId);

#endif
