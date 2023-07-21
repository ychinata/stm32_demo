#ifndef _OLED_IIC_SOFT_H_
#define _OLED_IIC_SOFT_H_	 
#include "sys.h"

#define _OLED_IIC_SOFT_NOGRAM_ 1    // ��ʹ��GRAM��ʽ

//IO��������           
#define OLED_SDA_IN()  {GPIOC->CRH&=0XF0FFFFFF;GPIOC->CRH|=(u32)8<<24;} //1000
#define OLED_SDA_OUT() {GPIOC->CRH&=0XF0FFFFFF;GPIOC->CRH|=(u32)2<<24;} //0010

#define OLED_IIC_SCL 	PCout(15) //SCL //����ʱ��
#define OLED_IIC_SDA 	PCout(14) //SDA	 //��������
#define OLED_WRITE_SDA   PCin(14)  //����SDA  		//����SDA 

#define high 1
#define low 0

#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64


///oled.c���ú���
void OLED_IIC_Start(void);// -- ����I2C����
void OLED_IIC_Stop(void);// -- �ر�I2C����
void Write_IIC_Byte(unsigned char IIC_Byte);// -- ͨ��I2C����дһ��byte������
void OLED_WrDat(unsigned char dat);// -- ��OLED��д����
void OLED_WrCmd(unsigned char cmd);// -- ��OLED��д����
u8 OLED_IIC_Wait_Ack(void);
void OLED_I2C_ACK(void);//���� ACK Ӧ��
void OLED_I2C_NoACK(void);//������ ACK Ӧ��

/************************************************* OLED��ʾ���� ***********************************************************/
void OLED_IIC_GPIO_Init(void);//��ʼ��GPIO

void OLED_Init(void);// -- OLED����ʼ�����򣬴˺���Ӧ�ڲ�����Ļ֮ǰ���ȵ���
void OLED_Set_Pos(unsigned char x, unsigned char y);// -- ������ʾ����
void OLED_Fill(unsigned char bmp_dat);// -- ȫ����ʾ(��ʾBMPͼƬʱ�Ż��õ��˹���)
void OLED_CLS(void);// -- ��λ/����
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);// -- 6x8������������ʾASCII�����С���У���̫����
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);// -- 8x16������������ʾASCII�룬�ǳ�����
void OLED_P16x16Ch(unsigned char x,unsigned char y,unsigned int N);// -- 16x16������������ʾ���ֵ���С���У������ø������塢�Ӵ֡���б���»��ߵ�
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);// -- ��128x64���ص�BMPλͼ��ȡ�����������ֱ�Ȼ���Ƶ�codetab�У��˺������ü���
void Draw_DATA(unsigned char x,unsigned char y,unsigned int N);



#endif


