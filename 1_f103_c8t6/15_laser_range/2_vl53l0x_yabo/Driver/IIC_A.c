/*----------------------------------------------------------------------------
//Copyright (C) 2015 广州维脉电子科技有限公司
//版权所有
//
//文 件 名：
//功能描述：
//			
//
//创建标志：
//
//修改标志：
//修改描述：
//---------------------------------------------------------------------------*/

#include "IIC_A.h"
#include "bsp_SysTick.h"

#define DELTIME   10

#define DelayNum 	1

uchar IIC1_Rec_Data[IIC_DATA_MAX_LEN]={0};
uchar IIC2_Rec_Data[IIC_DATA_MAX_LEN]={0};

void IIC_A_GPIO_Config(void)
{
			/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;
	
		//IIC1配置
	
		/*开启相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd(IIC1_SCL_GPIO_CLK, ENABLE);		
		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = IIC1_SCL_GPIO_PIN;	
		/*调用库函数，初始化GPIO*/
		GPIO_Init(IIC1_SCL_GPIO_PORT, &GPIO_InitStructure);
	
		/*开启相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd(IIC1_SDA_GPIO_CLK, ENABLE);		
		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		/*设置引脚模式为通用开漏输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //开漏输出+外部上拉电阻可输入输出 
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = IIC1_SDA_GPIO_PIN;	
		/*调用库函数，初始化GPIO*/
		GPIO_Init(IIC1_SDA_GPIO_PORT, &GPIO_InitStructure);
	
		//IIC2配置
	
		/*开启相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd(IIC2_SCL_GPIO_CLK, ENABLE);		
		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = IIC2_SCL_GPIO_PIN;	
		/*调用库函数，初始化GPIO*/
		GPIO_Init(IIC2_SCL_GPIO_PORT, &GPIO_InitStructure);
	
		/*开启相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd(IIC2_SDA_GPIO_CLK, ENABLE);		
		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		/*设置引脚模式为通用开漏输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //开漏输出+外部上拉电阻可输入输出  
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = IIC2_SDA_GPIO_PIN;	
		/*调用库函数，初始化GPIO*/
		GPIO_Init(IIC2_SDA_GPIO_PORT, &GPIO_InitStructure);	
}

void IIC1_Start(void)
{
	IIC1_SDA(1);
	IIC1_SCL(1);
	Delay_us(DelayNum);
	IIC1_SDA(0);
	Delay_us(DelayNum);
	IIC1_SCL(0);
	Delay_us(DelayNum);
}

void IIC1_Stop(void)
{
	IIC1_SCL(1);
	Delay_us(DelayNum);
	IIC1_SDA(0);
	Delay_us(DelayNum);
	IIC1_SDA(1);
	Delay_us(DelayNum);
}
/*发送应答信号*/
void IIC1_SendAck(uchar Ack)//Ack (0:Ack 1:NoAck)
{
  IIC1_SCL(0);
	if(Ack)
	{
		IIC1_SDA(1);
	}
  else
	{
		IIC1_SDA(0);
	}
//	Delay_us(DelayNum);
	IIC1_SCL(1);
//	Delay_us(DelayNum);
	IIC1_SCL(0);
//	Delay_us(DelayNum);
}
/*接收应答信号*/
uchar IIC1_RecAck(void)
{
  uchar A;
	IIC1_SCL(0);
  if(GPIO_ReadInputDataBit(IIC1_SDA_GPIO_PORT,IIC1_SDA_GPIO_PIN))//IIC1_SDA
	{
		A=1;
	}
	else
	{
		A=0;
	}
//	Delay_us(DelayNum);
	IIC1_SCL(1);  
//	Delay_us(DelayNum);  
	IIC1_SCL(0);
//	Delay_us(DelayNum);
	return (A?1:0);
}
/*向IIC总线发送一个字节数据*/
void IIC1_Send_Byte(uchar Dat)
{
	uchar i;
	IIC1_SCL(0);
	for(i=0;i<8;i++)
	{
		if(Dat & 0x80)
				IIC1_SDA(1);
		else
				IIC1_SDA(0);
		Dat<<=1;
//		Delay_us(DelayNum);
		IIC1_SCL(1);
//		Delay_us(DelayNum);
		IIC1_SCL(0);
//		Delay_us(DelayNum);	
	}
	IIC1_RecAck();
}
/*从IIC总线接收一个字节数据*/
uchar IIC1_Rec_Byte(void)
{
	unsigned char i,Dat=0;

	for(i=0;i<8;i++)
	{
		IIC1_SCL(0);
		Delay_us(1); 
		IIC1_SCL(1);
		Dat<<=1;
		if(GPIO_ReadInputDataBit(IIC1_SDA_GPIO_PORT,IIC1_SDA_GPIO_PIN))   //IIC1_SDA
		{
			Dat |=0x01;
		}
		Delay_us(1);    
	}
	return Dat;
}
/*单字节写*/
uchar IIC1_Single_Write(uchar Address, uchar Dat)
{
	IIC1_Start();
	IIC1_Send_Byte(SlaveAddress);
	IIC1_Send_Byte(Address);
	IIC1_Send_Byte(Dat);
	IIC1_Stop();
	return 0;
}
/*单字节读*/
uchar IIC1_Single_Read(uchar Addr)
{
	uchar Value;
	IIC1_Start();
	IIC1_Send_Byte(SlaveAddress);
	IIC1_Send_Byte(Addr);
	IIC1_Start();
	IIC1_Send_Byte(SlaveAddress+1);
	Value=IIC1_Rec_Byte();
	IIC1_SendAck(1);
	IIC1_Stop();
	return Value;
}
///*多字节读*/
//Len>=1;
void IIC1_Multiple_Read(uchar *ReadBuf, uchar StarAddr, uchar Len)
{
	uchar i;  //连续读出数据，地址范围
	
	for(i=0;i<Len;i++)
	{
		ReadBuf[i] = IIC1_Single_Read(StarAddr+i);
	}
	
	
//	//unsigned char Rec_Data[6];
//	IIC1_Start();
//	IIC1_Send_Byte(SlaveAddress);
//	IIC1_Send_Byte(StarAddr);//发送存储单元地址，从  StarAddr  开始 
//	IIC1_Stop();
//	IIC1_Start();
//	IIC1_Send_Byte(SlaveAddress+1);
//	for(i=0;i<Len;i++) //连续读取Len个地址数据，存储在Rec_Data
//	{
//		ReadBuf[i]=IIC1_Rec_Byte();
//		if(i==(Len-1))
//				IIC1_SendAck(1); //最后一个数据需要回NOACK
//		else
//				IIC1_SendAck(0); //回应ACK
//		Delay_us(1);
//	}
//	IIC1_Stop();
////	Delay(DELTIME);
}


