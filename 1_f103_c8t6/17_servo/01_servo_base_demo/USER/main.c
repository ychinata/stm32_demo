#include "TIMER.h"
#include "delay.h"


/**************************************************************************
Author��ƽ��С��֮��
A0���PWM

**************************************************************************/

int main(void)
{
    delay_init();
    TIM2_PWM_Init(9999, 143); //TIM2_Int_Init(u16 arr, u16 psc)����ʼ����ʱ��TIM2
                    //��ʱʱ��=��arr+1)(psc+1)/Tclk��TclkΪ�ڲ�ͨ�ö�ʱ��ʱ�ӣ�������Ĭ������Ϊ72MHZ
    u16 PWM=750;
    u8  Direction=1;
    while(1) {
        delay_ms(100);
        if(Direction)
            PWM=PWM+50;
        else 
            PWM=PWM-50;
        if (PWM>1250) 
            Direction=0;  //PWMֵ1250������λ�ýӽ�180��
        if (PWM<250)    
            Direction=1; //PWMֵ250������λ�ýӽ�0��
        TIM_SetCompare1(TIM2, PWM); //���ô�װ�벶��ȽϼĴ���������ֵ���൱�ڲ�������TIM_Pulse
                                //Ҳ������ռ�ձȣ������PWMֵ
                                //�ڱ�������PWMֵ��Ϊһ������(20ms)������A0����ĸߵ�ƽʱ��(��λ2^-3ms)
    }
}
