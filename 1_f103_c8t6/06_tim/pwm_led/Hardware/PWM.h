#ifndef __PWM_H
#define __PWM_H

void PWM_Init(void);
void PWM_SetCompare1(uint16_t Compare);

void TIM2_PWM_Init(u16 arr,u16 psc);

#endif
