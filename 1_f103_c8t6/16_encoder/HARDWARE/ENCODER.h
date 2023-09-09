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
#define ENCODER_PORT    GPIOA
#define ENCODER_RCC     RCC_APB2Periph_GPIOA
#define ENCODER_PIN     GPIO_Pin_1 | GPIO_Pin_0       


// ��������
void Encoder_Init_TIM4(u16 arr,u16 psc);
int Read_Encoder_TIM4(void);

#endif