void IIC2_Start(void)
{
	IIC2_SDA(1);
	IIC2_SCL(1);
	Delay_us(DelayNum);
	IIC2_SDA(0);
	Delay_us(DelayNum);
	IIC2_SCL(0);
	Delay_us(DelayNum);
}

void IIC2_Stop(void)
{
	IIC2_SCL(1);
	Delay_us(DelayNum);
	IIC2_SDA(0);
	Delay_us(DelayNum);
	IIC2_SDA(1);
	Delay_us(DelayNum);
}
/*发送应答信号*/
void IIC2_SendAck(uchar Ack)//Ack (0:Ack 1:NoAck)
{
  IIC2_SCL(0);
	if(Ack)
	{
		IIC2_SDA(1);
	}
  else
	{
		IIC2_SDA(0);
	}
//	Delay_us(DelayNum);
	IIC2_SCL(1);
//	Delay_us(DelayNum);
	IIC2_SCL(0);
//	Delay_us(DelayNum);
}
/*接收应答信号*/
uchar IIC2_RecAck(void)
{
  uchar A;
	IIC2_SCL(0);
  if(GPIO_ReadInputDataBit(IIC2_SDA_GPIO_PORT,IIC2_SDA_GPIO_PIN))//IIC2_SDA
	{
		A=1;
	}
	else
	{
		A=0;
	}
//	Delay_us(DelayNum);
	IIC2_SCL(1);  
//	Delay_us(DelayNum);  
	IIC2_SCL(0);
//	Delay_us(DelayNum);
	return (A?1:0);
}
/*向IIC总线发送一个字节数据*/
void IIC2_Send_Byte(uchar Dat)
{
	uchar i;
	IIC2_SCL(0);
	for(i=0;i<8;i++)
	{
		if(Dat & 0x80)
				IIC2_SDA(1);
		else
				IIC2_SDA(0);
		Dat<<=1;
//		Delay_us(DelayNum);
		IIC2_SCL(1);
//		Delay_us(DelayNum);
		IIC2_SCL(0);
//		Delay_us(DelayNum);	
	}
	IIC2_RecAck();
}
/*从IIC总线接收一个字节数据*/
uchar IIC2_Rec_Byte(void)
{
	unsigned char i,Dat=0;

	for(i=0;i<8;i++)
	{
		IIC2_SCL(0);
		Delay_us(1); 
		IIC2_SCL(1);
		Dat<<=1;
		if(GPIO_ReadInputDataBit(IIC2_SDA_GPIO_PORT,IIC2_SDA_GPIO_PIN))   //IIC2_SDA
		{
			Dat |=0x01;
		}
		Delay_us(1);    
	}
	return Dat;
}
/*单字节写*/
uchar IIC2_Single_Write(uchar Address, uchar Dat)
{
	IIC2_Start();
	IIC2_Send_Byte(SlaveAddress);
	IIC2_Send_Byte(Address);
	IIC2_Send_Byte(Dat);
	IIC2_Stop();
	return 0;
}
/*单字节读*/
uchar IIC2_Single_Read(uchar Addr)
{
	uchar Value;
	IIC2_Start();
	IIC2_Send_Byte(SlaveAddress);
	IIC2_Send_Byte(Addr);
	IIC2_Start();
	IIC2_Send_Byte(SlaveAddress+1);
	Value=IIC2_Rec_Byte();
	IIC2_SendAck(1);
	IIC2_Stop();
	return Value;
}
///*多字节读*/
//void IIC2_Multiple_Read(void)
//{
//	uchar i;  //连续读出数据，地址范围
//	//unsigned char Rec_Data[6];
//	IIC2_Start();
//	IIC2_Send_Byte(SlaveAddress);
//	IIC2_Send_Byte(0x03);//发送存储单元地址，从0x03开始 
//	IIC2_Start();
//	IIC2_Send_Byte(SlaveAddress+1);
//	for(i=0;i<6;i++) //连续读取6个地址数据，存储在Rec_Data
//	{
//		IIC2_Rec_Data[i]=IIC2_Rec_Byte();
//		if(i==5)
//				IIC2_SendAck(1); //最后一个数据需要回NOACK
//		else
//				IIC2_SendAck(0); //回应ACK
//	}
//	IIC2_Stop();
////	Delay(DELTIME);
//}
