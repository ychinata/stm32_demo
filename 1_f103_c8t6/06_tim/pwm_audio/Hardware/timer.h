#ifndef __TIMER_H
#define __TIMER_H

#include "timer.h"

extern uint16_t g_Num;

void PWM_SetCompare1(uint16_t Compare);

void TIM2_PwmInit(u16 arr,u16 psc);
void TIM3_AudioSampleInit(u16 arr,u16 psc);
    
#endif
