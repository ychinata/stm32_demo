#ifndef __MOTO_H
#define __MOTO_H
#include "sys.h" 
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define PWMA   TIM3->CCR4    //PID套件仅使用一路驱动 使用的A路，对应PB0引脚
#define AIN2   PBout(12)     //PID套件仅使用一路驱动 使用的A路
#define AIN1   PBout(13)     //PID套件仅使用一路驱动 使用的A路
#define BIN1   PBout(14)     //预留 备用
#define BIN2   PBout(15)     //预留 备用
#define PWMB   TIM3->CCR3    //预留 备用  ，对应PB1引脚
void Servo_Init(u16 arr,u16 psc); //PWM输出初始化
void MiniBalance_Motor_Init(void);          //初始化控制电机所需的引脚
#endif
