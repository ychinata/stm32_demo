#ifndef _OLED_IIC_SOFT_H_
#define _OLED_IIC_SOFT_H_	 
#include "sys.h"

//#define _OLED_IIC_SOFT_NOGRAM_ 1    // 不使用GRAM方式
#define _OLED_IIC_SOFT_GRAM_ 1    // 使用GRAM方式

//IO方向设置           
#define OLED_SDA_IN()  {GPIOC->CRH&=0XF0FFFFFF;GPIOC->CRH|=(u32)8<<24;} //1000
#define OLED_SDA_OUT() {GPIOC->CRH&=0XF0FFFFFF;GPIOC->CRH|=(u32)2<<24;} //0010

#ifdef _OLED_IIC_SOFT_NOGRAM_
#define OLED_IIC_SCL 	PCout(15) //SCL //串行时钟
#define OLED_IIC_SDA 	PCout(14) //SDA	 //串行数据
#define OLED_WRITE_SDA   PCin(14)  //输入SDA  		//输入SDA 

#define high 1
#define low 0

#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64


///oled.c调用函数
void OLED_IIC_Start(void);// -- 开启I2C总线
void OLED_IIC_Stop(void);// -- 关闭I2C总线
void Write_IIC_Byte(unsigned char IIC_Byte);// -- 通过I2C总线写一个byte的数据
void OLED_WrDat(unsigned char dat);// -- 向OLED屏写数据
void OLED_WrCmd(unsigned char cmd);// -- 向OLED屏写命令
u8 OLED_IIC_Wait_Ack(void);
void OLED_I2C_ACK(void);//产生 ACK 应答
void OLED_I2C_NoACK(void);//不产生 ACK 应答

/************************************************* OLED显示函数 ***********************************************************/
void OLED_IIC_GPIO_Init(void);//初始化GPIO

void OLED_Init(void);// -- OLED屏初始化程序，此函数应在操作屏幕之前最先调用
void OLED_Set_Pos(unsigned char x, unsigned char y);// -- 设置显示坐标
void OLED_Fill(unsigned char bmp_dat);// -- 全屏显示(显示BMP图片时才会用到此功能)
void OLED_CLS(void);// -- 复位/清屏
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);// -- 6x8点整，用于显示ASCII码的最小阵列，不太清晰
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);// -- 8x16点整，用于显示ASCII码，非常清晰
void OLED_P16x16Ch(unsigned char x,unsigned char y,unsigned int N);// -- 16x16点整，用于显示汉字的最小阵列，可设置各种字体、加粗、倾斜、下划线等
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);// -- 将128x64像素的BMP位图在取字软件中算出字表，然后复制到codetab中，此函数调用即可
void Draw_DATA(unsigned char x,unsigned char y,unsigned int N);
#endif

#ifdef _OLED_IIC_SOFT_GRAM_

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

void OLED_Test(void);

#endif



#endif


