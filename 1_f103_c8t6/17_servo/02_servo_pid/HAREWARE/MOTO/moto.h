#ifndef __MOTO_H
#define __MOTO_H
#include "sys.h" 
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#define PWMA   TIM3->CCR4    //PID�׼���ʹ��һ·���� ʹ�õ�A·����ӦPB0����
#define AIN2   PBout(12)     //PID�׼���ʹ��һ·���� ʹ�õ�A·
#define AIN1   PBout(13)     //PID�׼���ʹ��һ·���� ʹ�õ�A·
#define BIN1   PBout(14)     //Ԥ�� ����
#define BIN2   PBout(15)     //Ԥ�� ����
#define PWMB   TIM3->CCR3    //Ԥ�� ����  ����ӦPB1����
void Servo_Init(u16 arr,u16 psc); //PWM�����ʼ��
void MiniBalance_Motor_Init(void);          //��ʼ�����Ƶ�����������
#endif
