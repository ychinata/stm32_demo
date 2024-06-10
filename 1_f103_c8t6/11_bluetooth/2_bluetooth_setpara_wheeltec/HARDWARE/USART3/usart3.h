/***********************************************
公司：轮趣科技（东莞）有限公司
品牌：WHEELTEC
官网：wheeltec.net
淘宝店铺：shop114407458.taobao.com 
速卖通: https://minibalance.aliexpress.com/store/4455017
版本：
修改时间：2021-04-29

 
Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version:
Update：2021-04-29

All rights reserved
***********************************************/
#ifndef __USRAT3_H
#define __USRAT3_H 
#include "sys.h"	  	

//定义蓝牙串口驱动管脚
#define BT_IN_RCC    RCC_APB2Periph_GPIOB
#define BT_IN_GPIO      GPIOB    
#define BT_PIN_TX  GPIO_Pin_10      
#define BT_PIN_RX  GPIO_Pin_11
//定义蓝牙串口驱动管脚

extern u8 Usart3_Receive;
void uart3_init(u32 bound);
void USART3_IRQHandler(void);
#endif

