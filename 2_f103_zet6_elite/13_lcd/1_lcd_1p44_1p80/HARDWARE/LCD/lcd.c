#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "Text.h"
#include "Dis_Picture.h" 

//////////////////////////////////////////////////////////////////////////////////	 

//1.44��  LCD Һ������	  
  
//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

//********************************************************************************

//////////////////////////////////////////////////////////////////////////////////	 
				 
//LCD�Ļ�����ɫ�ͱ���ɫ	  

u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 

uint8_t USE_HORIZONTAL=0;    //�������ã� 		0,����ģʽ   1,����ģʽ.


//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;



/******************************************************************************

  �ӿڶ��壬����ݽ����޸Ĳ��޸���ӦIO��ʼ��--�������Ӻ�IOҪ��Ӧ

//	#define LCD_SDA        	//PB15      //����������
//	#define LCD_SCL        	//PB13      //ʱ����
//	#define LCD_CS        	//PB12      //Ƭѡ	
//	#define LCD_SDO/RST     //PB14      //�������/��λ
//	#define LCD_DC         	//PC5       //����/�����л�
//	#define LCD_BLK         //PB1    	  //�������



*******************************************************************************/


/******************************************************************************/

//������ void SPI2_Init(void)

//�������ܣ������� Ӳ��SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI2�ĳ�ʼ��


//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

/******************************************************************************/


void SPI2_Init(void)
{	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB ,ENABLE);
	
	//����SPI2�ܽ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
			
	//SPI2����ѡ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);
 
	//����ֻ���SPI�ڳ�ʼ��
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);    //��λSPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);   //ֹͣ��λSPI2

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:ȫ��˫��ģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                    //����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		                      //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                      //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                        //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	//���岨����Ԥ��Ƶ��ֵ:������Ԥ��Ƶֵ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                          //CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);                                 //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����

	SPI2_ReadWriteByte(0xff);//��������		  
	
}



/******************************************************************************/

//������ void LCD_GPIO_Init(void)

//�������ܣ�����Һ��IO��ʼ������

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	


//	#define LCD_SDA        	//PB15      //����������
//	#define LCD_SCL        	//PB13      //ʱ����
//	#define LCD_CS        	//PB12      //Ƭѡ	
//	#define LCD_SDO/RST     //PB14      //�������/��λ
//	#define LCD_DC         	//PC5       //����/�����л�
//	#define LCD_BLK         //PB1    	  //�������

/******************************************************************************/
void LCD_GPIO_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	      
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC ,ENABLE);
	
  #if    Used_HW_SPI   //��ʼ��Ӳ��SPI2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 |GPIO_Pin_12 ;   //  Ƭѡ	  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;               //PC5    C/D  ����/�����л�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //100MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //�������
	GPIO_Init(GPIOC, &GPIO_InitStructure);                 
  		
  SPI2_Init();           //��ʼ��Ӳ��SPI2
	
	#else   //ʹ��IOģ��SPIͨ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;   //| GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;               //PC5    C/D  ����/�����л�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //100MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //�������
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;               //PB14--MISO    ���ݽ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //100MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	#endif 
	

// LCD_HardwareRest();   //Ӳ��λ--���IO���ӣ�Ӳ����λ�ɿ���Ч
	
	LCD_BLK_On;           //��������
	
//	LCD_BLK_Off;   //�رձ���
//	LCD_BLK_On;    //��������
      
}




/********************************************************************/
//������u8 LCD_SPI_ReadWriteByte(u8 TxData)
//�������ܣ� SPI��д���� �� ʹ�ò����Ĵ�����ʽ���ٶȸ���

//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�


//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
//*******************************************************************/

u8 SPI2_ReadWriteByte(u8 TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	
	SPI_I2S_SendData(SPI2, TxData);                                  //ͨ������SPIx����һ��byte  ����
	
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
 
	return SPI_I2S_ReceiveData(SPI2);                                //����ͨ��SPIx������յ�����	
   
}


