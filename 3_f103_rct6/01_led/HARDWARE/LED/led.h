#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

/*硬件说明：
    正点原子F103mini：LED0(PA8)和LED1(PD2)
    大越创新MiniPro核心板：LED0(PA8)

 */

//修改LED IO - Bgn
//LED0 -> PA8
#define PINNo_LED_0   8
#define GPIO_LED_0 GPIOA
#define RCCAPB2PeriphGPIO_LED_0 RCC_APB2Periph_GPIOA
#define LED0 PAout(PINNo_LED_0)
//LED1 -> PD2
#define PINNo_LED_1   2
#define GPIO_LED_1 GPIOD
#define RCCAPB2PeriphGPIO_LED_1 RCC_APB2Periph_GPIOD
#define LED1 PDout(PINNo_LED_1)
//修改LED IO - End

#define GPIO_Pin_LED0                 ((uint16_t)(0x1<<PINNo_LED_0))
#define GPIO_Pin_LED1                 ((uint16_t)(0x1<<PINNo_LED_1))

void LED_Init(void);//初始化

		 				    
#endif
