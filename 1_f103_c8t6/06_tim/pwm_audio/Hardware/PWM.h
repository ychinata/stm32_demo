#ifndef __PWM_H
#define __PWM_H

extern uint16_t g_Num;

void PWM_SetCompare1(uint16_t Compare);

void TIM2_PwmInit(u16 arr,u16 psc);
void TIM3_TimerInit(u16 arr,u16 psc);
    
#endif
