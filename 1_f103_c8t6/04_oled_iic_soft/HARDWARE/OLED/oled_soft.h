#ifndef __OLED_SOFT_H
#define __OLED_SOFT_H	 
#include "sys.h"

//
#define OLED_FONTSIZE 12

// 根据实际接线引脚修改以下配置
// SLC-PC15, SDA-PC14:OK
// SLC-PC15, SDA-PC13:OK
// 改为PA/PB不行，原因未知
//IO操作函数	           
#define OLED_IIC_SCL    PCout(15) //SCL
#define OLED_IIC_SDA    PCout(14) //SDA	 
#define OLED_READ_SDA   PCin(14)  //输入SDA 

#define OLED_4PIN_SCL GPIO_Pin_15	
#define OLED_4PIN_SDA GPIO_Pin_14

#define OLED_GPIO   GPIOC
#define OLED_RCC_APB2Periph_GPIO  RCC_APB2Periph_GPIOC
// 根据实际接线引脚修改以上配置

//IO方向设置           
#define OLED_SDA_IN()  {OLED_GPIO->CRH&=0XF0FFFFFF;OLED_GPIO->CRH|=(u32)8<<24;} //1000
#define OLED_SDA_OUT() {OLED_GPIO->CRH&=0XF0FFFFFF;OLED_GPIO->CRH|=(u32)2<<24;} //0010

void OLED_IIC_START(void);

void OLED_IIC_SendByte(u8 txd);
void OLED_IIC_Write_Command(u8 command);
void OLED_IIC_Write_Data(u8 data);


void OLED_IIC_GPIO_Init(void);
void OLED_IIC_START(void);
void OLED_IIC_STOP(void);
u8 OLED_IIC_Wait_Ack(void);


void OLED_Init(void);
void OLED_ON(void);
void OLED_Display_Off(void);
void OLED_Clear(void);  
void OLED_Fill(u8 fill_Data);//全屏填充
void OLED_SetPos(unsigned char x, unsigned char y) ;

void OLED_ClearPoint(u8 x,u8 y);
void OLED_Refresh_Gram(void);

void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1);
u32  OLED_Pow(u8 m,u8 n);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1);
void OLED_ScrollDisplay(u8 num,u8 space);
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);

#endif


