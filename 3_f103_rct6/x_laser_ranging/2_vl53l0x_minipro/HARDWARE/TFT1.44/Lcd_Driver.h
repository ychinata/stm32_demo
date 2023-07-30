#ifndef __Lcd_Driver_H
#define __Lcd_Driver_H

#include "stdint.h"


//STM32F103���İ�����
//�⺯���汾����
/************** Ƕ��ʽ������  **************/
/********** mcudev.taobao.com ��Ʒ  ********/




#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//��ɫ0 3165 00110 001011 00101
#define GRAY1   0x8410      	//��ɫ1      00000 000000 00000
#define GRAY2   0x4208      	//��ɫ2  1111111111011111




#define LCD_CTRL   	  	  GPIOB		   //����TFT���ݶ˿�
#define LCD_CTRL2   	  	GPIOC		   //����TFT���ݶ˿�
#define LCD_BLK        	GPIO_Pin_7   //PC7
#define LCD_RS         	GPIO_Pin_6	 //PC6
#define LCD_CS        	GPIO_Pin_12  //PB12

#define LCD_SCL        	GPIO_Pin_13	 //PB13
#define LCD_SDA        	GPIO_Pin_15	 //PB15



//Һ�����ƿ���1�������궨��
#define	LCD_CS_SET  	LCD_CTRL->BSRR=LCD_CS    
#define	LCD_SDA_SET  	LCD_CTRL->BSRR=LCD_SDA 
#define	LCD_SCL_SET  	LCD_CTRL->BSRR=LCD_SCL 

#define	LCD_RS_SET  	LCD_CTRL2->BSRR=LCD_RS 
#define	LCD_BLK_SET  	LCD_CTRL2->BSRR=LCD_BLK   

//Һ�����ƿ���0�������궨��
#define	LCD_CS_CLR  	LCD_CTRL->BRR=LCD_CS    
#define	LCD_SDA_CLR  	LCD_CTRL->BRR=LCD_SDA    
#define	LCD_SCL_CLR  	LCD_CTRL->BRR=LCD_SCL    

#define	LCD_RS_CLR  	LCD_CTRL2->BRR=LCD_RS 
#define	LCD_BLK_CLR  	LCD_CTRL2->BRR=LCD_BLK 


#define LCD_DATAOUT(x) LCD_DATA->ODR=x; //�������
#define LCD_DATAIN     LCD_DATA->IDR;   //��������


void LCD_GPIO_Init(void);
void Lcd_WriteIndex(uint8_t Index);
void Lcd_WriteData(uint8_t Data);
void Lcd_WriteReg(uint8_t Index,uint8_t Data);
uint16_t Lcd_ReadReg(uint8_t LCD_Reg);
void Lcd_Reset(void);
void Lcd_Init(void);
void Lcd_Clear(uint16_t Color);
void Lcd_SetXY(uint16_t x,uint16_t y);
void Gui_DrawPoint(uint16_t x,uint16_t y,uint16_t Data);
unsigned int Lcd_ReadPoint(uint16_t x,uint16_t y);
void Lcd_SetRegion(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end);
void LCD_WriteData_16Bit(uint16_t Data);

#endif
