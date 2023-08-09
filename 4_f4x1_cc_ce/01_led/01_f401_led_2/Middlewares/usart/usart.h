#ifndef __USART_H
#define __USART_H


#include "stdio.h"	
#include "stm32f4xx_conf.h"


/********************************************************************************

//代码功能：串口 USART 1 驱动

//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

*******************************************************************************/


#define USART_REC_LEN  			64  	//定义最大接收字节数 64

#define EN_USART1_RX 			  1 		//使能（1）/禁止（0）串口1接收


extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	


//如果想串口中断接收，请不要注释以下宏定义

void USART1_init(u32 bound);


#endif















/********************************************************************************

//代码功能：串口 USART 1 驱动

//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

*******************************************************************************/