/********************************************************************/
//������u8 LCD_SPI_WR(u8 byte)
//�������ܣ� SPI��д���� �� ʹ��IO����ģ�ⷽʽ


//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�


//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
//*******************************************************************/

u8 IO_SPI_ReadWriteByte(u8 TxData)
{
	unsigned char bit_ctr;
	
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)  // ���8λ
	{
		
		
		if((TxData&0x80)==0x80)LCD_SDA_SET; 			// MSB TO MOSI
			else LCD_SDA_CLR; 

		LCD_SCL_CLR;
		
		TxData=(TxData<<1);					  // shift next bit to MSB
				
		TxData|=LCD_SDO;	        		// capture current MISO bit
		
		LCD_SCL_SET;
		
	}
	
	LCD_SCL_SET;//����״̬Ϊ�ߵ�ƽ
	
	return TxData;
	
}


//********************************************************************/

//������void LCD_WR_REG(u16 regval)
//�������ܣ�
//д�Ĵ�������
//regval:�Ĵ���ֵ

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
//*******************************************************************/

void LCD_WR_REG(u16 regval)
{   
	 LCD_CS_CLR;  //LCD_CS=0  //Ƭѡ
   LCD_DC_CLR;  //LCD_DC=0  //��������״̬
	 LCD_SPI_ReadWriteByte(regval&0x00FF);
	 LCD_CS_SET;  //LCD_CS=1	 
}



//*******************************************************************/

//������void LCD_WR_DATA8(u8 data)   //д8λ����
//�������ܣ�
//Һ����--д8λ���ݺ���

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
//*******************************************************************/


void LCD_WR_DATA8(u8 data)   //д8λ����
{
	LCD_CS_CLR;  //LCD_CS=0  //Ƭѡ
	LCD_DC_SET;	 //LCD_DC=1   //��������״̬			    	   
	LCD_SPI_ReadWriteByte(data);	
	LCD_CS_SET;  //LCD_CS=1   			 
}


//*******************************************************************/
//������void LCD_WR_DATA16(u16 data)   //д16λ����
//�������ܣ�дLCD����
//���������
//data:Ҫд���ֵ

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
//*******************************************************************/


void LCD_WR_DATA16(u16 data)
{	
	
 	LCD_CS_CLR;  //LCD_CS=0  //Ƭѡ
	LCD_DC_SET;	 //LCD_DC=1   //��������״̬
	LCD_SPI_ReadWriteByte(data>>8);
	LCD_SPI_ReadWriteByte(data);
	LCD_CS_SET;    //LCD_CS=1
	
}


//*******************************************************************/

//������void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)  //дҺ�����Ĵ���

//�������ܣ�д�Ĵ���

//�������: 
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
//*******************************************************************/

void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);		   //д��Ҫд�ļĴ������	 
	LCD_WR_DATA8(LCD_RegValue);//д������	    		 
}


//*******************************************************************/
//������void LCD_WriteRAM_Prepare(void)   
//�������ܣ���ʼдGRAM  ����

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
//*******************************************************************/

void LCD_WriteRAM_Prepare(void)
{
 	LCD_WR_REG(lcddev.wramcmd);	  
}	



//*******************************************************************/
//������LCD_WriteRAM(u16 RGB_Code)   
//�������ܣ�д�������ɫֵ
//�������:
//RGB_Code:��ɫֵ

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
//*******************************************************************/

void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD_WR_DATA16( RGB_Code );//дʮ��λGRAM
}

//********************************************************************************/
//������void LCD_DisplayOn(void)
//�������ܣ�
//LCD������ʾ

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
//*******************************************************************/
void LCD_DisplayOn(void)
{					   
	LCD_WR_REG(0X29);	//������ʾ
}	


//*******************************************************************/
//������void LCD_DisplayOff(void)
//�������ܣ�
//LCD�ر���ʾ

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
//*******************************************************************/

