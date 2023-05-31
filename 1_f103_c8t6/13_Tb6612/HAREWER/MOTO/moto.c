#include "moto.h"


/**************************************************************************
函数功能：电机的正反转
入口参数：mode   mode=0时为正转  mode=1时反转
返回  值：无
**************************************************************************/


//void moto(int mode)
//{
//	if(mode==1)
//	{
//	 GPIO_SetBits(GPIOB, GPIO_Pin_7);	 // 高电平
//	 GPIO_ResetBits(GPIOB, GPIO_Pin_8);	 // 低电平}
//	}
//	 if(mode==0)
//	{
//	 GPIO_SetBits(GPIOB, GPIO_Pin_8);	 // 高电平
//	 GPIO_ResetBits(GPIOB, GPIO_Pin_7);	 // 低电平}
//	 }
// 
//}

//huifei
// drv8833没有PWM引脚
void MOTOR_TURN_DRV8833(int mode)
{    
    // 左右轮要反转
    if (mode==1) {
        // left wheel
        GPIO_SetBits(MOTOR_IN_GPIO, MOTOR_PIN_AIN1);	 // 高电平
        GPIO_ResetBits(MOTOR_IN_GPIO, MOTOR_PIN_AIN2);	 // 低电平
        // right wheel
        GPIO_SetBits(MOTOR_IN_GPIO, MOTOR_PIN_BIN2);	 // 高电平
        GPIO_ResetBits(MOTOR_IN_GPIO, MOTOR_PIN_BIN1);	 // 低电平                       
    }
    if (mode==0) {
        // left wheel
        GPIO_SetBits(MOTOR_IN_GPIO, MOTOR_PIN_AIN2);	 // 高电平
        GPIO_ResetBits(MOTOR_IN_GPIO, MOTOR_PIN_AIN1);	 // 低电平
        // right wheel
        GPIO_SetBits(MOTOR_IN_GPIO, MOTOR_PIN_BIN1);	 // 高电平
        GPIO_ResetBits(MOTOR_IN_GPIO, MOTOR_PIN_BIN2);	 // 低电平        
    }         
}

