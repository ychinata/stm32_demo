#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//引脚定义
#define OneWire 		GPIO_Pin_0
#define OneWire_Port 	GPIOB
#define OneWire_LOW 	GPIO_ResetBits(OneWire_Port,OneWire)
#define OneWire_HIGH 	GPIO_SetBits(OneWire_Port,OneWire)

/**
  * @brief  单总线通信引脚初始化
  * @param  无
  * @retval 无
  */
void OneWire_Pin_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructrue;
	GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructrue.GPIO_Pin = OneWire;
	GPIO_InitStructrue.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(OneWire_Port,&GPIO_InitStructrue);
}
/**
  * @brief  设置引脚是输入还是输出模式
  * @param  cmd 1--为输出模式，0--为输入模式
  * @retval 无
  */
void OneWire_Output_Input(uint8_t cmd)
{
	GPIO_InitTypeDef GPIO_InitStructrue;
	
	if (cmd)//OUT
	{
		GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_Out_PP;		
	}
	else	//IN
	{
		GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_IPU;
	}
	
	GPIO_InitStructrue.GPIO_Pin = OneWire;
	GPIO_InitStructrue.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(OneWire_Port,&GPIO_InitStructrue);
}
/**
  * @brief  单总线初始化
  * @param  无
  * @retval AckBit 从机响应位，0--为响应，1--为未响应
  */
uint8_t OneWire_Init(void)
{
	OneWire_Pin_Init();
	uint8_t AckBit;
	
	OneWire_Output_Input(1);
	OneWire_LOW;
	Delay_us(480);	//480us
	OneWire_HIGH;
	Delay_us(30);	//30us
	
	OneWire_Output_Input(0);
	AckBit = GPIO_ReadInputDataBit(OneWire_Port,OneWire);	//60-240US
	Delay_us(200);	//480us
	
	return AckBit;
}

/**
  * @brief  单总线发送一个字节
  * @param  Byte 要发送的字节
  * @retval 无
  */
void OneWire_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i=0;i<8;i++)
	{
		OneWire_Output_Input(1);
		OneWire_LOW;
		Delay_us(2);	//2us
		(Byte&(0x01<<i))? OneWire_HIGH : OneWire_LOW;
		Delay_us(45);	//30us
		OneWire_HIGH;
	}
}

/**
  * @brief  单总线接收一个字节
  * @param  无
  * @retval Byte 接收的一个字节
  */
uint8_t OneWire_ReceiveByte(void)
{
	uint8_t i,Byte=0x00;
	
	for (i=0;i<8;i++)
	{
		OneWire_Output_Input(1);
		OneWire_LOW;
		Delay_us(2);	//2us
		OneWire_HIGH;
		OneWire_Output_Input(0);
		if (GPIO_ReadInputDataBit(OneWire_Port,OneWire)){Byte|=(0x01<<i);
		}
		Delay_us(45);	//45us
	}
	
	return Byte;
}

	
