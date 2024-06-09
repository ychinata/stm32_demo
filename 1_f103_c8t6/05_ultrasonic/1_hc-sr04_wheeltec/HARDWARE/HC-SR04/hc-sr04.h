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
#ifndef __HC_SR04_H
#define __HC_SR04_H

#include <sys.h>	 

// 引脚定义（无需修改）
#define HCSR04_GPIO_TRIG    ((GPIO_TypeDef *) (APB2PERIPH_BASE + 0x0800 + 0x400*HCSR04_GPIO_ENUM_TRIG)) //GPIOB
#define HCSR04_PIN_TRIG     ((uint16_t)(0x1 << HCSR04_PIN_ENUM_TRIG))//GPIO_Pin_1
#define HCSR04_RCC_TRIG     ((uint32_t)(4<<HCSR04_GPIO_ENUM_TRIG))

#define HCSR04_GPIO_ECHO    ((GPIO_TypeDef *) (APB2PERIPH_BASE + 0x0800 + 0x400*HCSR04_GPIO_ENUM_ECHO)) //GPIOB
#define HCSR04_PIN_ECHO     ((uint16_t)(0x1 << HCSR04_PIN_ENUM_ECHO))//GPIO_Pin_0
#define HCSR04_RCC_ECHO     ((uint32_t)(4<<HCSR04_GPIO_ENUM_ECHO))

// 函数声明
void HCSR04_Init(u16 arr,u16 psc);
u32 HCSR04_GetDistane(void);
void TIM3_IRQHandler(void);

#endif
