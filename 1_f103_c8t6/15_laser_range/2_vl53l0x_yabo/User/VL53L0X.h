#ifndef __VL53L0X_H
#define	__VL53L0X_H


#include "stm32f10x.h"

#define VL53L0X_REG_IDENTIFICATION_MODEL_ID         0xc0
#define VL53L0X_REG_IDENTIFICATION_REVISION_ID      0xc2
#define VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD   0x50
#define VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD 0x70
#define VL53L0X_REG_SYSRANGE_START                  0x00
#define VL53L0X_REG_RESULT_INTERRUPT_STATUS         0x13
#define VL53L0X_REG_RESULT_RANGE_STATUS             0x14
//#define address 0x29

#define VL53L0X1_INT_GPIO_PORT    	GPIOA			              /* GPIO端口 */
#define VL53L0X1_INT_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define VL53L0X1_INT_GPIO_PIN				GPIO_Pin_8			        /* 连接到SCL时钟线的GPIO */

#define VL53L0X2_INT_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define VL53L0X2_INT_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define VL53L0X2_INT_GPIO_PIN				GPIO_Pin_7			        /* 连接到SCL时钟线的GPIO */

extern void VL53L0X_Test(void);

#endif