void LCD_DisplayOff(void)
{	   
	LCD_WR_REG(0X28);	//�ر���ʾ

} 

//********************************************************************************/
//������void LCD_SoftRest(void)
//�������ܣ�����Ļ�����ִ����λ����
//LCD������ʾ

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
//*******************************************************************/
void LCD_SoftRest(void)
{					   
	LCD_WR_REG(0x01);	//������λ����
	LCD_Delay_ms(100);      // delay 50 ms 
}	


//********************************************************************************/
//������void LCD_SoftRest(void)
//�������ܣ�����Ļ�����ִ����λ����
//LCD������ʾ

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
//*******************************************************************/
void LCD_HardwareRest(void)
{					   
	LCD_RST_CLR;     //Һ������λ --Ӳ��λ--ʹ��       //PB14��ΪҺ������λ��������
	LCD_Delay_ms(50);      // delay 50 ms 
	LCD_DC_SET;      //Һ������λ --Ӳ��λ--ʧ��       //PB14��ΪҺ������λ��������
	LCD_Delay_ms(30);      // delay 30 ms 
}	



//*******************************************************************/
//������void LCD_SetCursor(u16 Xpos, u16 Ypos)
//�������ܣ����ù��λ��
//���������
//Xpos:������
//Ypos:������

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
//*******************************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 
    if(USE_HORIZONTAL==0) {
            
        LCD_WR_REG(lcddev.setxcmd); 
        LCD_WR_DATA16(Xpos+2);

        LCD_WR_REG(lcddev.setycmd);
        LCD_WR_DATA16(Ypos+3);	
            
    } else {
        LCD_WR_REG(lcddev.setxcmd); 
        LCD_WR_DATA16(Xpos+3);

        LCD_WR_REG(lcddev.setycmd);
        LCD_WR_DATA16(Ypos+2);	
    }
	
}





//*******************************************************************/
//������void LCD_DrawPoint(u16 x,u16 y)
//�������ܣ�����
//���������
//x,y:����
//POINT_COLOR:�˵����ɫ

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
//*******************************************************************/

void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD_WR_DATA16(POINT_COLOR); 
}


//*******************************************************************/
//������void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
//�������ܣ����ٻ���
//���������
//x,y:����
//color:��ɫ

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
//*******************************************************************/

void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	
     
    	LCD_SetCursor(x,y);		//���ù��λ�� 
			LCD_WR_REG(lcddev.wramcmd); 
			LCD_WR_DATA16(color); //д��16λ��ɫ

}	 





/**************************************************************************/
//������void LCD_Display_Dir(u8 dir)
//�������ܣ�����LCD����ʾ�������ز���

//���������

//����LCD��ʾ����

