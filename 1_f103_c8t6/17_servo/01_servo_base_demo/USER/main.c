#include "TIMER.h"
#include "delay.h"


/**************************************************************************
Author：平衡小车之家
A0输出PWM

**************************************************************************/

int main(void)
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
