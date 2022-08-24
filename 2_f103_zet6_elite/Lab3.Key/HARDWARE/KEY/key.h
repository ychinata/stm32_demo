#ifndef __KEY_H__
#define __KEY_H__
#include "sys.h"

#define KEY0 GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)
#define KEY1 GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)
#define WK_UP GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)

#define KEY0_PRES 1
#define KEY1_PRES 2
#define WKUP_PRES 3

#define KEY_MODE_NOT_SPT_CONTI 0
#define KEY_MODE_SPT_CONTI 1

void KEY_Init(void);
u8 KEY_Scan(u8);

#endif
