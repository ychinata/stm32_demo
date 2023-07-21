
#include "stm32f10x.h"
#include "oled_soft.h"
#include "delay.h"
//#include "usart.h"
#ifdef _OLED_IIC_SOFT_GRAM_
#include "oled_soft_font.h"

//https://blog.csdn.net/lgyLGY35/article/details/118693991

//�����õ���һ�ָ���GRAM��ʵ���Ͼ���SRAM�ķ�ʽ���޸���ʾ�����ݣ���ΪOLEDģ�鲻֧���ȶ���д������ÿ��д�Ķ�����GRAM��
//�������ͣ�

u8 OLED_GRAM[128][8];

void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_IIC_Write_Command(0xb0+i);  //����ҳ��ַ��0~7��
		OLED_IIC_Write_Command(0x00);    //������ʾλ�á��е͵�ַ
		OLED_IIC_Write_Command(0x10);    //������ʾλ�á��иߵ�ַ	
		for(n=0;n<128;n++)
			OLED_IIC_Write_Data(OLED_GRAM[n][i]); 
	}   
}


/***********************************  ��дIICʱ��  *****************************************************/
//��ʼ��GPIO
void OLED_IIC_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(OLED_RCC_APB2Periph_GPIO | RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Pin = OLED_4PIN_SCL|OLED_4PIN_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //GPIO_Mode_Out_OD       //���ģ��IIC���ó��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(OLED_GPIO, &GPIO_InitStructure); //��ʼ�� GPIO
	GPIO_SetBits(OLED_GPIO, OLED_4PIN_SCL|OLED_4PIN_SDA); 
	
//	PWR_BackupAccessCmd(ENABLE);//�����޸�RTC �ͺ󱸼Ĵ���
//	RCC_LSEConfig(RCC_LSE_OFF);//�ر��ⲿ�����ⲿʱ���źŹ��� ��PC13 PC14 PC15 �ſ��Ե���ͨIO�á�
//	BKP_TamperPinCmd(DISABLE);//�ر����ּ�⹦�ܣ�Ҳ���� PC13��Ҳ���Ե���ͨIO ʹ��
//	PWR_BackupAccessCmd(DISABLE);//��ֹ�޸ĺ󱸼Ĵ���
	
}

//��ʼ�ź�
void OLED_IIC_START(void)
{
	OLED_SDA_OUT();
	OLED_IIC_SCL = 1;
	OLED_IIC_SDA = 1;		//START:when CLK is high,DATA change form high to low
	delay_us(4);
	OLED_IIC_SDA = 0;
	delay_us(4);
	OLED_IIC_SCL = 0;		//ǯס IIC ���ߣ�׼�����ͻ��������
}

//��ֹ�ź�
void OLED_IIC_STOP(void)
{
	OLED_SDA_OUT();
	OLED_IIC_SCL = 0;
	OLED_IIC_SDA = 0;		//STOP:when CLK is high DATA change form low to high
	delay_us(4);
	OLED_IIC_SCL = 1;
	OLED_IIC_SDA = 1;		//���� IIC ���߽����ź�
	delay_us(4);
}



//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
// 0������Ӧ��ɹ�
u8 OLED_IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	OLED_SDA_IN();		//SDA ����Ϊ����
	OLED_IIC_SDA=1;delay_us(1);    //OLED_IIC_SDA=1;�� BIT_ADDR(GPIOB_ODR_Addr,n)  //��� ��ֱ�Ӳ���ODR������Ҫ�����������
	//��������Ҫע��ĵط��ǣ���Ȼ������GPIO�ķ���Ϊ���룬����GPIO�ĵ�ƽ���ǿ������õģ�û��ì�ܣ�GPIO������CRL  CRH�������������ƽ��ODR�Ĵ���������
	OLED_IIC_SCL=1;delay_us(1);
	while(OLED_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime > 250 )
		{
			OLED_IIC_STOP();
			return 1;
		}			
	} 
	//��ȡ���͵�ƽ�Ŷ�
	OLED_IIC_SCL=0; //ʱ����� 0   //ǯλ�������´δ���
	return 0;
}	

