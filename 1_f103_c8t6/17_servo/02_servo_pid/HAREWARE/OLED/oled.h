#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
 /**************************************************************************
���ߣ�ƽ��С��֮�� 
�Ա����̣�http://shop114407458.taobao.com/
**************************************************************************/
//-----------------OLED�˿ڶ���---------------- 
#define OLED_RST_Clr() PBout(3)=0   //RES
#define OLED_RST_Set() PBout(3)=1   //RES

#define OLED_RS_Clr() PAout(15)=0    //DC
#define OLED_RS_Set() PAout(15)=1    //DC

#define OLED_SCLK_Clr()  PBout(5)=0  //SCL
#define OLED_SCLK_Set()  PBout(5)=1   //SCL

#define OLED_SDIN_Clr()  PBout(4)=0   //SDA
#define OLED_SDIN_Set()  PBout(4)=1   //SDA

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
//OLED�����ú���
void OLED_WR_Byte(u8 dat,u8 cmd);	         //��OLEDд��һ���ֽ�
void OLED_Display_On(void);                //����OLED��ʾ
void OLED_Display_Off(void);               //�ر�OLED��ʾ
void OLED_Refresh_Gram(void);		   				 //ˢ����ʾ		    
void OLED_Init(void);                      //��ʼ��OLED
void OLED_Clear(void);                     //OLED��������պ�������Ļ���Ǻ�ɫ
void OLED_DrawPoint(u8 x,u8 y,u8 t);       //OLED����
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode); //��ָ����λ����ʾһ���ַ�
void OLED_ShowNumber(u8 x,u8 y,u32 num,u8 len,u8 size); //��ָ����λ����ʾ����
void OLED_ShowString(u8 x,u8 y,const u8 *p);	          //��ָ����λ����ʾ�ַ���
#endif     
	 
