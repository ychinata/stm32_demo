#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#define PI 3.14159265
#define ZHONGZHI 3085
extern	int Balance_Pwm,Velocity_Pwm;
int TIM1_UP_IRQHandler(void);
void Set_Pwm(int moto);  //��ֵ��PWM�Ĵ���
void Key(void);  //�����޸�����״̬ 
int Xianfu(int value,int Amplitude);
int myabs(int a);   //ȡ����ֵ

int Position_PID (int position,int target);


#endif
