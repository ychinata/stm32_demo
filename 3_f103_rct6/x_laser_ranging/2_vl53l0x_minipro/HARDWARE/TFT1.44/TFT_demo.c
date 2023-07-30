/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "Lcd_Driver.h"
#include "GUI.h"

#include "delay.h"

#include "Picture.h"
#include "TFT_demo.h"

//STM32F103���İ�����
//�⺯���汾����
/************** Ƕ��ʽ������  **************/
/********** mcudev.taobao.com ��Ʒ  ********/

unsigned char Num[10]={0,1,2,3,4,5,6,7,8,9};
void Redraw_Mainmenu(void)
{

	Lcd_Clear(GRAY0);
	
	Gui_DrawFont_GBK16(8,0,BLUE,GRAY0,"STM32���Ӽ���");
	Gui_DrawFont_GBK16(16,20,RED,GRAY0,"Һ�����Գ���");

	DisplayButtonUp(15,38,113,58); //x1,y1,x2,y2
	Gui_DrawFont_GBK16(16,40,GREEN,GRAY0,"��ɫ������");

	DisplayButtonUp(15,68,113,88); //x1,y1,x2,y2
	Gui_DrawFont_GBK16(16,70,BLUE,GRAY0,"������ʾ����");

	DisplayButtonUp(15,98,113,118); //x1,y1,x2,y2
	Gui_DrawFont_GBK16(16,100,RED,GRAY0,"ͼƬ��ʾ����");
	delay_ms(1500);
}

void Num_Test(void)
{
	u8 i=0;
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,20,RED,GRAY0,"Num Test");
	delay_ms(1000);
	Lcd_Clear(GRAY0);

	for(i=0;i<10;i++)
	{
	Gui_DrawFont_Num32((i%3)*40,32*(i/3)+5,RED,GRAY0,Num[i+1]);
	delay_ms(100);
	}
	
}

void Font_Test(void)
{
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,10,BLUE,GRAY0,"������ʾ����");

	delay_ms(1000);
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(8,8,BLACK,GRAY0,"STM32���Ӽ���");
	Gui_DrawFont_GBK16(16,28,GREEN,GRAY0,"רעҺ������");
	Gui_DrawFont_GBK16(16,48,RED,GRAY0, "ȫ�̼���֧��");
	Gui_DrawFont_GBK16(0,68,BLUE,GRAY0," Tel:1234567890");
	Gui_DrawFont_GBK16(0,88,RED,GRAY0, " mcudev.taobao");	
	delay_ms(1800);	
}

void Color_Test(void)
{
	u8 i=1;
	Lcd_Clear(GRAY0);
	
	Gui_DrawFont_GBK16(20,10,BLUE,GRAY0,"Color Test");
	delay_ms(500);

	while(i--)
	{
		Lcd_Clear(WHITE);
		delay_ms(500);
		Lcd_Clear(BLACK);
		delay_ms(500);
		Lcd_Clear(RED);
		delay_ms(500);
	  Lcd_Clear(GREEN);
		delay_ms(500);
	  Lcd_Clear(BLUE);
		delay_ms(500);
	}		
}

//ȡģ��ʽ ˮƽɨ�� ������ ��λ��ǰ
void showimage(const unsigned char *p) //��ʾ40*40 QQͼƬ
{
  	int i,j,k; 
	unsigned char picH,picL;
	Lcd_Clear(WHITE); //����  
	
	for(k=0;k<3;k++)
	{
	   	for(j=0;j<3;j++)
		{	
			Lcd_SetRegion(40*j,40*k,40*j+39,40*k+39);		//��������
		    for(i=0;i<40*40;i++)
			 {	
			 	picL=*(p+i*2);	//���ݵ�λ��ǰ
				picH=*(p+i*2+1);				
				LCD_WriteData_16Bit(picH<<8|picL);  						
			 }	
		 }
	}		
}


//ȡģ��ʽ ˮƽɨ�� ������ ��λ��ǰ
void Fullscreen_showimage(const unsigned char *p) //��ʾ128*128 ͼƬ
{
  	int i; 
	unsigned char picH,picL;
	
	Lcd_Clear(WHITE); //����  
	
	
			Lcd_SetRegion(0,0,127,127);		//��������:ɨ����㵽�յ�0��127���պ�128����
		    for(i=0;i<128*128;i++)
				 {	
					picL=*(p+i*2);	//���ݵ�λ��ǰ
					picH=*(p+i*2+1);				
					LCD_WriteData_16Bit(picH<<8|picL);  						
				 }	
		
}


void Test_Demo(void)
{
	Lcd_Init();
//	LCD_LED_SET;//ͨ��IO���Ʊ�����				:���̲���Ҫ��ע�͵�
	Redraw_Mainmenu();//�������˵�(�����������ڷֱ��ʳ�������ֵ�����޷���ʾ)
	Color_Test();//�򵥴�ɫ������
	Num_Test();//������������
	Font_Test();//��Ӣ����ʾ����		
	
	showimage(gImage_qq);//ͼƬ��ʾʾ��:��ʾʾ����ͼƬԽ�󣬻�ռ�ø����FLASH�ռ䣬���Ը�������ʵ�����ͼƬ��ʾ����
	
	delay_ms(1500);
	
	Fullscreen_showimage(gImage_XHR128);//ͼƬ��ʾʾ��:��ʾʾ����ͼƬԽ�󣬻�ռ�ø����FLASH�ռ䣬���Ը�������ʵ�����ͼƬ��ʾ����
	delay_ms(1500);
	Fullscreen_showimage(gImage_XNH128);//ͼƬ��ʾʾ��
	delay_ms(1500);
	Fullscreen_showimage(gImage_ATM128);//ͼƬ��ʾʾ��
	delay_ms(1500);
//	LCD_LED_CLR;//IO���Ʊ�����	
}