////dir:   0,����  ��
//         1,����  ��
//         2,����  ��
//         3,����  ��

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
//*************************************************************************/
void LCD_Display_Dir(u8 dir)
{
	
		
	if(dir==0)			     //����  ��
	{
		lcddev.dir=0;	     //����
		lcddev.width=LCD_X;
		lcddev.height=LCD_H;
    
		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;
		
    LCD_WriteReg(0x36,0xC8);//ѡ��ɨ�跽��		
		
		USE_HORIZONTAL=0;   //�������ã� 		0,����ģʽ   1,����ģʽ.
	

	}
	
else if (dir==1)			 //����
	{	  				
		lcddev.dir=0;	     //����
		lcddev.width=LCD_X;
		lcddev.height=LCD_H;
    
		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;
		
		LCD_WriteReg(0x36,0x48);//ѡ��ɨ�跽��		
		
		USE_HORIZONTAL=0;   //�������ã� 		0,����ģʽ   1,����ģʽ.
		

	} 	
	
	
	else if (dir==2)			//����
	{	  				
		lcddev.dir=1;	     //����
		lcddev.width=LCD_H;
		lcddev.height=LCD_X;

		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  
		
		LCD_WriteReg(0x36,0xA8);//ѡ��ɨ�跽��		
		
		USE_HORIZONTAL=1;   //�������ã� 		0,����ģʽ   1,����ģʽ.
		

		
	} 
 else if (dir==3)				  //����
	{	  				
		lcddev.dir=1;	        //����
		lcddev.width=LCD_H;
		lcddev.height=LCD_X;

		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B; 
    LCD_WriteReg(0x36,0x68);//ѡ��ɨ�跽��		
		
		USE_HORIZONTAL=1;   //�������ã� 		0,����ģʽ   1,����ģʽ.
		


	} 	
 else //����Ĭ��Ϊ����--��
 {
	  lcddev.dir=0;	     //����
		lcddev.width=LCD_X;
		lcddev.height=LCD_H;
    
		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;

	  LCD_WriteReg(0x36,0xC8);//ѡ��ɨ�跽��		
	 
	  USE_HORIZONTAL=0;   //�������ã� 		0,����ģʽ   1,����ģʽ.
	 
	 
 }	 


//////�������ã�Ϊ���ڲ������ã�������ȫ������ʾ��Χ			

   LCD_Set_Window(0,0,lcddev.width,lcddev.height);//����ȫ������
 
 
 

 	
}	


/**************************************************************************/
//������void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
//�������ܣ�����LCD����ʾ����

//���ô���,���Զ����û������굽�������Ͻ�(sx,sy).
//sx,sy:������ʼ����(���Ͻ�)
//width,height:���ڿ�Ⱥ͸߶�,�������0!!
//�����С:width*height.

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
//*************************************************************************/

void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{   
	
	
	  width=sx+width-1;
	  height=sy+height-1;
   if(USE_HORIZONTAL==0)
			 {
				 
				LCD_WR_REG(lcddev.setxcmd); 
				LCD_WR_DATA16(sx+2);      //���� X�������
				LCD_WR_DATA16(width+2);   //���� X�����յ�	
					
				LCD_WR_REG(lcddev.setycmd);
				LCD_WR_DATA16(sy+3);      //���� Y�������
				LCD_WR_DATA16(height+3);  //���� Y�����յ�
				 
			 }
		 else
			 {
				  LCD_WR_REG(lcddev.setxcmd); 
					LCD_WR_DATA16(sx+3);      //���� X�������
					LCD_WR_DATA16(width+3);   //���� X�����յ�	
						
					LCD_WR_REG(lcddev.setycmd);
					LCD_WR_DATA16(sy+2);      //���� Y�������
					LCD_WR_DATA16(height+2);  //���� Y�����յ�
				 
		  }
} 


/*******************************************************************************/
//������void LCD_Init(void)
//�������ܣ���ʼ��lcd


//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/*******************************************************************************/

