#include "moto.h"


/**************************************************************************
�������ܣ����������ת
��ڲ�����mode   mode=0ʱΪ��ת  mode=1ʱ��ת
����  ֵ����
**************************************************************************/


//void moto(int mode)
//{
//	if(mode==1)
//	{
//	 GPIO_SetBits(GPIOB, GPIO_Pin_7);	 // �ߵ�ƽ
//	 GPIO_ResetBits(GPIOB, GPIO_Pin_8);	 // �͵�ƽ}
//	}
//	 if(mode==0)
//	{
//	 GPIO_SetBits(GPIOB, GPIO_Pin_8);	 // �ߵ�ƽ
//	 GPIO_ResetBits(GPIOB, GPIO_Pin_7);	 // �͵�ƽ}
//	 }
// 
//}

//huifei
// drv8833û��PWM����
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

