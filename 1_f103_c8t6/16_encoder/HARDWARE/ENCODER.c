#include "ENCODER.h"

/*TIM2初始化为左轮编码器接口*/
void Encoder_Init_TIM2(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//定义一个定时器初始化的结构体
    TIM_ICInitTypeDef TIM_ICInitStructure; //定义一个定时器编码器模式初始化的结构体
	
	RCC_APB1PeriphClockCmd(ENCODERL_TIM_RCC, ENABLE); //使能TIM4时钟
	RCC_APB2PeriphClockCmd(ENCODERL_RCC, ENABLE); //使能CPIOB时钟
 
	GPIO_InitStructure.GPIO_Pin = ENCODERL_PIN;	//PB6、PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(ENCODERL_PORT, &GPIO_InitStructure);	//根据GPIO_InitStructure的参数初始化GPIOB0

	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; // 预分频器 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(ENCODERL_TIM, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct的参数初始化定时器TIM4
	
	TIM_EncoderInterfaceConfig(ENCODERL_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3：CH1、CH2同时计数，四分频
	TIM_ICStructInit(&TIM_ICInitStructure); //把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 10;  //设置滤波器长度
	TIM_ICInit(ENCODERL_TIM, &TIM_ICInitStructure); //根TIM_ICInitStructure参数初始化定时器TIM4编码器模式

    // 开启中断
    //TIM_ClearFlag(ENCODERL_TIM, TIM_FLAG_Update);//清除TIM的更新标志位
    //TIM_ITConfig(ENCODERL_TIM, TIM_IT_Update, ENABLE);
    // 开启中断
    
    //Reset counter
    TIM_SetCounter(ENCODERL_TIM,0);
	TIM_Cmd(ENCODERL_TIM, ENABLE); //使能定时器4
}

/*TIM4初始化为右轮编码器接口*/
void Encoder_Init_TIM4(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//定义一个定时器初始化的结构体
    TIM_ICInitTypeDef TIM_ICInitStructure; //定义一个定时器编码器模式初始化的结构体
	
	RCC_APB1PeriphClockCmd(ENCODERR_TIM_RCC, ENABLE); //使能TIM4时钟
	RCC_APB2PeriphClockCmd(ENCODERR_RCC, ENABLE); //使能CPIOB时钟
 
	GPIO_InitStructure.GPIO_Pin = ENCODERR_PIN;	//PB6、PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(ENCODERR_PORT, &GPIO_InitStructure);	//根据GPIO_InitStructure的参数初始化GPIOB0

	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; // 预分频器 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(ENCODERR_TIM, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct的参数初始化定时器TIM4
	
	TIM_EncoderInterfaceConfig(ENCODERR_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3：CH1、CH2同时计数，四分频
	TIM_ICStructInit(&TIM_ICInitStructure); //把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 10;  //设置滤波器长度
	TIM_ICInit(ENCODERR_TIM, &TIM_ICInitStructure); //根TIM_ICInitStructure参数初始化定时器TIM4编码器模式

    // 开启中断
    //TIM_ClearFlag(ENCODERR_TIM, TIM_FLAG_Update);//清除TIM的更新标志位
    //TIM_ITConfig(ENCODERR_TIM, TIM_IT_Update, ENABLE);
    // 开启中断

    //Reset counter
    TIM_SetCounter(ENCODERL_TIM,0);
	TIM_Cmd(ENCODERR_TIM, ENABLE); //使能定时器4
}

/********************************************
 *@Func: 电机编码器初始化
 *@Author: xxy
 *@Date: 2023.9.10
 *********************************************/
void ENCODER_Init(void)
{
    Encoder_Init_TIM2(ENCODER_TIM_PERIOD,0);
    Encoder_Init_TIM4(ENCODER_TIM_PERIOD,0);
}


//读取左轮编码器计数
int Read_Encoder_TIM2(void)
{
	int Encoder_TIM;
	Encoder_TIM = TIM2->CNT; //读取计数
	if(Encoder_TIM>0xefff)
        Encoder_TIM=Encoder_TIM-0xffff; //转化计数值为有方向的值，大于0正转，小于0反转。
	                                                      //TIM4->CNT范围为0-0xffff，初值为0。
	TIM2->CNT=0; //读取完后计数清零
	return Encoder_TIM; //返回值
}

//读取右轮编码器计数
int Read_Encoder_TIM4(void)
{
	int Encoder_TIM;
	Encoder_TIM = TIM4->CNT; //读取计数
	if(Encoder_TIM>0xefff)
        Encoder_TIM=Encoder_TIM-0xffff; //转化计数值为有方向的值，大于0正转，小于0反转。
	                                                      //TIM4->CNT范围为0-0xffff，初值为0。
	TIM4->CNT=0; //读取完后计数清零
	return Encoder_TIM; //返回值
}

/********************************************
 *@Func: 电机编码器读取计数值(轮趣教程版)
 *@Author: xxy
 *@Date: 2023.9.10
 *********************************************/
int ENCODER_ReadCnt(int encoderId)
{
    int cnt = -1;
    if (encoderId == ENCODER_ID_LEFT) {
        cnt = Read_Encoder_TIM2();
    } else if (encoderId == ENCODER_ID_RIGHT) {
        cnt = Read_Encoder_TIM4();
    } else {}
    return cnt;
}

/**************************************************************************
Function: Read encoder count per unit time(轮趣B570版)
函数功能：单位时间读取编码器计数
入口参数：TIMX：定时器
返回  值：速度值
 *@Author: 轮趣科技
**************************************************************************/
/*
int ENCODER_ReadCnt(u8 encoderId)
{
    int Encoder_TIM;    
    switch(encoderId) {
        case ENCODER_ID_LEFT:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
        //case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;	
        case ENCODER_ID_RIGHT:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;	
        default: Encoder_TIM=0;
    }
    return Encoder_TIM;
}
*/

/**************************************************************************
Function: TIM4 interrupt service function
 *@Author: 轮趣科技
**************************************************************************/
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM4->SR&=~(1<<0);//清除中断标志位 	    
}

/**************************************************************************
Function: TIM2 interrupt service function
 *@Author: 轮趣科技
**************************************************************************/
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//溢出中断
	{    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 	    
}
