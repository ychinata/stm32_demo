#include "stm32f10x.h"
#include "oled_iic_soft.h"
#include "delay.h"
#ifdef _OLED_IIC_SOFT_NOGRAM_
#include "oled_soft_font.h"
/***********************************  ��дIICʱ��  *****************************************************/
//��ʼ��GPIO
void OLED_IIC_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //GPIO_Mode_Out_OD       //���ģ��IIC���ó��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ�� GPIO
	GPIO_SetBits(GPIOC,GPIO_Pin_14|GPIO_Pin_15); 
	
//	PWR_BackupAccessCmd(ENABLE);//�����޸�RTC �ͺ󱸼Ĵ���
//	RCC_LSEConfig(RCC_LSE_OFF);//�ر��ⲿ�����ⲿʱ���źŹ��� ��PC13 PC14 PC15 �ſ��Ե���ͨIO�á�
//	BKP_TamperPinCmd(DISABLE);//�ر����ּ�⹦�ܣ�Ҳ���� PC13��Ҳ���Ե���ͨIO ʹ��
//	PWR_BackupAccessCmd(DISABLE);//��ֹ�޸ĺ󱸼Ĵ���
	
}

//��ʼ�ź�
void OLED_IIC_Start(void)
{
	OLED_SDA_OUT();
	OLED_IIC_SCL = 1;
	OLED_IIC_SDA = 1;		//START:when CLK is high,DATA change form high to low
	delay_us(4);
	OLED_IIC_SDA = 0;
	delay_us(4);
	OLED_IIC_SCL = 0;		//ǯס I2C ���ߣ�׼�����ͻ��������
}

//��ֹ�ź�
void OLED_IIC_Stop(void)
{
	OLED_SDA_OUT();
	OLED_IIC_SCL = 0;
	OLED_IIC_SDA = 0;		//STOP:when CLK is high DATA change form low to high
	delay_us(4);
	OLED_IIC_SCL = 1;
	OLED_IIC_SDA = 1;		//���� I2C ���߽����ź�
	delay_us(4);
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
// 0������Ӧ��ɹ�
u8 OLED_IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	OLED_SDA_IN();		//SDA ����Ϊ����
	OLED_IIC_SDA=1;delay_us(1);    //OLED_I2C_SDA=1;�� BIT_ADDR(GPIOB_ODR_Addr,n)  //��� ��ֱ�Ӳ���ODR������Ҫ�����������
	//��������Ҫע��ĵط��ǣ���Ȼ������GPIO�ķ���Ϊ���룬����GPIO�ĵ�ƽ���ǿ������õģ�û��ì�ܣ�GPIO������CRL  CRH�������������ƽ��ODR�Ĵ���������
	OLED_IIC_SCL=1;delay_us(1);
	while(OLED_WRITE_SDA)
	{
		ucErrTime++;
		if(ucErrTime > 250 )
		{
			OLED_IIC_Stop();
			return 1;
		}			
	} 
	//��ȡ���͵�ƽ�Ŷ�
	OLED_IIC_SCL=0; //ʱ����� 0   //ǯλ�������´δ���
	return 0;
}	


/************************ �ò�������������**********************************/
//�ӻ�����ѡ�����Ӧ���źţ�ͬ��Ҳ����ѡ�񲻲���Ӧ���źš�
//����Ӧ���źž��Ǵӻ���SDA�����ͣ�������Ӧ���źž���SDA��һֱ���ָߵ�ƽ

//���� ACK Ӧ��
void OLED_I2C_ACK(void)
{
	//��ʱ�����൱���ڽ������ݣ��Ǳ�����
	OLED_IIC_SCL=0;
	OLED_SDA_OUT();
	OLED_IIC_SDA=0;
	delay_us(2);
	OLED_IIC_SCL=1;
	delay_us(2);
	OLED_IIC_SCL=0;
}

//������ ACK Ӧ��
void OLED_I2C_NoACK(void)
{
	OLED_IIC_SCL=0;
	OLED_SDA_OUT();
	OLED_IIC_SDA=1;
	delay_us(2);
	OLED_IIC_SCL=1;
	delay_us(2);
	OLED_IIC_SCL=0;
}
/************************ �ò�����������������**********************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	u8 i;
	OLED_SDA_OUT();
	OLED_IIC_SCL = 0;  //����ʱ�ӿ�ʼ���ݴ���
	for(i=0;i<8;i++)
	{
		OLED_IIC_SDA = (IIC_Byte & 0x80) >> 7;
		IIC_Byte <<= 1;
		delay_us(2);
		OLED_IIC_SCL = 1;
		delay_us(2);
		OLED_IIC_SCL = 0;
		delay_us(2);
	}
	//������һ�ξ��޷�д�ɹ���Ӧ������Ϊ�ھŸ�ʱ�������
	OLED_IIC_SCL = 1;
	delay_us(2);
	OLED_IIC_SCL = 0;
	delay_us(2);
}

/*********************OLEDд����************************************/ 
void OLED_WrDat(unsigned char IIC_Data)
{
	OLED_IIC_Start();
	Write_IIC_Byte(0x78);
	OLED_IIC_Wait_Ack();
	Write_IIC_Byte(0x40);			//write data
	OLED_IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Data);
	OLED_IIC_Wait_Ack();
	OLED_IIC_Stop();
}
/*********************OLEDд����************************************/
void OLED_WrCmd(unsigned char IIC_Command)
{
	OLED_IIC_Start();
	Write_IIC_Byte(0x78);            //Slave address,SA0=0
	OLED_IIC_Wait_Ack();
	Write_IIC_Byte(0x00);			//write command
	OLED_IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Command);
	OLED_IIC_Wait_Ack();
	OLED_IIC_Stop();
}

