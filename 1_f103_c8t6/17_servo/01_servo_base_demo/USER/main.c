#include "TIMER.h"
#include "delay.h"


/*******************************************************************************

	主控：STM32F103C8T6 库函数版
	舵机：SG90（180度）
	说明：
		本例程是通过串口助手手动发送a b c d e分别控制-90度 -45度 0度 45度 90度转角；
		使用PB6（TIM4_CH1）做PWM输出
https://blog.csdn.net/weixin_45419341/article/details/123020872        
	
********************************************************************************/
int main(void)
{
    delay_init();
    TIM2_PWM_Init(1999,719); //TIM2_Int_Init(u16 arr, u16 psc)，初始化定时器TIM2
                    //定时时间=（arr+1)(psc+1)/Tclk，Tclk为内部通用定时器时钟，本例程默认设置为72MHZ
    
    while(1) {
        delay_ms(1000);

        //-90度
        TIM_SetCompare1(TIM2,1750);//占空比（2000-1750）/2000*20ms=2.5ms
        
        delay_ms(1000);
//        //45度
        TIM_SetCompare1(TIM2,1800);//占空比（2000-1800）/2000*20ms=2ms
//        //0度
//        TIM_SetCompare1(TIM2,1850);//占空比（2000-1850）/2000*20ms=1.5ms
//        //-45度
//        TIM_SetCompare1(TIM2,1900);//占空比（2000-1900）/2000*20ms=1ms	
//        //-90度
//        TIM_SetCompare1(TIM2,1945);//占空比（2000-1945）/2000*20ms=0.5ms          
            
    }
}

/**************************************************************************
Author：平衡小车之家
A0输出PWM

**************************************************************************/
int main2(void)
{
    delay_init();
    TIM2_PWM_Init(9999, 143); //TIM2_Int_Init(u16 arr, u16 psc)，初始化定时器TIM2
                    //定时时间=（arr+1)(psc+1)/Tclk，Tclk为内部通用定时器时钟，本例程默认设置为72MHZ
    u16 PWM=750;
    u8  Direction=1;
    while(1) {
        delay_ms(100);
        if(Direction)
            PWM=PWM+50;
        else 
            PWM=PWM-50;
        if (PWM>1250) 
            Direction=0;  //PWM值1250代表舵机位置接近180度
        if (PWM<250)    
            Direction=1; //PWM值250代表舵机位置接近0度
        TIM_SetCompare1(TIM2, PWM); //设置待装入捕获比较寄存器的脉冲值，相当于不断设置TIM_Pulse
                                //也即设置占空比，输出的PWM值
                                //在本例程中PWM值即为一个周期(20ms)内引脚A0输出的高电平时长(单位2^-3ms)
    }
}
