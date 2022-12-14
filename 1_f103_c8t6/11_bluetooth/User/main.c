#include "stm32f10x.h"                  // Device header
#include "Delay.h"			//延时
#include "LED.h"			//LED模块
#include "OLED.h"			//OLED模块
#include "Key.h"			//按键模块
#include "DS18B20.h"		//温度获取模块
#include "Motor.h"			//电机模块
#include "LightSensor.h"	//光亮度获取模块
#include "LED.h"			//LED灯模块
#include "Serial.h"			//蓝牙通信模块


/*
 * 修改波特率：单片机侧和蓝牙侧 
 */

uint16_t AT;	//接收蓝牙发过来的指令
uint8_t LEDNum,FanSpeed;	//光度，转速
int8_t FanDirection = 1 ;	//转向
char SendString[100];		//向蓝牙发送的显示信息
uint8_t StandardL = 35, StandardT = 26;	//标准亮度和温度
uint8_t NowL;	//实时亮度
float NowT;	//实时光度
uint8_t LFlag = 0, TFlag = 0;	//开启控光，控温模块；0--不开启，1--开启
uint8_t i;

void Control_L(uint8_t StandardL)	//光控模块
{
	if (NowL < StandardL)
	{
		LEDNum = 100;
	}
	else
	{
		LEDNum = 0;
	}
	LED_SetCompare2(LEDNum);
};

void Control_T(uint8_t StandardT)	//温控模块
{
	if (NowT > StandardT)
	{
		FanSpeed = 100;			
	}
	else
	{
		FanSpeed = 0;	
	}
	Motor_SetSpeed(FanSpeed);
};

int main(void)
{
	OLED_Init();
	Serial_Init();
	LED_Init();	
	//Motor_Init();
	//LightSensor_Init();
	
	OLED_ShowString(1, 1, "BT Rx:");
	
	while (1) {
		if (Serial_GetRxFlag() == 1) {
			AT = Serial_GetRxData();			
			switch (AT) {
				case 1:
					LED1_ON();
					break;
				case 2:
					LED2_ON();				
					break;
				default:
					break;
			}					
		}
	}
}

int main1(void)
{
	OLED_Init();
	Serial_Init();
	LED_Init();	
	Motor_Init();
	LightSensor_Init();
	
	while (1)
	{
		NowL = LightSensor_GetNum();
		NowT = DS18B20_ReadTemp();
		memset(SendString,0,sizeof(SendString));
		sprintf(SendString,"%d;%s",NowL,Temperature_String());
		Serial_SendString(SendString);
		
		
		if (Serial_GetRxFlag() == 1)
		{
			AT = Serial_GetRxData();			
			switch (AT)
			{
				case 1:
					LEDNum = 100;
					LED_SetCompare2(LEDNum);
					break;
				case 2:
					LEDNum = 0;
					LED_SetCompare2(LEDNum);					
					break;
				case 3:
					LEDNum = (LEDNum == 100? 100 : LEDNum + 10);
					LED_SetCompare2(LEDNum);					
					break;
				case 4:
					LEDNum = (LEDNum == 10? 10 : LEDNum - 10);
					LED_SetCompare2(LEDNum);					
					break;
				case 5:
					FanSpeed = 100;
					Motor_SetSpeed(FanSpeed);
					break;
				case 6:
					FanSpeed = 0;
					Motor_SetSpeed(FanSpeed);
					break;
				case 7:				
					FanSpeed = (FanSpeed == 100? 100 : FanSpeed + 10);
					Motor_SetSpeed(FanDirection * FanSpeed);
					break;
				case 8:					
					FanSpeed = (FanSpeed == 10? 10 : FanSpeed - 10);
					Motor_SetSpeed(FanDirection * FanSpeed);
					break;
				case 9:
					FanDirection = 1;					
					Motor_SetSpeed(FanDirection * FanSpeed);
					break;
				case 10:
					FanDirection = -1;					
					Motor_SetSpeed(FanDirection * FanSpeed);
					break;
				case 11:
					LFlag = 1;
					i++;
					break;
				case 12:					
					LFlag = 0;
					LEDNum = 0;
					LED_SetCompare2(LEDNum);
					break;
				case 13:
					TFlag = 1;		
					break;
				case 14:
					TFlag = 0;
					FanSpeed = 0;
					Motor_SetSpeed(FanSpeed);
					break;

				default:
					if (AT >= 15 && AT <= 115)
					{
						StandardL = AT - 15;
					}
					else if(AT >= 116 && AT <= 216)
					{
						StandardT = AT - 116;
					}
					break;
			}
			
		}
		if (LFlag)
		{
			Control_L(StandardL);
		}
		if (TFlag)
		{
			Control_T(StandardT);
		}
		
	}
}

