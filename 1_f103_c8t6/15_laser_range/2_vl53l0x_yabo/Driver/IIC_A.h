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
/*IIC�˿ڶ���*/
#ifndef __IIC_A_H
#define __IIC_A_H

#include "stm32f10x.h"

#define uchar unsigned char
#define uint 	unsigned int

#define	IIC_DATA_MAX_LEN 64

#define SlaveAddress 0x52   //����������IIC�����еĴӵ�ַ

#define IIC1_SCL_GPIO_PORT    	GPIOA			              /* GPIO�˿� */
#define IIC1_SCL_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO�˿�ʱ�� */
#define IIC1_SCL_GPIO_PIN				GPIO_Pin_11			        /* ���ӵ�SCLʱ���ߵ�GPIO */

#define IIC1_SDA_GPIO_PORT    	GPIOA			              /* GPIO�˿� */
#define IIC1_SDA_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO�˿�ʱ�� */
#define IIC1_SDA_GPIO_PIN				GPIO_Pin_12			        /* ���ӵ�SDAʱ���ߵ�GPIO */

#define IIC2_SCL_GPIO_PORT    	GPIOB			              /* GPIO�˿� */
#define IIC2_SCL_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define IIC2_SCL_GPIO_PIN				GPIO_Pin_6			        /* ���ӵ�SCLʱ���ߵ�GPIO */

#define IIC2_SDA_GPIO_PORT    	GPIOB			              /* GPIO�˿� */
#define IIC2_SDA_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define IIC2_SDA_GPIO_PIN				GPIO_Pin_7			        /* ���ӵ�SDAʱ���ߵ�GPIO */


#define IIC1_SCL(a)	if (a)	\
					GPIO_SetBits(IIC1_SCL_GPIO_PORT,IIC1_SCL_GPIO_PIN);\
					else		\
					GPIO_ResetBits(IIC1_SCL_GPIO_PORT,IIC1_SCL_GPIO_PIN)
#define IIC1_SDA(a)	if (a)	\
					GPIO_SetBits(IIC1_SDA_GPIO_PORT,IIC1_SDA_GPIO_PIN);\
					else		\
					GPIO_ResetBits(IIC1_SDA_GPIO_PORT,IIC1_SDA_GPIO_PIN)
#define IIC2_SCL(a)	if (a)	\
					GPIO_SetBits(IIC2_SCL_GPIO_PORT,IIC2_SCL_GPIO_PIN);\
					else		\
					GPIO_ResetBits(IIC2_SCL_GPIO_PORT,IIC2_SCL_GPIO_PIN)
#define IIC2_SDA(a)	if (a)	\
					GPIO_SetBits(IIC2_SDA_GPIO_PORT,IIC2_SDA_GPIO_PIN);\
					else		\
					GPIO_ResetBits(IIC2_SDA_GPIO_PORT,IIC2_SDA_GPIO_PIN)


extern unsigned char IIC1_Rec_Data[IIC_DATA_MAX_LEN];
extern unsigned char IIC2_Rec_Data[IIC_DATA_MAX_LEN];

extern void IIC_A_GPIO_Config(void);

//extern void IIC1_Start(void);
//extern void IIC1_Stop(void);
///*����Ӧ���ź�*/
//extern void IIC1_SendAck(uchar Ack);
//extern uchar IIC1_RecAck(void);
//extern void IIC1_Send_Byte(uchar Dat);
//extern uchar IIC1_Rec_Byte(void);
/*���ֽ�д*/
extern uchar IIC1_Single_Write(uchar Address, uchar Dat);
/*���ֽڶ�*/
extern uchar IIC1_Single_Read(uchar Addr);
/*���ֽڶ�*/
//extern void IIC1_Multiple_Read(void);
extern void IIC1_Multiple_Read(uchar *ReadBuf, uchar StarAddr, uchar Len);


//extern void IIC2_Start(void);
//extern void IIC2_Stop(void);
///*����Ӧ���ź�*/
//extern void IIC2_SendAck(uchar Ack);
//extern uchar IIC2_RecAck(void);
//extern void IIC2_Send_Byte(uchar Dat);
//extern uchar IIC2_Rec_Byte(void);
/*���ֽ�д*/
extern uchar IIC2_Single_Write(uchar Address, uchar Dat);
/*���ֽڶ�*/
extern uchar IIC2_Single_Read(uchar Addr);
/*���ֽڶ�*/
//extern void IIC2_Multiple_Read(void);


#endif /*IIC_A.H*/
