#include "timer.h"
#include "led.h"
#include "usart.h"

/************************************************
Date: 2022.3.8
Author: h00421956
Func:  
//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
************************************************/
void TIM3_PWM_Init(u16 arr,u16 psc) {

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	// 重映射
	// 把 TIM3_CH2 直接映射到了 PB5 上，而通过前面的学习，我们知道 PB5 和 DS0 是直接连接的
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);	
	
	//设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitTypeDef gpioInitStru;
	gpioInitStru.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	gpioInitStru.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	gpioInitStru.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInitStru);//初始化GPIO


	// 初始化TIM3
	TIM_TimeBaseInitTypeDef  timeBaseStru;
	timeBaseStru.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	timeBaseStru.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	timeBaseStru.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	timeBaseStru.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &timeBaseStru); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	// 初始化TIM3 Channel2 PWM模式
	TIM_OCInitTypeDef timOCInitStru;
	timOCInitStru.TIM_OCMode = TIM_OCMode_PWM2;
	timOCInitStru.TIM_OCPolarity = TIM_OCPolarity_High;
	timOCInitStru.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC2Init(TIM3, &timOCInitStru);
	
	// 使能TIM3在CCR2上的预装载寄存器
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	// 使能TIM3
	TIM_Cmd(TIM3, ENABLE);

}


