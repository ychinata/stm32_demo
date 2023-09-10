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
//左轮 TIM2/PA1/PA0
#define ENCODERL_TIM    TIM2
#define ENCODERL_TIM_RCC    RCC_APB1Periph_TIM2
#define ENCODERL_PORT   GPIOA
#define ENCODERL_RCC    RCC_APB2Periph_GPIOA
#define ENCODERL_PIN    GPIO_Pin_1 | GPIO_Pin_0

//右轮 TIM4/PB7/PB6
#define ENCODERR_TIM    TIM4
#define ENCODERR_TIM_RCC    RCC_APB1Periph_TIM4
#define ENCODERR_PORT   GPIOB
#define ENCODERR_RCC    RCC_APB2Periph_GPIOB
#define ENCODERR_PIN    GPIO_Pin_7 | GPIO_Pin_6
/********************************************
    编码器引脚定义
 *********************************************/
 
#define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为F103的定时器是16位的。

#define ENCODER_ID_LEFT 0
#define ENCODER_ID_RIGHT 1

// 函数声明
void ENCODER_Init(void);
int ENCODER_ReadCnt(int encoderId);

#endif