//�ӻ�����ѡ�����Ӧ���źţ�ͬ��Ҳ����ѡ�񲻲���Ӧ���źš�
//����Ӧ���źž��Ǵӻ���SDA�����ͣ�������Ӧ���źž���SDA��һֱ���ָߵ�ƽ

//���� ACK Ӧ��
void OLED_IIC_ACK(void)
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
void OLED_IIC_NoACK(void)
{
	OLED_IIC_SCL=0;
	OLED_SDA_OUT();
	OLED_IIC_SDA=1;
	delay_us(2);
	OLED_IIC_SCL=1;
	delay_us(2);
	OLED_IIC_SCL=0;
}


//IIC ����һ���ֽ�
void OLED_IIC_SendByte(u8 txd)
{
	u8 i;
	OLED_SDA_OUT();
	OLED_IIC_SCL = 0;  //����ʱ�ӿ�ʼ���ݴ���
	for(i=0;i<8;i++)
	{
		OLED_IIC_SDA = (txd&0x80) >> 7;
		txd <<= 1;
		delay_us(2);
		OLED_IIC_SCL = 1;
		delay_us(2);
		OLED_IIC_SCL = 0;
		delay_us(2);
	}
}


//�� 1 ���ֽڣ�ack=1 ʱ������ ACK��ack=0������ nACK
u8 OLED_IIC_ReadByte(u8 ack)
{
	u8 i,receive = 0;
	OLED_SDA_IN();
	for(i=0;i<8;i++)
	{
		OLED_IIC_SCL=0; 
		delay_us(2);
		OLED_IIC_SCL=1;
		receive <<= 1;
		if(OLED_IIC_SDA) receive++;
		//receive |= OLED_IIC_SDA;
		delay_us(2);
	}
	if(!ack)
		OLED_IIC_NoACK();
	else
		OLED_IIC_ACK();
		
	return receive;
}

/*****************************************************************************************************/

//д����
void OLED_IIC_Write_Command(u8 command)
{
	OLED_IIC_START();
	OLED_IIC_SendByte(0x78);  //OLED��ַ
	OLED_IIC_Wait_Ack();
	OLED_IIC_SendByte(0x00);  //�Ĵ�����ַ
	OLED_IIC_Wait_Ack();
	OLED_IIC_SendByte(command);
	OLED_IIC_Wait_Ack();
	OLED_IIC_STOP();
}

//д����
void OLED_IIC_Write_Data(u8 data)
{
    OLED_IIC_START();
    OLED_IIC_SendByte(0x78);//OLED��ַ
    OLED_IIC_Wait_Ack();
    OLED_IIC_SendByte(0x40);//�Ĵ�����ַ
    OLED_IIC_Wait_Ack();
    OLED_IIC_SendByte(data);
    OLED_IIC_Wait_Ack();
    OLED_IIC_STOP();
}

/******************************* OLED ���� ****************************************************/

