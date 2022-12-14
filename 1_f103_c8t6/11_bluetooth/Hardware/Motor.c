#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/**
  * @brief  Motor初始化
  * @param  无
  * @retval 无
  */
void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//设置GPIOC的外设时钟
	
	//设置GPIO模式
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	PWM_Init();
}

/**
  * @brief  Motor设置速率
  * @param  Speed 要设置的速率,正数--正转;负数--反转;0--停止
  * @retval 无
  */
void Motor_SetSpeed(int8_t Speed)
{
	if (Speed > 0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		PWM_SetCompare3(Speed);	
	}
	else if (Speed == 0)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		PWM_SetCompare3(Speed);
	}
	else
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		PWM_SetCompare3(-Speed);	
	}
}

/**
  * @brief  电机开启
  * @param  无
  * @retval 无
  */
void Motor_ON(void)
{
	Motor_SetSpeed(100);
}

/**
  * @brief  电机关闭
  * @param  无
  * @retval 无
  */
void Motor_OFF(void)
{
	Motor_SetSpeed(0);
}