void LCD_Init(void)
{ 	
	 
  LCD_GPIO_Init();        //��ʼ������ I/O�ӿ�	
	
	LCD_SoftRest();        //��λ 
	
	//LCD Init For 1.44Inch LCD Panel with ST7735R.
	LCD_WR_REG(0x11);//Sleep exit 
	delay_ms (120);
		
	//ST7735R Frame Rate
	LCD_WR_REG(0xB1); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x2D); 

	LCD_WR_REG(0xB2); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x2D); 

	LCD_WR_REG(0xB3); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x2D); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x2D); 
	
	LCD_WR_REG(0xB4); //Column inversion 
	LCD_WR_DATA8(0x07); 
	
	//ST7735R Power Sequence
	LCD_WR_REG(0xC0); 
	LCD_WR_DATA8(0xA2); 
	LCD_WR_DATA8(0x02); 
	LCD_WR_DATA8(0x84); 
	LCD_WR_REG(0xC1); 
	LCD_WR_DATA8(0xC5); 

	LCD_WR_REG(0xC2); 
	LCD_WR_DATA8(0x0A); 
	LCD_WR_DATA8(0x00); 

	LCD_WR_REG(0xC3); 
	LCD_WR_DATA8(0x8A); 
	LCD_WR_DATA8(0x2A); 
	LCD_WR_REG(0xC4); 
	LCD_WR_DATA8(0x8A); 
	LCD_WR_DATA8(0xEE); 
	
	LCD_WR_REG(0xC5); //VCOM 
	LCD_WR_DATA8(0x0E); 
	
	LCD_WR_REG(0x36); //MX, MY, RGB mode 
	LCD_WR_DATA8(0xC8); 
	
	//ST7735R Gamma Sequence
	LCD_WR_REG(0xe0); 
	LCD_WR_DATA8(0x0f); 
	LCD_WR_DATA8(0x1a); 
	LCD_WR_DATA8(0x0f); 
	LCD_WR_DATA8(0x18); 
	LCD_WR_DATA8(0x2f); 
	LCD_WR_DATA8(0x28); 
	LCD_WR_DATA8(0x20); 
	LCD_WR_DATA8(0x22); 
	LCD_WR_DATA8(0x1f); 
	LCD_WR_DATA8(0x1b); 
	LCD_WR_DATA8(0x23); 
	LCD_WR_DATA8(0x37); 
	LCD_WR_DATA8(0x00); 	
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x02); 
	LCD_WR_DATA8(0x10); 

	LCD_WR_REG(0xe1); 
	LCD_WR_DATA8(0x0f); 
	LCD_WR_DATA8(0x1b); 
	LCD_WR_DATA8(0x0f); 
	LCD_WR_DATA8(0x17); 
	LCD_WR_DATA8(0x33); 
	LCD_WR_DATA8(0x2c); 
	LCD_WR_DATA8(0x29); 
	LCD_WR_DATA8(0x2e); 
	LCD_WR_DATA8(0x30); 
	LCD_WR_DATA8(0x30); 
	LCD_WR_DATA8(0x39); 
	LCD_WR_DATA8(0x3f); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x03); 
	LCD_WR_DATA8(0x10);  
	
	LCD_WR_REG(0x2a);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x7f);

	LCD_WR_REG(0x2b);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x9f);
	
	LCD_WR_REG(0xF0); //Enable test command  
	LCD_WR_DATA8(0x01); 
	LCD_WR_REG(0xF6); //Disable ram power save mode 
	LCD_WR_DATA8(0x00); 
	
	LCD_WR_REG(0x3A); //65k mode 
	LCD_WR_DATA8(0x05); 
	
	
	LCD_WR_REG(0x29);//Display on	 

	  LCD_Display_Dir(LCD_DIR_Mode);	//ѡ��--��Ļ��ʾ��ʽ
		
	  LCD_BLK_On;					//��������
		
	  LCD_Clear(WHITE);
	
	
	
}


//////////////////���º�������Ļ��ʾͼ�εļ���������////////////DevEBox  ��Խ����//////shop389957290.taobao.com//////////////////////////////////////



/*******************************************************************************/
//������void LCD_Clear(u16 color)
//�������ܣ�ȫ��������亯��
//���������
//color:Ҫ���������ɫ

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/*******************************************************************************/

void LCD_Clear(u16 color)
{
	u32 index=0;
	u32 totalpoint;
	
	LCD_Set_Window(0,0,lcddev.width,lcddev.height);//����ȫ������
	
	totalpoint=lcddev.width * lcddev.height; 			//�õ��ܵ���
 
	LCD_SetCursor(0,0);	//���ù��λ�� 
	
	LCD_WriteRAM_Prepare();     		//��ʼд��GRAM	
	
	for(index=0;index<totalpoint;index++)
	{
		LCD_WR_DATA16(color);	
	}
} 


/*******************************************************************************/
//������void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)

//�������ܣ�������亯��--��䵥����ɫ
//���������
//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/*******************************************************************************/

