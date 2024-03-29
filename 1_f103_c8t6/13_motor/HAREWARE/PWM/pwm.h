#ifndef __PWM_H
#define	__PWM_H

#include "stm32f10x.h"

//#define MOTOR_PIN_AIN1 GPIO_Pin_7//T3C2
#define PWM_MOTORA_SetSpeed(x) TIM_SetCompare2(TIM3,x);  // PA7��T3C2,����MOTOR_AIN1
//#define MOTOR_PIN_BIN1 GPIO_Pin_6//T3C1
#define PWM_MOTORB_SetSpeed(x) TIM_SetCompare1(TIM3,x);  // PA6��T3C1,����MOTOR_BIN1

#define PWM_SPEED_MAX 999

void TIM4_PWM_Init(u16 arr,u16 psc);
void TIM2_PWM_2ChannelInit(u16 arr, u16 psc);
void TIM3_PWM_2ChannelInit(u16 arr, u16 psc);

#endif

