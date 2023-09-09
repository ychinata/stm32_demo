#ifndef __TIMER_H
#define __TIMER_H	
#include "sys.h"
#include "stm32f10x_tim.h"


/********************************************
    编码器引脚定义
 *********************************************/
/* 
MG310电机编码器：
M-
VCC
A相 <-> PA1
B相 <-> PA0
GND
M+
 */
 //TIM4
#define ENCODER_PORT    GPIOB
#define ENCODER_RCC     RCC_APB2Periph_GPIOB
#define ENCODER_PIN     GPIO_Pin_7 | GPIO_Pin_6


// 函数声明
void Encoder_Init_TIM4(u16 arr,u16 psc);
int Read_Encoder_TIM4(void);

#endif