//OLED��ʼ��
void OLED_Init(void)
{																				//��ָ��ʲô��˼���Զ��ղ��� ssd1306ָ����⣩
	delay_ms(100); //�������ʱ����Ҫ
    
    //0720����
    OLED_IIC_GPIO_Init();
    
	OLED_IIC_Write_Command(0xAE); //display off      										 //�ر���ʾ
    OLED_IIC_Write_Command(0x20); //Set Memory Addressing Mode                               //�����ڴ��ַģʽ
    OLED_IIC_Write_Command(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid  //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
    OLED_IIC_Write_Command(0xb0); //Set Page Start Address for Page Addressing Mode,0-7      //����ҳ��ʼ��ַ
    OLED_IIC_Write_Command(0xc0); //Set COM Output Scan Direction                            //����COMɨ�跽��; 0xC0<-->0xC1:��ֱ����   
    OLED_IIC_Write_Command(0x00); //---set low column address            					 //��������ʼ �͵�ַ
    OLED_IIC_Write_Command(0x10); //---set high column address								 //����ҳ��ʼ�ߵ�ַ
    OLED_IIC_Write_Command(0x40); //--set start line address                                 //������ʾ��ʼ�� [5:0],����.
    OLED_IIC_Write_Command(0x81); //--set contrast control register                          //�Աȶ�����
    OLED_IIC_Write_Command(0xff); //���ȵ��� 0x00~0xff                                       //1~255;Ĭ��0X7F (��������,Խ��Խ��
    OLED_IIC_Write_Command(0xa1); //--set segment re-map 0 to 127							 //���ö���ӳ�䣬�ı���Ļ�����е�ַ�Ͷ����������ӳ���ϵ, ����ǿ��OLEDģ����ƵĿ������ԡ�������ֻӰ��������������, �Ѵ洢��GDDRAM�е����ݽ����ֲ���
    OLED_IIC_Write_Command(0xa6); //--set normal display                                     //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
    OLED_IIC_Write_Command(0xa8); //--set multiplex ratio(1 to 64)                           //��������·��
    OLED_IIC_Write_Command(0x3F); //                                                         //Ĭ��0X3F(1/64) 
    OLED_IIC_Write_Command(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
    OLED_IIC_Write_Command(0xd3); //-set display offset                                      //������ʾƫ��
    OLED_IIC_Write_Command(0x00); //-not offset                                              //Ĭ��Ϊ0
    OLED_IIC_Write_Command(0xd5); //--set display clock divide ratio/oscillator frequency    //����ʱ�ӷ�Ƶ����,��Ƶ��
    OLED_IIC_Write_Command(0xf0); //--set divide ratio                                       //[3:0],��Ƶ����;[7:4],��Ƶ��
    OLED_IIC_Write_Command(0xd9); //--set pre-charge period                                  //����Ԥ�������
    OLED_IIC_Write_Command(0x22); //                                                         //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_IIC_Write_Command(0xda); //--set com pins hardware configuration                     //����COMӲ����������
    OLED_IIC_Write_Command(0x12);                                                              //�����ڴ��ַģʽ
    OLED_IIC_Write_Command(0xdb); //--set vcomh                                                //����VCOMH ��ѹ����
    OLED_IIC_Write_Command(0x20); //0x20,0.77xVcc                                              //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
    OLED_IIC_Write_Command(0x8d); //--set DC-DC enable                                         //��ɱ�����
    OLED_IIC_Write_Command(0x14); //                                                           //bit2������/�ر�
	OLED_IIC_Write_Command(0xaf); //--turn on oled panel                                       //������ʾ
}


//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 	

 /**
  * @brief  OLED_ON����OLED�������л���
  * @param  ��
  * @retval ��
  */
void OLED_ON(void)
{
    OLED_IIC_Write_Command(0X8D);  //���õ�ɱ�
    OLED_IIC_Write_Command(0X14);  //������ɱ�
    OLED_IIC_Write_Command(0XAF);  //OLED����
}

//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_IIC_Write_Command(0X8D);  //���õ�ɱ�
    OLED_IIC_Write_Command(0X10);  //������ɱ�
    OLED_IIC_Write_Command(0XAE);  //OLED����
}	


//��������,������,������Ļ�Ǻ�ɫ��  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
			OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//������ʾ
}

/**
  * @brief  OLED_Fill�����������Ļ
  * @param  fill_Data:Ҫ��������
    * @retval ��
  */
void OLED_Fill(u8 fill_Data)//ȫ�����
{
    u8 m,n;
    for(m=0;m<8;m++)
    {
        OLED_IIC_Write_Command(0xb0+m);       //page0-page1
        OLED_IIC_Write_Command(0x00);     //low column start address
        OLED_IIC_Write_Command(0x10);     //high column start address
        for(n=0;n<128;n++)
		{
			OLED_IIC_Write_Data(fill_Data);
		}
    }
}