void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
		u32 i;
		u32 xlen=0;

    //���ô���
	
	  LCD_Set_Window(sx,sy,ex-sx,ey-sy);//���ô���
 	
	  LCD_WR_REG(lcddev.wramcmd);	  
	
 		xlen=(ex-sx)*(ey-sy);//������ܹ���Ҫд��ĵ���
		
		LCD_WriteRAM_Prepare();     			//��ʼд��GRAM
		
		for(i=0;i<=xlen;i++)
		{
		 	LCD_WR_DATA16(color);	//��ʾ��ɫ 	    
		}


		//�ָ�ȫ������
				
		//�������ã�Ϊ���ڲ������ã�������ȫ������ʾ��Χ			
			
    LCD_Set_Window(0,0,lcddev.width,lcddev.height);//����ȫ������
		 
}


/*******************************************************************************/
//������void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)

//�������ܣ�������亯��
//���������
//��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/*******************************************************************************/

void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  

		u32 i;
		u32 xlen=0;

 //���ô���

    LCD_Set_Window(sx,sy,ex-sx,ey-sy);//���ô���	
		  	
	  LCD_WR_REG(lcddev.wramcmd);	  
	
 		xlen=(ex-sx)*(ey-sy);//������ܹ���Ҫд��ĵ���
		
		LCD_WriteRAM_Prepare();     			//��ʼд��GRAM
		
		for(i=0;i<=xlen;i++)
		{
		 	LCD_WR_DATA16(*color);	//��ʾ��ɫ 	    
		}


	//�������ã�Ϊ���ڲ������ã�������ȫ������ʾ��Χ			
			
    LCD_Set_Window(0,0,lcddev.width,lcddev.height);//����ȫ������
		

}



/*******************************************************************************/
//������void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
//�������ܣ�����
//���������
//x1,y1:�������
//x2,y2:�յ�����
//Color;������ɫ

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/*******************************************************************************/

void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 Color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	
	
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else
     {
		    incx=-1;
		    delta_x=-delta_x;
		 }

	
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else
     { 
		   incy=-1;
		   delta_y=-delta_y;
		 }

	
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_Fast_DrawPoint(uRow,uCol,Color);//���� --ʹ��������ɫ���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

/*******************************************************************************/
//������void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
//�������ܣ�������	  
//���������
//(x1,y1),(x2,y2):���εĶԽ�����
//Color;������ɫ

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/*******************************************************************************/

void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 Color)
{
	LCD_DrawLine(x1,y1,x2,y1,Color);
	LCD_DrawLine(x1,y1,x1,y2,Color);
	LCD_DrawLine(x1,y2,x2,y2,Color);
	LCD_DrawLine(x2,y1,x2,y2,Color);
}

/*******************************************************************************/
//������void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
//�������ܣ���ָ��λ�û�һ��ָ����С��Բ
//���������
//(x,y):���ĵ�
//r    :�뾶
//Color;������ɫ

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/*******************************************************************************/

void LCD_Draw_Circle(u16 x0,u16 y0,u8 r, u16 Color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_Fast_DrawPoint(x0+a,y0-b,Color);             //5
 		LCD_Fast_DrawPoint(x0+b,y0-a,Color);             //0           
		LCD_Fast_DrawPoint(x0+b,y0+a,Color);             //4               
		LCD_Fast_DrawPoint(x0+a,y0+b,Color);             //6 
		LCD_Fast_DrawPoint(x0-a,y0+b,Color);             //1       
 		LCD_Fast_DrawPoint(x0-b,y0+a,Color);             
		LCD_Fast_DrawPoint(x0-a,y0-b,Color);             //2             
  	LCD_Fast_DrawPoint(x0-b,y0-a,Color);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
}


/*******************************************************************************/
//������LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u16 color,u8 mode)
//�������ܣ���ָ��λ����ʾһ���ַ�
//���������
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24
//color,�ַ���ɫ
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/*******************************************************************************/

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u16 color,u8 mode)
{  							  
  u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,color);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
}



