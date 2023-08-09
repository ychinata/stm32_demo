	
/********************************************************************************

//代码功能：  标准库 GPIO 控制

//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

*******************************************************************************/


#ifndef __gpio_H
#define __gpio_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f4xx.h"
#include "main.h"

//LED端口定义
	 
#define LED2_ON     GPIO_ResetBits(GPIOC,GPIO_Pin_13)  	//LED2灯 IO输出低电平--点亮
#define LED2_OFF    GPIO_SetBits(GPIOC,GPIO_Pin_13)   	//LED2灯 IO输出高电平--熄灭	 
	 
	 
void GPIO_Config(void);

#ifdef __cplusplus
}
#endif

#endif






























/********************************************************************************

//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

*******************************************************************************/





