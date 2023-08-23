#include "moto.h"
#include "pwm.h"

/********************************************
* @brief      ���GPIO��ʼ��DRV8833
              //AIN2/BIN2��Ϊ�������
* @param      void
* @retval     void
*********************************************/
void MOTOR_DRV8833Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(MOTOR_IN_RCC,ENABLE);

    GPIO_InitStructure.GPIO_Pin=MOTOR_PIN_AIN2|MOTOR_PIN_BIN2;//AIN2/BIN2��Ϊ�������
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_IN_GPIO,&GPIO_InitStructure);
    GPIO_ResetBits(MOTOR_IN_GPIO, MOTOR_PIN_AIN2|MOTOR_PIN_BIN2);
    //��ʱ��3��ʼ����AIN1/BIN1
    TIM3_PWM_2ChannelInit(PWM_SPEED_MAX, 72-1);
}


/**************************************************************************
�������ܣ����������ת
��ڲ�����mode   mode=0ʱΪ��ת  mode=1ʱ��ת
 *@Author: ��Ȥ
**************************************************************************/
void MOTOR_TURN_TB6612(int mode)
{
	if(mode==1)
	{
	 GPIO_SetBits(GPIOB, GPIO_Pin_7);	 // �ߵ�ƽ
	 GPIO_ResetBits(GPIOB, GPIO_Pin_8);	 // �͵�ƽ}
	}
	 if(mode==0)
	{
	 GPIO_SetBits(GPIOB, GPIO_Pin_8);	 // �ߵ�ƽ
	 GPIO_ResetBits(GPIOB, GPIO_Pin_7);	 // �͵�ƽ}
	 }
 
}

/********************************************
 *@Func: drv8833û��PWM����,�˺���ֻ��������ת,û�е���
 *@Para: 
 *@Ret: 
 *@Author: xxy
 *@Date: 2023.
*********************************************/
void MOTOR_TURN_DRV8833(int mode)
{    
    // ������Ҫ��ת
    if (mode==1) {
        // left wheel
        GPIO_SetBits(MOTOR_IN_GPIO, MOTOR_PIN_AIN1);	 // �ߵ�ƽ
        GPIO_ResetBits(MOTOR_IN_GPIO, MOTOR_PIN_AIN2);	 // �͵�ƽ
        // right wheel
        GPIO_SetBits(MOTOR_IN_GPIO, MOTOR_PIN_BIN2);	 // �ߵ�ƽ
        GPIO_ResetBits(MOTOR_IN_GPIO, MOTOR_PIN_BIN1);	 // �͵�ƽ                       
    }
    if (mode==0) {
        // left wheel
        GPIO_SetBits(MOTOR_IN_GPIO, MOTOR_PIN_AIN2);	 // �ߵ�ƽ
        GPIO_ResetBits(MOTOR_IN_GPIO, MOTOR_PIN_AIN1);	 // �͵�ƽ
        // right wheel
        GPIO_SetBits(MOTOR_IN_GPIO, MOTOR_PIN_BIN1);	 // �ߵ�ƽ
        GPIO_ResetBits(MOTOR_IN_GPIO, MOTOR_PIN_BIN2);	 // �͵�ƽ        
    }         
}

/********************************************
 *@Func: DRV8833����С��ǰ��/���ˣ�ʹ��2·PWM��Ҳ����ʹ��4·PWM��Ҫ����ʵ�֣�
 *@Para: speed��ȡֵ��Χ��0-PWM_SPEED_MAX��
 *@Ret: 
 *@Author: xxy
 *@Date: 2023.8.23
*********************************************/
void CAR_Run_DRV8833With2PWM(int carRunMode, int speed)
{
    // С��ǰ�������ַ�ת����ת��ת
    if (carRunMode == CAR_RUN_MOED_FORWARD) {    
        // AIN1-PWM, BIN2-1:��ת,ռ�ձ�ԽС,ת��Խ��
        PWM_MOTORA_SetSpeed(PWM_SPEED_MAX-speed);
        GPIO_SetBits(MOTOR_IN_GPIO, MOTOR_PIN_AIN2);    
        // BIN1-PWM, BIN2-0:��ת,ռ�ձ�Խ��,ת��Խ��
        PWM_MOTORB_SetSpeed(speed);
        GPIO_ResetBits(MOTOR_IN_GPIO, MOTOR_PIN_BIN2);            
    } else { // С�����ˣ�������ת����ת��ת
        // AIN1-PWM, BIN2-0:��ת,ռ�ձ�Խ��,ת��Խ��
        PWM_MOTORA_SetSpeed(speed);
        GPIO_ResetBits(MOTOR_IN_GPIO, MOTOR_PIN_AIN2);                           
        // BIN1-PWM, BIN2-1:��ת,ռ�ձ�ԽС,ת��Խ��
        PWM_MOTORB_SetSpeed(PWM_SPEED_MAX-speed);
        GPIO_SetBits(MOTOR_IN_GPIO, MOTOR_PIN_BIN2);      
    }
}

