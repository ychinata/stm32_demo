#ifndef __MOTO_H
#define	__MOTO_H

#include "stm32f10x.h"


//huifei-DRV8833
#define MOTOR_IN_GPIO      GPIOA
// left
#define MOTOR_PIN_AIN1 GPIO_Pin_7//T3C2
#define MOTOR_PIN_AIN2 GPIO_Pin_3
//right
#define MOTOR_PIN_BIN1 GPIO_Pin_6//T3C1
#define MOTOR_PIN_BIN2 GPIO_Pin_4

#define MOTOR_IN_RCC    RCC_APB2Periph_GPIOA
//huifei-DRV8833

//
#define MOTOR_TURN_MODE_FORWARD  0
#define MOTOR_TURN_MODE_REVERSAL 1
//
#define CAR_RUN_MOED_FORWARD  0
#define CAR_RUN_MOED_BACKWARD 1

void MOTOR_TURN_TB6612(int mode);
void MOTOR_TURN_DRV8833(int mode);
void MOTOR_DRV8833Init(void);

void CAR_Run_DRV8833With2PWM(int carRunMode, int speed);

#endif