/*******************************************************************************/
//������u32 LCD_Pow(u8 m,u8 n)
//�������ܣ�m^n����
//�������������8λ����
//����ֵ:m^n�η�.

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/*******************************************************************************/
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}	


/*******************************************************************************/
//������void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size,u16 color)
//�������ܣ���ʾ����,��λΪ0,����ʾ
//���������

//x,y :�������	 
//num:��ֵ(0~4294967295);	
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 


//����ֵ:��

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/*******************************************************************************/

void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size,u16 color)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,color,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,color,0); 
	}
} 



/*******************************************************************************/
//������void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u16 color,u8 mode)
//�������ܣ���ʾ����,��λΪ0,������ʾ
//���������
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//color:��ɫ 
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/*******************************************************************************/

void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u16 color,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,color,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,color,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,color,mode&0X01); 
	}
} 

/*******************************************************************************/
//������void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u16 color,u8 *p)
//�������ܣ���ʾ�ַ���
//���������
//x,y:�������
//width,height:�����С  
//size:�����С
//color:��ɫ 
//*p:�ַ�����ʼ��ַ		  

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/*******************************************************************************/


void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u16 color,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,color,0);
        x+=size/2;
        p++;
    }  
}







//****************************************************************************************/
//������void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2)
//��������: ����Ļ��ʾһ͹��İ�ť��
//��    ��: u16 x1,y1,x2,y2 ��ť�����ϽǺ����½�����
//��    ��: ��


//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/****************************************************************************************/

void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2)
{
	
	LCD_DrawLine(x1,y1, x2,y1,GRAY2);  //H
		
	LCD_DrawLine(x1+1,y1+1,x2,y1+1,GRAY1);  //H
		
	LCD_DrawLine(x1,  y1,  x1,y2,GRAY2);    //V
		
	LCD_DrawLine(x1+1,y1+1,x1+1,y2,GRAY1);  //V
	
	LCD_DrawLine(x1, y2, x2, y2,WHITE);     //H
	LCD_DrawLine(x2, y1, x2, y2,WHITE);     //V
}

//****************************************************************************************/
//������void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2)
//��������: ����Ļ��ʾһ���µİ�ť��
//��    ��: u16 x1,y1,x2,y2 ��ť�����ϽǺ����½�����
//��    ��: ��
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/****************************************************************************************/
void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2)
{
	
	LCD_DrawLine(x1,  y1,  x2,y1,WHITE);    //H
	LCD_DrawLine(x1,  y1,  x1,y2,WHITE);    //V
	
	LCD_DrawLine(x1+1,y2-1,x2,y2-1,GRAY1);  //H
		
	LCD_DrawLine(x1,  y2,  x2,y2,GRAY2);    //H
		
	LCD_DrawLine(x2-1,y1+1,x2-1,y2,GRAY1);  //V
		
  LCD_DrawLine(x2  ,y1  ,x2,y2,GRAY2);    //V
	

}


//****************************************************************************************/
//������void Draw_Test(void)
//��������: ����ͼ�κ�������


//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/****************************************************************************************/

void Draw_Test(void)
{
	
		LCD_Clear(WHITE); //����
		
	  LCD_DrawLine(20,20, lcddev.width-20,lcddev.height-20,RED);//���ߺ���
	  LCD_DrawLine(20,lcddev.height-20, lcddev.width-20, 20,RED);//���ߺ���
	
	  LCD_DrawRectangle(20,20,lcddev.width-20,lcddev.height-20, BLUE);//���Ʒ���״
	  
	  LCD_Draw_Circle(lcddev.width/2,lcddev.height/2,lcddev.width/3-1, BRED);//����Բ��
	  LCD_Draw_Circle(lcddev.width/2,lcddev.height/2,lcddev.width/3, BRED);//����Բ��
	  LCD_Draw_Circle(lcddev.width/2,lcddev.height/2,lcddev.width/3+1, BRED);//����Բ��
			
		delay_ms(2000);//��ʱ

}


