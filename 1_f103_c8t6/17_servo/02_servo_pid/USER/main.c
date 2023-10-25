#include "sys.h"
#include "led.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/

**************************************************************************/

int Target_Velocity=20,Position=750,Target_Position=750; //���������������
int Moto;//���PWM���� Ӧ��Motor�� ��Moto�¾�
float Position_KP=120,Position_KI=0.1,Position_KD=500; //PIDϵ��

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�����жϷ���Ϊ2
	delay_init(72);                 //=====��ʱ��ʼ��
	uart_init(128000);              //=====��ʼ������1
	KEY_Init();						//=====������ʼ��
	LED_Init();
	Servo_Init(9999,143);
	delay_ms(100);
	Timer1_Init(99,7199);           //=====��ʱ�жϳ�ʼ�� 
	
	while(1)
		{
			printf("Position: %d  ",Position);		//��ǰ������λ�ô�ӡ������
			printf("Velocity: %d  ",Target_Velocity);				//��ǰ��PID�����������ӡ������
			printf("Target: %d\r\n",Target_Position);//��ǰ��λ��Ŀ��ֵ��ӡ������
			delay_ms(200);
			
//			DataScope();//Minibalance��λ����ʾ����
//			delay_ms(50);//��λ����Ҫ����ʱ���ϸ�50ms
		} 
}

