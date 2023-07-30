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
/*IIC端口定义*/
#ifndef __IIC_A_H
#define __IIC_A_H

#include "stm32f10x.h"

#define uchar unsigned char
#define uint 	unsigned int

#define	IIC_DATA_MAX_LEN 64

#define SlaveAddress 0x52   //定义器件在IIC总线中的从地址

#define IIC1_SCL_GPIO_PORT    	GPIOA			              /* GPIO端口 */
#define IIC1_SCL_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define IIC1_SCL_GPIO_PIN				GPIO_Pin_11			        /* 连接到SCL时钟线的GPIO */

#define IIC1_SDA_GPIO_PORT    	GPIOA			              /* GPIO端口 */
#define IIC1_SDA_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define IIC1_SDA_GPIO_PIN				GPIO_Pin_12			        /* 连接到SDA时钟线的GPIO */

#define IIC2_SCL_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define IIC2_SCL_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define IIC2_SCL_GPIO_PIN				GPIO_Pin_6			        /* 连接到SCL时钟线的GPIO */

#define IIC2_SDA_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define IIC2_SDA_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define IIC2_SDA_GPIO_PIN				GPIO_Pin_7			        /* 连接到SDA时钟线的GPIO */


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
///*发送应答信号*/
//extern void IIC1_SendAck(uchar Ack);
//extern uchar IIC1_RecAck(void);
//extern void IIC1_Send_Byte(uchar Dat);
//extern uchar IIC1_Rec_Byte(void);
/*单字节写*/
extern uchar IIC1_Single_Write(uchar Address, uchar Dat);
/*单字节读*/
extern uchar IIC1_Single_Read(uchar Addr);
/*多字节读*/
//extern void IIC1_Multiple_Read(void);
extern void IIC1_Multiple_Read(uchar *ReadBuf, uchar StarAddr, uchar Len);


//extern void IIC2_Start(void);
//extern void IIC2_Stop(void);
///*发送应答信号*/
//extern void IIC2_SendAck(uchar Ack);
//extern uchar IIC2_RecAck(void);
//extern void IIC2_Send_Byte(uchar Dat);
//extern uchar IIC2_Rec_Byte(void);
/*单字节写*/
extern uchar IIC2_Single_Write(uchar Address, uchar Dat);
/*单字节读*/
extern uchar IIC2_Single_Read(uchar Addr);
/*多字节读*/
//extern void IIC2_Multiple_Read(void);


#endif /*IIC_A.H*/
