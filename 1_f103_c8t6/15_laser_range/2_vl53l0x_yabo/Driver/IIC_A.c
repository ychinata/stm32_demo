/*----------------------------------------------------------------------------
//Copyright (C) 2015 ����ά�����ӿƼ����޹�˾
//��Ȩ����
//
//�� �� ����
//����������
//			
//
//������־��
//
//�޸ı�־��
//�޸�������
//---------------------------------------------------------------------------*/

#include "IIC_A.h"
#include "bsp_SysTick.h"

#define DELTIME   10

#define DelayNum 	1

uchar IIC1_Rec_Data[IIC_DATA_MAX_LEN]={0};
uchar IIC2_Rec_Data[IIC_DATA_MAX_LEN]={0};

void IIC_A_GPIO_Config(void)
{
			/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;
	
		//IIC1����
	
		/*������ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd(IIC1_SCL_GPIO_CLK, ENABLE);		
		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = IIC1_SCL_GPIO_PIN;	
		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(IIC1_SCL_GPIO_PORT, &GPIO_InitStructure);
	
		/*������ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd(IIC1_SDA_GPIO_CLK, ENABLE);		
		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		/*��������ģʽΪͨ�ÿ�©���*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //��©���+�ⲿ���������������� 
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = IIC1_SDA_GPIO_PIN;	
		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(IIC1_SDA_GPIO_PORT, &GPIO_InitStructure);
	
		//IIC2����
	
		/*������ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd(IIC2_SCL_GPIO_CLK, ENABLE);		
		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = IIC2_SCL_GPIO_PIN;	
		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(IIC2_SCL_GPIO_PORT, &GPIO_InitStructure);
	
		/*������ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd(IIC2_SDA_GPIO_CLK, ENABLE);		
		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		/*��������ģʽΪͨ�ÿ�©���*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //��©���+�ⲿ����������������  
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = IIC2_SDA_GPIO_PIN;	
		/*���ÿ⺯������ʼ��GPIO*/
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
/*����Ӧ���ź�*/
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
/*����Ӧ���ź�*/
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
/*��IIC���߷���һ���ֽ�����*/
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
/*��IIC���߽���һ���ֽ�����*/
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
/*���ֽ�д*/
uchar IIC1_Single_Write(uchar Address, uchar Dat)
{
	IIC1_Start();
	IIC1_Send_Byte(SlaveAddress);
	IIC1_Send_Byte(Address);
	IIC1_Send_Byte(Dat);
	IIC1_Stop();
	return 0;
}
/*���ֽڶ�*/
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
///*���ֽڶ�*/
//Len>=1;
void IIC1_Multiple_Read(uchar *ReadBuf, uchar StarAddr, uchar Len)
{
	uchar i;  //�����������ݣ���ַ��Χ
	
	for(i=0;i<Len;i++)
	{
		ReadBuf[i] = IIC1_Single_Read(StarAddr+i);
	}
	
	
//	//unsigned char Rec_Data[6];
//	IIC1_Start();
//	IIC1_Send_Byte(SlaveAddress);
//	IIC1_Send_Byte(StarAddr);//���ʹ洢��Ԫ��ַ����  StarAddr  ��ʼ 
//	IIC1_Stop();
//	IIC1_Start();
//	IIC1_Send_Byte(SlaveAddress+1);
//	for(i=0;i<Len;i++) //������ȡLen����ַ���ݣ��洢��Rec_Data
//	{
//		ReadBuf[i]=IIC1_Rec_Byte();
//		if(i==(Len-1))
//				IIC1_SendAck(1); //���һ��������Ҫ��NOACK
//		else
//				IIC1_SendAck(0); //��ӦACK
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
/*����Ӧ���ź�*/
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
/*����Ӧ���ź�*/
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
/*��IIC���߷���һ���ֽ�����*/
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
/*��IIC���߽���һ���ֽ�����*/
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
/*���ֽ�д*/
uchar IIC2_Single_Write(uchar Address, uchar Dat)
{
	IIC2_Start();
	IIC2_Send_Byte(SlaveAddress);
	IIC2_Send_Byte(Address);
	IIC2_Send_Byte(Dat);
	IIC2_Stop();
	return 0;
}
/*���ֽڶ�*/
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
///*���ֽڶ�*/
//void IIC2_Multiple_Read(void)
//{
//	uchar i;  //�����������ݣ���ַ��Χ
//	//unsigned char Rec_Data[6];
//	IIC2_Start();
//	IIC2_Send_Byte(SlaveAddress);
//	IIC2_Send_Byte(0x03);//���ʹ洢��Ԫ��ַ����0x03��ʼ 
//	IIC2_Start();
//	IIC2_Send_Byte(SlaveAddress+1);
//	for(i=0;i<6;i++) //������ȡ6����ַ���ݣ��洢��Rec_Data
//	{
//		IIC2_Rec_Data[i]=IIC2_Rec_Byte();
//		if(i==5)
//				IIC2_SendAck(1); //���һ��������Ҫ��NOACK
//		else
//				IIC2_SendAck(0); //��ӦACK
//	}
//	IIC2_Stop();
////	Delay(DELTIME);
//}