//������ʼ������
void OLED_SetPos(unsigned char x, unsigned char y) 
{ 
    OLED_IIC_Write_Command(0xB0 + y);
    OLED_IIC_Write_Command(((x & 0xF0) >> 4) | 0x10);
    OLED_IIC_Write_Command((x & 0x0F) | 0x01);
}


//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)
		return;//������Χ��.
	pos = 7-y/8;            //�ڼ�ҳ  ��ʵ�Ƿ�������Ȼ��       Ȼ��ᷭת
	bx = y%8;
	temp = 1<<(7-bx);       //����ʹ��� 1 ������y��ƥ��
	if(t)
		OLED_GRAM[x][pos] |= temp;
	else 
		OLED_GRAM[x][pos] &= ~temp;	
}

//���һ����
//x:0~127
//y:0~63
void OLED_ClearPoint(u8 x,u8 y)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	OLED_GRAM[x][i]|=n;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
}



//����
//x:0~128
//y:0~64
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2)
{
	u8 i,k,k1,k2;
	
	if((x2>128)||(y2>64)||(x1>x2)||(y1>y2))  
//    if((x1<0)||(x2>128)||(y1<0)||(y2>64)||(x1>x2)||(y1>y2)) 
        return ;
	if(x1==x2)    //������
	{
			for(i=0;i<(y2-y1);i++)
			{
				OLED_DrawPoint(x1,y1+i,1);
			}
	}
	else if(y1==y2)   //������
	{
			for(i=0;i<(x2-x1);i++)
			{
				OLED_DrawPoint(x1+i,y1,1);
			}
	}
	else      //��б��
	{
		k1=y2-y1;
		k2=x2-x1;
		k=k1*10/k2;
		for(i=0;i<(x2-x1);i++)
		{
		  OLED_DrawPoint(x1+i,y1+i*k/10,1);
		}
	}
}


//x,y:Բ������
//r:Բ�İ뾶
void OLED_DrawCircle(u8 x,u8 y,u8 r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b,1);
        OLED_DrawPoint(x - a, y - b,1);
        OLED_DrawPoint(x - a, y + b,1);
        OLED_DrawPoint(x + a, y + b,1);
 
        OLED_DrawPoint(x + b, y + a,1);
        OLED_DrawPoint(x + b, y - a,1);
        OLED_DrawPoint(x - b, y - a,1);
        OLED_DrawPoint(x - b, y + a,1);
        
        a++;
        num = (a * a + b * b) - r*r;//���㻭�ĵ���Բ�ĵľ���
        if(num > 0)
        {
            b--;
			a--;
        }
    }
}


//���ֳ�����ȡģ��ʽ��һ���� 6 * 8����һ������8 * 16��
//��һ��˵������8��6�еľ��α����ȡ��������Ҫ���ַ����ڶ���������16��8�еľ��α����ȡ���ַ�(��Ϊoled��ÿһҳֻ��8���У����Ծ���Ҫʹ����һҳ�Ŀռ�)��
//���� 6 * 8�����֣��ַ�����ÿ�������ַ���������12 08 �����ģ�ÿ��12��ʮ����������ÿ����Ϊһ��
//�� 8*16���ֵģ���16 08������ÿһ����16��ʮ����������һ�д���һ��


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//size:ѡ������ 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1)
{
	u8 i,m,temp,size2,chr1;
	u8 y0 = y;
	 size2=( size1/8 + ((size1%8)?1:0) ) * (size1/2);  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr1=chr-' ';  //����ƫ�ƺ��ֵ
	for(i=0;i<size2;i++)
	{
		if(size1==12)
        {temp = asc2_1206[chr1][i];} //����1206����
		else if(size1==16)
        {temp = asc2_1608[chr1][i];} //����1608����
		else if(size1==24)
        {temp = asc2_2412[chr1][i];} //����2412����
		else return;
		for(m=0;m<8;m++)           //д������
		{
			if(temp&0x80)
			{
				OLED_DrawPoint(x,y,1);
			}
			else 
				OLED_ClearPoint(x,y);
			temp <<= 1;
			y++;
			if( (y-y0)==size1 )
			{
				y = y0;
				x++;
				break;
			}
		}
	}	    
}


