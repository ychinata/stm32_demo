#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

// 2022.3.8

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);

#endif
