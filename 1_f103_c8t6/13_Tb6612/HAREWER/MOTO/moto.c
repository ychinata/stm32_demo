#include "moto.h"
#include "pwm.h"

/********************************************
* @brief      电机GPIO初始化DRV8833
              //AIN2/BIN2作为方向控制
* @param      void
* @retval     void
*********************************************/
void MOTOR_DRV8833Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(MOTOR_IN_RCC,ENABLE);

    GPIO_InitStructure.GPIO_Pin=MOTOR_PIN_AIN2|MOTOR_PIN_BIN2;//AIN2/BIN2作为方向控制
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_IN_GPIO,&GPIO_InitStructure);
    GPIO_ResetBits(MOTOR_IN_GPIO, MOTOR_PIN_AIN2|MOTOR_PIN_BIN2);
    //定时器3初始化：AIN1/BIN1
    TIM3_PWM_2ChannelInit(PWM_SPEED_MAX, 72-1);
}


/**************************************************************************
函数功能：电机的正反转
入口参数：mode   mode=0时为正转  mode=1时反转
 *@Author: 轮趣
**************************************************************************/
void MOTOR_TURN_TB6612(int mode)
{
	if(mode==1)
	{
	 GPIO_SetBits(GPIOB, GPIO_Pin_7);	 // 高电平
	 GPIO_ResetBits(GPIOB, GPIO_Pin_8);	 // 低电平}
	}
	 if(mode==0)
	{
	 GPIO_SetBits(GPIOB, GPIO_Pin_8);	 // 高电平
	 GPIO_ResetBits(GPIOB, GPIO_Pin_7);	 // 低电平}
	 }
 
}

/********************************************
 *@Func: drv8833没有PWM引脚,此函数只控制正反转,没有调速
 *@Para: 
 *@Ret: 
 *@Author: xxy
 *@Date: 2023.
*********************************************/
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

/********************************************
 *@Func: DRV8833驱动小车前进/后退，使用2路PWM（也可以使用4路PWM，要另外实现）
 *@Para: speed的取值范围（0-PWM_SPEED_MAX）
 *@Ret: 
 *@Author: xxy
 *@Date: 2023.8.23
*********************************************/
void CAR_Run_DRV8833With2PWM(int carRunMode, int speed)
{
    // 小车前进：左轮反转，右转正转
    if (carRunMode == CAR_RUN_MOED_FORWARD) {    
        // AIN1-PWM, BIN2-1:反转,占空比越小,转速越快
        PWM_MOTORA_SetSpeed(PWM_SPEED_MAX-speed);
        GPIO_SetBits(MOTOR_IN_GPIO, MOTOR_PIN_AIN2);    
        // BIN1-PWM, BIN2-0:正转,占空比越大,转速越快
        PWM_MOTORB_SetSpeed(speed);
        GPIO_ResetBits(MOTOR_IN_GPIO, MOTOR_PIN_BIN2);            
    } else { // 小车后退：左轮正转，右转反转
        // AIN1-PWM, BIN2-0:正转,占空比越大,转速越快
        PWM_MOTORA_SetSpeed(speed);
        GPIO_ResetBits(MOTOR_IN_GPIO, MOTOR_PIN_AIN2);                           
        // BIN1-PWM, BIN2-1:反转,占空比越小,转速越快
        PWM_MOTORB_SetSpeed(PWM_SPEED_MAX-speed);
        GPIO_SetBits(MOTOR_IN_GPIO, MOTOR_PIN_BIN2);      
    }
}