//��ʾ�ַ���
//x,y:�������  
//size1:�����С 
//*chr:�ַ�����ʼ��ַ 
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1)
{
	while((*chr>=' ')&&(*chr<='~'))//�ж��ǲ��ǷǷ��ַ�!
	{
		OLED_ShowChar(x,y,*chr,size1);
		x += size1 / 2;
		if(x > 128-size1)  //����
		{
			x=0;
			y += size1;
		}
		chr++;
  }
}

//m^n
u32 OLED_Pow(u8 m,u8 n)   //m��n�η�
{
	u32 result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1)
{
	u8 t,temp;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
			if(temp==0)
			{
				OLED_ShowChar(x+(size1/2)*t,y,'0',size1);
      }
			else 
			{
			  OLED_ShowChar(x+(size1/2)*t,y,temp+'0',size1);
			}
  }
}

//��ʾ����
//x,y:�������
//num:���ֶ�Ӧ�����
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1)
{
	u8 i,m,n=0,temp,chr1;
	u8 x0=x,y0=y;
	u8 size3=size1/8;
	while(size3--)
	{
		chr1=num*size1/8+n;
		n++;
			for(i=0;i<size1;i++)
			{
				if(size1==16)
				{temp=Hzk1[chr1][i];}//����16*16����
				else if(size1==24)
				{temp=Hzk2[chr1][i];}//����24*24����
				else if(size1==32)       
				{temp=Hzk3[chr1][i];}//����32*32����
				else if(size1==64)
				{temp=Hzk4[chr1][i];}//����64*64����
				else return;
							
				for(m=0;m<8;m++)
				{
					if(temp&0x01) OLED_DrawPoint(x,y,1);
					else OLED_ClearPoint(x,y);
					temp >>= 1;
					y++;
				}
				x++;
				if((x-x0)==size1)
				{
					x=x0;
					y0=y0+8;
				}
				y=y0;
			 }
	}
}

//num ��ʾ���ֵĸ���
//space ÿһ����ʾ�ļ��
void OLED_ScrollDisplay(u8 num,u8 space)
{
	u8 i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
			OLED_ShowChinese(128,24,t,16); //д��һ�����ֱ�����OLED_GRAM[][]������
			t++;
		}
		if(t==num)
		{
			for(r=0;r<16*space;r++)      //��ʾ���
			{
				for(i=0;i<144;i++)
				{
					for(n=0;n<8;n++)
					{
						OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
					}
				}
				OLED_Refresh_Gram();
			 }
			t = 0;
		}
		m++;
		if(m==16){ m=0;}
		for(i=0;i<144;i++)   //ʵ������
		{
			for(n=0;n<8;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh_Gram();
	}
}						
						
//x0,y0���������
//x1,y1���յ�����
//BMP[]��Ҫд���ͼƬ����
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[])
{
    u32 j=0;
    u8 x=0,y=0;
    if(y%8==0)y=0;
    else y+=1;
    for(y=y0;y<y1;y++)
    {
        OLED_SetPos(x0,y);
        for(x=x0;x<x1;x++)
        {
            OLED_IIC_Write_Data(BMP[j]);
            j++;
        }
    }
}	

void OLED_Test(void)
{
    OLED_ShowString(0,0,(u8 *)"ABCDEFGHIJKLMNO",OLED_FONTSIZE);  //������
    OLED_ShowString(0,16,(u8 *)"ABCDEFGHIJKLMNO",OLED_FONTSIZE);  //������
    OLED_ShowString(0,32,(u8 *)"IIC SOFT",OLED_FONTSIZE);  //������
    OLED_ShowString(0,48,(u8 *)"2023.7.21",OLED_FONTSIZE);  //������
    OLED_Refresh_Gram();
}

#endif
