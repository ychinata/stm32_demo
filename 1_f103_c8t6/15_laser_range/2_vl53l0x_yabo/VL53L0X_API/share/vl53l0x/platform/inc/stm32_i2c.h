#ifndef _VL53L0X_I2C_H
#define _VL53L0X_I2C_H

#include "stm32f10x.h"
#include "stm32f10x_i2c.h"

#define I2C_SCL_GPIO       	GPIOB
#define	I2C_PIN_SCL       	GPIO_Pin_6
#define I2C_SCL_HIGH()      GPIO_SetBits(I2C_SCL_GPIO,I2C_PIN_SCL) ;
#define I2C_SCL_LOW()  	    GPIO_ResetBits(I2C_SCL_GPIO,I2C_PIN_SCL);

//#define I2C_X_GPIO       	GPIOB
//#define	I2C_PIN_X       	GPIO_Pin_7
//#define I2C_X_HIGH()        GPIO_SetBits(I2C_X_GPIO,I2C_PIN_X) ;
//#define I2C_X_LOW()  	    GPIO_ResetBits(I2C_X_GPIO,I2C_PIN_X);

#define I2C_SDA_GPIO       	GPIOB
#define	I2C_PIN_SDA       	GPIO_Pin_7
#define I2C_SDA_HIGH()      GPIO_SetBits(I2C_SDA_GPIO,I2C_PIN_SDA) ;
#define I2C_SDA_LOW()  	    GPIO_ResetBits(I2C_SDA_GPIO,I2C_PIN_SDA);
#define I2C_SDA_STATE       GPIO_ReadInputDataBit(I2C_SDA_GPIO,I2C_PIN_SDA)
//(I2C_SDA_GPIO->IDR & I2C_PIN_SDA)

void i2c_init(void);
uint8_t i2c_write(uint8_t addr, uint8_t reg, uint32_t len, uint8_t * data);
uint8_t i2c_read(uint8_t addr, uint8_t reg, uint32_t len, uint8_t *buf);
#endif