//****************************************************************************************/
//������void Color_Test(void)
//��������: ��ɫ�����ʾ����


//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/****************************************************************************************/

void Color_Test(void)
{
	
		LCD_Clear(WHITE); //����
		
		Draw_Font16B(8,16,BLUE,"1:��ɫ������");
	
	  delay_ms(1000);
	
	  LCD_Fill(5,5,lcddev.width-5,lcddev.height-5,RED);//����һ�����ڣ�д��ָ��������ɫ
	
	  LCD_Fill(20,20,lcddev.width-20,lcddev.height-20,YELLOW);//����һ�����ڣ�д��ָ��������ɫ
	
	  LCD_Fill(50,50,lcddev.width-50,lcddev.height-50,BLUE);//����һ�����ڣ�д��ָ��������ɫ
	
	  LCD_Fill(60,60,lcddev.width-60,lcddev.height-60,MAGENTA);//����һ�����ڣ�д��ָ��������ɫ
	
		delay_ms(1000);

		LCD_Clear(WHITE);
	
	  Draw_Test();//��ͼ��������
	
		delay_ms(300);//��ʱ
		LCD_Clear(BLACK);
		delay_ms(300);//��ʱ
		LCD_Clear(RED);
		delay_ms(300);//��ʱ
		LCD_Clear(YELLOW);
		delay_ms(300);//��ʱ
		LCD_Clear(GREEN);
		delay_ms(300);//��ʱ
		LCD_Clear(BLUE);
		delay_ms(300);//��ʱ

}


uint16_t D_Color=BLUE; //������ɫ
uint16_t B_Color=WHITE; //������ɫ

//****************************************************************************************/
//������void Font_Test(void)
//��������: ����������ʾ����


//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/****************************************************************************************/

void Font_Test(void)
{
	LCD_Clear(WHITE);
	Draw_Font16B(8,16,BLUE,"2:������ʾ����");

	delay_ms(1000);
	LCD_Clear(WHITE);
	
	Draw_Font24B(16,4,RED, "��Խ����");
	Draw_Font24B(4,32,BLACK,"Һ��������");
	Draw_Font16B(4,60, BLACK,"Ƕ��ʽ������");

	Draw_Font16B(0,100,BLUE,"F:mcudev.taobao.com");
	
	delay_ms(1000);	
	
	LCD_Clear(WHITE);

	Draw_Font16B(4,20,RED,"0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz");

	delay_ms(1000);	
	
	LCD_Clear(WHITE);


}


//****************************************************************************************/
//������void Demo_Menu(void)
//��������: ��ʾ����˵�


//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com
/****************************************************************************************/
void LCD_DemoMenu(void)
{

	LCD_Clear(WHITE); //����
	
	Draw_Font24B(12,2,RED,"��Խ����");
	Draw_Font24B(4,30,RED,"Һ��������");

	Draw_Font16B(4,60,BLUE,"1: ��ɫ���");
	Draw_Font16B(4,80,BLUE,"2: ������ʾ");
	Draw_Font16B(4,100,BLUE,"3: ͼƬ��ʾ");
//  
	Draw_Font16B(4,120,RED,"F:mcudev.taobao.com");
//	Draw_Font16B(4,140,RED,"F:devebox.taobao.com");
//	
    delay_ms(2000);//��ʱ

    Color_Test();       //��ɫ������
    Font_Test();        //����������ʾ����
    GBK_LibFont_Test(); //GBK�ֿ���� -(���ʹ�ò����ֿ��Һ�����汾���˴��������Σ������ֿ���ԣ�
    //Show_Picture();     //��ʾһ��ͼƬ	// ע�͵����Լ���100k��bin��С
}

