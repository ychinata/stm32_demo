#include "stm32f10x.h"                  // Device header
#include "timer.h"

#define TIM_AUDIO_SAMPLE TIM3           // 换定时器总共要改3处

void PWM_SetCompare1(uint16_t Compare);

uint16_t g_Num = 0;
u8 g_PwmValue = 0;

/* PWM定时器(TIM2)初始化 */
//arr：自动重装值,psc：时钟预分频数
void TIM2_PwmInit(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;	
    
    //这里时钟选择为APB1的2倍,而APB1为36M.即TIM2时钟源为72M	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //使能定时器时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能GPIO外设
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //正点原子添加
    //GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE); //正点原子添加,Timer2部分重映射.增加此句,导致呼吸灯不亮
	
	//设置该引脚为复用输出功能,输出TIM2的PWM脉冲波形GPIOA.0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		//GPIO_Pin_X;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2); // 正点原子无
	
	//初始化TIM
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = arr;		//ARR,设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;		//PSC,设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 正点原子无
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
    //捕获配置
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR,设置待装入捕获比较寄存器的脉冲值
	TIM_OC1Init(TIM2, &TIM_OCInitStructure); //根据T指定的参数初始化外设TIM2 OC1  

	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //正点原子添加,使能TIM3在CCR2上的预装载寄存器
	
	TIM_Cmd(TIM2, ENABLE);
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2, Compare);
}

/*********************
 * Func.: 定时器3初始化,用于秒表
 * Author:江科大自化协
 * Date:2023.2.9
 *********************/ 
void TIM3_AudioSampleInit(u16 arr,u16 psc)//10000 - 1, 7200 - 1
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;    
    NVIC_InitTypeDef NVIC_InitStructure;    
    //这里时钟选择为APB1的2倍,而APB1为36M.即TIM2时钟源为72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_InternalClockConfig(TIM_AUDIO_SAMPLE);
	    
    // 定时器时基配置
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM_AUDIO_SAMPLE, &TIM_TimeBaseInitStructure);
	
    // 定时器中断配置
    TIM_ClearFlag(TIM_AUDIO_SAMPLE, TIM_FLAG_Update);
	TIM_ITConfig(TIM_AUDIO_SAMPLE, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM_AUDIO_SAMPLE, ENABLE);
}

/*********************
 * Func.: 定时器3中断函数,用于秒表计数
 * Author:江科大自化协
 * Date:2023.2.9
 *********************/ 
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM_AUDIO_SAMPLE, TIM_IT_Update) == SET) {
		//中断函数执行内容
        g_Num++;
        g_PwmValue += 10;
        if (g_PwmValue >= 100) {
            g_PwmValue = 0;
        }
		PWM_SetCompare1(g_PwmValue);
        //
		TIM_ClearITPendingBit(TIM_AUDIO_SAMPLE, TIM_IT_Update);
	}
}
