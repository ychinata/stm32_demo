#ifndef __MOTO_H
#define	__MOTO_H

#include "stm32f10x.h"


//huifei-DRV8833
#define MOTOR_IN_GPIO      GPIOA
// left
#define MOTOR_PIN_AIN1 GPIO_Pin_7
#define MOTOR_PIN_AIN2 GPIO_Pin_3
//right
#define MOTOR_PIN_BIN1 GPIO_Pin_6
#define MOTOR_PIN_BIN2 GPIO_Pin_4

#define MOTOR_IN_RCC    RCC_APB2Periph_GPIOA
//huifei-DRV8833

void moto(int mode);
void MOTOR_TURN_DRV8833(int mode);

#endif
