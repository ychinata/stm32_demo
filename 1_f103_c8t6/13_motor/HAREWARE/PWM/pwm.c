#include "pwm.h"
#include "motor.h"

/**************************************************************************
函数功能：pwm初始化-TB6612
入口参数：arr：设为一个时钟频率的最大值  psc： 预分频值
返回  值：无
Author: 轮趣
**************************************************************************/
void TIM4_PWM_Init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;                 //定义结构体GPIO_InitStructure
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       //定义结构TIM_TimeBaseStructure
    TIM_OCInitTypeDef  TIM_OCInitStructure;               //定义结构TIM_OCInitStructure

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  //使能定时器4时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能PB端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;          //PB6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //IO口速度
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     	//复用模式输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);              //GBIOB初始化  

    TIM_TimeBaseStructure.TIM_Period = arr;    //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;  //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //PWMTIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //设置TIM输出比较极性为高
    TIM_OCInitStructure.TIM_OutputState=	TIM_OutputState_Enable;   //比较输出使能

    TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据TIM_OCInitStructure中指定的参数初始化外设TIM4

    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);	    //使能预装载寄存器

    TIM_ARRPreloadConfig(TIM4, ENABLE);      //使能自动装载允许位

    TIM_Cmd(TIM4, ENABLE);   //启动定时器TIM4

}

/********************************************
* @brief      TIM2 PWM初始化
* @param      arr
*             psc
* @retval     void
* @Author: https://www.cnblogs.com/xiaohuzaixue/p/17363415.html
//PWM引脚
 *PA1-T2C2
 *PA2-T2C3
*********************************************/
void TIM2_PWM_2ChannelInit(u16 arr, u16 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure; //定义引脚初始化结构体
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue; //定义定时中断结构体	
  TIM_OCInitTypeDef TIM_OCInitTypeStrue; //定义PWM输出结构体
	
  RCC_APB2PeriphClockCmd(MOTOR_IN_RCC, ENABLE); //使能GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //使能通用定时器2时钟
	
  GPIO_InitStructure.GPIO_Pin=MOTOR_PIN_AIN1|MOTOR_PIN_BIN1;//定时器引脚PA1 CH2   PA2 CH3
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //复用推挽输出模式，A0引脚复用
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; //引脚输出速度为
  GPIO_Init(MOTOR_IN_GPIO, &GPIO_InitStructure); //初始化引脚GPIO
	 
  TIM_TimeBaseInitStrue.TIM_Period=arr; //计数周期
  TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //预分频系数
  TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
  TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //一般不使用，默认TIM_CKD_DIV1
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStrue); //初始化定时器TIM2
	
  TIM_OCInitTypeStrue.TIM_OCMode=TIM_OCMode_PWM1; //PWM模式1，定时器计数小于 TIM_Pulse时，输出有效电平
  TIM_OCInitTypeStrue.TIM_OCPolarity=TIM_OCNPolarity_High; //输出有效电平为高电平
  TIM_OCInitTypeStrue.TIM_OutputState=TIM_OutputState_Enable; //使能PWM输出
  TIM_OCInitTypeStrue.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
  TIM_OC2Init(TIM2, &TIM_OCInitTypeStrue); //初始化定时器2通道2
  
  TIM_OCInitTypeStrue.TIM_OCMode=TIM_OCMode_PWM1;//PWM模式1，时器计数小于TIM_Puls输出有效电平
  TIM_OCInitTypeStrue.TIM_OCPolarity=TIM_OCNPolarity_High; //输出有效电平为高电平
  TIM_OCInitTypeStrue.TIM_OutputState=TIM_OutputState_Enable; //使能PWM输出
  TIM_OCInitTypeStrue.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
  TIM_OC3Init(TIM2, &TIM_OCInitTypeStrue); //初始化定时器2通道3

  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable); //CH2预装载使能
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable); //CH3预装载使能
  TIM_ARRPreloadConfig(TIM2, ENABLE); //预装载使能
  TIM_Cmd(TIM2, ENABLE); //使能定时器TIM2
}

/********************************************
 *@Func: 2路PWM
 *PA6-T3C1
 *PA7-T3C2
 *@Para: speed的取值范围（0-PWM_SPEED_MAX）
 *@Ret: 
 *@Author: xxy
 *@Date: 2023.8.23
*********************************************/
void TIM3_PWM_2ChannelInit(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(MOTOR_IN_RCC, ENABLE);       //使能GPIO外设时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    //设置该引脚为复用输出功能,输出TIM3 CH1/2的PWM脉冲波形        GPIOA 6 7
    GPIO_InitStructure.GPIO_Pin = MOTOR_PIN_AIN1 | MOTOR_PIN_BIN1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_IN_GPIO, &GPIO_InitStructure);       //初始化GPIOA
    //GPIO_SetBits(GPIOA, PWM_PIN_TIM3_A);

    //GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);//改变指定管脚的映射   

    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    //初始化TIM1 Channel 1~2 PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//TIM_OCPolarity_Low //输出极性:TIM输出比较极性高
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
    TIM_ARRPreloadConfig(TIM3,ENABLE);//使能预装载寄存器

    TIM_Cmd(TIM3, ENABLE);  //使能TIM3
}

