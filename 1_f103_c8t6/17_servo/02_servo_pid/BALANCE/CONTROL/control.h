#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define PI 3.14159265
#define ZHONGZHI 3085
extern	int Balance_Pwm,Velocity_Pwm;
int TIM1_UP_IRQHandler(void);
void Set_Pwm(int moto);  //赋值给PWM寄存器
void Key(void);  //按键修改运行状态 
int Xianfu(int value,int Amplitude);
int myabs(int a);   //取绝对值

int Position_PID (int position,int target);


#endif
