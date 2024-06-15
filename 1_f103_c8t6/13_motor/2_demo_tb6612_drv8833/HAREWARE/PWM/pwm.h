#ifndef __PWM_H
#define	__PWM_H

#include "stm32f10x.h"

//DRV8833
//#define PWM_MOTORA_SetSpeed(x) TIM_SetCompare2(TIM3,x);  // PA7是T3C2,控制MOTOR_AIN1
//#define PWM_MOTORB_SetSpeed(x) TIM_SetCompare1(TIM3,x);  // PA6是T3C1,控制MOTOR_BIN1
#define PWM_SPEED_MAX 999

void TIM4_PWM_Init(u16 arr,u16 psc);
void TIM2_PWM_2ChannelInit(u16 arr, u16 psc);
void TIM3_PWM_2ChannelInit(u16 arr, u16 psc);

void PWM_InitTb6612(TIM_TypeDef* TIMx, u16 arr,u16 psc);

#endif

