#ifndef __OLED_H
#define __OLED_H

#define uint8_t u8

/*硬件说明：
    正点原子F103mini：
    大越创新MiniPro核心板：PB6/7
    
    F103RCT6与F103C8T6的IIC资源相似

 */


//修改LED IO - Bgn
#define OLED_4PIN_SCL GPIO_Pin_6
#define OLED_4PIN_SDA GPIO_Pin_7
//#define OLED_4PIN_SCL GPIO_Pin_8	
//#define OLED_4PIN_SDA GPIO_Pin_9	

#define GPIO_Pin_OLED_SCL   X
#define GPIO_Pin_OLED_SDA   X
#define GPIO_OLED GPIOB
#define RCCAPB2PeriphGPIO_OLED RCC_APB2Periph_GPIOB

//修改LED IO - End

#define OLED_W_SCL(x)		GPIO_WriteBit(GPIOB, OLED_4PIN_SCL, (BitAction)(x))
#define OLED_W_SDA(x)		GPIO_WriteBit(GPIOB, OLED_4PIN_SDA, (BitAction)(x))

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

#endif
