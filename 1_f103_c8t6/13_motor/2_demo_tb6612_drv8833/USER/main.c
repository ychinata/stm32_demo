#include "stm32f10x.h"
#include "delay.h"
#include "gpio.h"
#include "motor.h"
#include "oled.h"
#include "pwm.h"

/***********************************************
Author����Ȥ�Ƽ�����ݸ�����޹�˾
Version��5.7
Date��2021-04-29
***********************************************/
int TEST_TB6612(void);
int TEST_DRV8833(void);
void TEST_DRV8833_ErgodicVelocity(void);
void TEST_DRV8833_RandomWalk(void);

/********************************************
 *@Func: motor������
 *@Author: xxy
 *@Date: 2023.8.23
*********************************************/
int main(void)
{
    TEST_TB6612();
    //TEST_DRV8833();
}

//TB6612
//Author:��Ȥ
int TEST_TB6612(void)
{	    
    int pwmvalue = 2000;
    static int  delaymsGap = 500;    
    
    SystemInit(); //����ϵͳʱ��Ϊ72M   
    delay_init();    //��ʱ������ʼ��
    MOTOR_Init();
    OLED_Init();   

    OLED_ShowString(1, 1, "TB6612");    
    OLED_ShowNum(2, 1, pwmvalue, 4);
    OLED_ShowString(4, 1, "2024.6.15");  

    while(1) {       
        MOTOR_TURN_TB6612(0);                //moto=1ʱ��ת
        PWM_MOTORA_SetSpeed(pwmvalue);
        delay_ms(delaymsGap);
        MOTOR_TURN_TB6612(1);                //moto=0ʱ��ת
        PWM_MOTORA_SetSpeed(pwmvalue);
        delay_ms(delaymsGap);
    }
}

/********************************************
 *@Func: DRV8833����С��ǰ��/����
 *@Author: xxy
 *@Date: 2023.8.23
*********************************************/
int TEST_DRV8833(void)
{     
    SystemInit(); //����ϵͳʱ��Ϊ72M   
    delay_init();    //��ʱ������ʼ��            
    MOTOR_Init();  
    OLED_Init();
    
    OLED_ShowString(1, 1, "Gear:");
    
    while(1) {
        //TEST_DRV8833_ErgodicVelocity();
        TEST_DRV8833_RandomWalk();
    }
}

void TEST_DRV8833_RandomWalk(void)
{
    static int speed = 600;//PWM_SPEED_MAX
    static int  delaymsGap = 500;
           
    //MOTOR_TURN_DRV8833(0);        
    speed = 700;    // ��Ȥ���̣�700��ʼ������ֱ�ߣ����������ǰ    //�ǵ��̵����⻹��DRV8833������?
    CAR_Run(CAR_RUN_MOED_FORWARD, speed);//ǰ������ת�����˱���ת��
    OLED_ShowNum(3, 1, speed, 4);
    delay_ms(delaymsGap);
    CAR_Run(CAR_RUN_MOED_TURNLEFT, speed);
    delay_ms(delaymsGap);   
    CAR_Run(CAR_RUN_MOED_FORWARD, speed);
    delay_ms(delaymsGap);
    CAR_Run(CAR_RUN_MOED_BACKWARD, speed);
    delay_ms(delaymsGap);    
    CAR_Run(CAR_RUN_MOED_TURNRIGHT, speed);
    delay_ms(delaymsGap);   
    CAR_Run(CAR_RUN_MOED_FORWARD, speed);
    delay_ms(delaymsGap);
    CAR_Run(CAR_RUN_MOED_BACKWARD, speed);
    delay_ms(delaymsGap);      
    speed = 0; 
    CAR_Run(CAR_RUN_MOED_FORWARD, speed);
    OLED_ShowNum(3, 1, speed, 4);
    delay_ms(5000);
}

/********************************************
 *@Func: DRV8833�����ٶ�
 *@Author: xxy
 *@Date: 2023.9.23
*********************************************/
void TEST_DRV8833_ErgodicVelocity(void)
{
    //DRV8833_1_Forward(); 
    int i = 0;
    int gear = 0;
    int speedInit = 599;
    int speed = speedInit;
    int speedStep = (PWM_SPEED_MAX+1) / 10;
    // 10���𼶵���
    for (i = 0; i < 10; i++) {        
        if (speed > PWM_SPEED_MAX) {
            speed = speedInit;
        }
        CAR_Run(CAR_RUN_MOED_FORWARD, speed);        
        //��ʾ��λ
        gear = (speed+1) / speedStep;
        OLED_ShowNum(2, 1, gear, 2);
        OLED_ShowNum(3, 1, speed, 4);
        delay_ms(5000);
        speed += speedStep;
    }
}