//���ϼ�https://blog.csdn.net/lgyLGY35/article/details/118693991

//���¼�https://blog.csdn.net/lgyLGY35/article/details/118655290

/***************************************************************************************************************************/
								/********************* OLED��ʾ���� *****************/

/*********************OLED ��������************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
} 
/*********************OLEDȫ��************************************/
//�������д��ʱ���β�bmp_dat��һ����λ����������Ҳ������һ���е�һ�е�8�������м��������������û�вο���
void OLED_Fill(unsigned char bmp_dat) 
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(bmp_dat);
	}
}
/*********************OLED��λ************************************/
void OLED_CLS(void)
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(0);
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : ��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	OLED_WrCmd(0X8D);  //���õ�ɱ�
	OLED_WrCmd(0X10);  //�رյ�ɱ�
	OLED_WrCmd(0XAE);  //OLED����
}


//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : ��OLED�������л���
//--------------------------------------------------------------
void OLED_ON(void)
{
	OLED_WrCmd(0X8D);  //���õ�ɱ�
	OLED_WrCmd(0X14);  //������ɱ�
	OLED_WrCmd(0XAF);  //OLED����
}


/*********************OLED��ʼ��************************************/
void OLED_Init(void)
{
	delay_ms(500);//��ʼ��֮ǰ����ʱ����Ҫ��
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(Brightness); // Set SEG Output Current Brightness ���ȵ��� 0x00~0xff
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00); //��ʼ����
	OLED_Set_Pos(0,0);
} 

/***************������������ʾ6*8һ���׼ASCII�ַ���	��ʾ�����꣨x,y����yΪҳ��Χ0��7****************/
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;   //��ȥ�ո��ASCII�룬�õ�������е�ƫ����
		if(x>121)  //121+6 = 127
		{ 
			x=0;
			y++;
		}
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
			OLED_WrDat(F6x8[c][i]);
		x += 6;
		j++;
	}
}

/*******************������������ʾ8*16һ���׼ASCII�ַ���	 ��ʾ�����꣨x,y����yΪҳ��Χ0��7****************/
void OLED_P8x16Str(unsigned char x,unsigned  char y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i+8]);
		x+=8;
		j++;
	}
}

//--------------------------------------------------------------
// Prototype      : OLED_P16x16Ch(unsigned char x, unsigned char y, unsigned char N)
// Calls          : 
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); N:������codetab.h�е�����
// Description    : ��ʾcodetab.h�еĺ���,16*16����
//--------------------------------------------------------------

/*****************������������ʾ16*16����  ��ʾ�����꣨x,y����yΪҳ��Χ0��7****************************/
void OLED_P16x16Ch(unsigned char x,unsigned char y,unsigned int N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	}
	OLED_Set_Pos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	} 	  	
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WrDat(BMP[j++]);
	    }
	}
}


void Draw_DATA(unsigned char x,unsigned char y,unsigned int N){
	u8 i=0,j;
	u16 C=N;
	while(C){
		i++;
		C/=10;
	}
	i--;
	while(N){
		j=N%10;
		OLED_Set_Pos(x+i,y);
		switch(j){
			case 0:if(i==0)break;OLED_P8x16Str(x+8*i,y,"0");break;
			case 1:OLED_P8x16Str(x+8*i,y,"1");break;
			case 2:OLED_P8x16Str(x+8*i,y,"2");break;
			case 3:OLED_P8x16Str(x+8*i,y,"3");break;
			case 4:OLED_P8x16Str(x+8*i,y,"4");break;
			case 5:OLED_P8x16Str(x+8*i,y,"5");break;
			case 6:OLED_P8x16Str(x+8*i,y,"6");break;
			case 7:OLED_P8x16Str(x+8*i,y,"7");break;
			case 8:OLED_P8x16Str(x+8*i,y,"8");break;
			case 9:OLED_P8x16Str(x+8*i,y,"9");break;
		}
		N=N/10;
		i--;
		delay_ms(10);
	}
}

#endif
