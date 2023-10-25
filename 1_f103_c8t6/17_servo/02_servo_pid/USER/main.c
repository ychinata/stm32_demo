#include "sys.h"
#include "led.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/

**************************************************************************/

int Target_Velocity=20,Position=750,Target_Position=750; //编码器的脉冲计数
int Moto;//电机PWM变量 应是Motor的 向Moto致敬
float Position_KP=120,Position_KI=0.1,Position_KD=500; //PID系数

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断分组为2
	delay_init(72);                 //=====延时初始化
	uart_init(128000);              //=====初始化串口1
	KEY_Init();						//=====按键初始化
	LED_Init();
	Servo_Init(9999,143);
	delay_ms(100);
	Timer1_Init(99,7199);           //=====定时中断初始化 
	
	while(1)
		{
			printf("Position: %d  ",Position);		//当前编码器位置打印到串口
			printf("Velocity: %d  ",Target_Velocity);				//当前的PID控制器输出打印到串口
			printf("Target: %d\r\n",Target_Position);//当前的位置目标值打印到串口
			delay_ms(200);
			
//			DataScope();//Minibalance上位机显示波形
//			delay_ms(50);//上位机需要的延时，严格50ms
		} 
}

