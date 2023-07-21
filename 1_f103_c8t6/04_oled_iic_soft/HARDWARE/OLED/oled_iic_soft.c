#include "stm32f10x.h"
#include "oled_iic_soft.h"
#include "delay.h"
#ifdef _OLED_IIC_SOFT_NOGRAM_
#include "oled_soft_font.h"
/***********************************  重写IIC时序  *****************************************************/
//初始化GPIO
void OLED_IIC_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //GPIO_Mode_Out_OD       //软件模拟IIC配置成推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化 GPIO
	GPIO_SetBits(GPIOC,GPIO_Pin_14|GPIO_Pin_15); 
	
//	PWR_BackupAccessCmd(ENABLE);//允许修改RTC 和后备寄存器
//	RCC_LSEConfig(RCC_LSE_OFF);//关闭外部低速外部时钟信号功能 后，PC13 PC14 PC15 才可以当普通IO用。
//	BKP_TamperPinCmd(DISABLE);//关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用
//	PWR_BackupAccessCmd(DISABLE);//禁止修改后备寄存器
	
}

//起始信号
void OLED_IIC_Start(void)
{
	OLED_SDA_OUT();
	OLED_IIC_SCL = 1;
	OLED_IIC_SDA = 1;		//START:when CLK is high,DATA change form high to low
	delay_us(4);
	OLED_IIC_SDA = 0;
	delay_us(4);
	OLED_IIC_SCL = 0;		//钳住 I2C 总线，准备发送或接收数据
}

//终止信号
void OLED_IIC_Stop(void)
{
	OLED_SDA_OUT();
	OLED_IIC_SCL = 0;
	OLED_IIC_SDA = 0;		//STOP:when CLK is high DATA change form low to high
	delay_us(4);
	OLED_IIC_SCL = 1;
	OLED_IIC_SDA = 1;		//发送 I2C 总线结束信号
	delay_us(4);
}

//等待应答信号到来
//返回值：1，接收应答失败
// 0，接收应答成功
u8 OLED_IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	OLED_SDA_IN();		//SDA 设置为输入
	OLED_IIC_SDA=1;delay_us(1);    //OLED_I2C_SDA=1;是 BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 ，直接操作ODR，不需要配置输入输出
	//程序中需要注意的地方是，虽然设置了GPIO的方向为输入，但是GPIO的电平还是可以设置的，没有矛盾，GPIO方向由CRL  CRH来决定，输出电平由ODR寄存器来决定
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
	//读取到低电平才对
	OLED_IIC_SCL=0; //时钟输出 0   //钳位，方便下次传输
	return 0;
}	


/************************ 用不到这两个函数**********************************/
//从机可以选择产生应答信号，同样也可以选择不产生应答信号。
//产生应答信号就是从机将SDA线拉低，不产生应答信号就是SDA线一直保持高电平

//产生 ACK 应答
void OLED_I2C_ACK(void)
{
	//此时主机相当于在接收数据，是被动方
	OLED_IIC_SCL=0;
	OLED_SDA_OUT();
	OLED_IIC_SDA=0;
	delay_us(2);
	OLED_IIC_SCL=1;
	delay_us(2);
	OLED_IIC_SCL=0;
}

//不产生 ACK 应答
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
/************************ 用不到上面这两个函数**********************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	u8 i;
	OLED_SDA_OUT();
	OLED_IIC_SCL = 0;  //拉低时钟开始数据传输
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
	//不加这一段就无法写成功，应该是因为第九个时序的问题
	OLED_IIC_SCL = 1;
	delay_us(2);
	OLED_IIC_SCL = 0;
	delay_us(2);
}

/*********************OLED写数据************************************/ 
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
/*********************OLED写命令************************************/
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

//以上见https://blog.csdn.net/lgyLGY35/article/details/118693991

//以下见https://blog.csdn.net/lgyLGY35/article/details/118655290

/***************************************************************************************************************************/
								/********************* OLED显示函数 *****************/

/*********************OLED 设置坐标************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
} 
/*********************OLED全屏************************************/
//这个函数写的时候，形参bmp_dat是一个八位二进制数，也就是在一行中的一列的8个点中有几个亮，所以这个没有参考表
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
/*********************OLED复位************************************/
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
// Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	OLED_WrCmd(0X8D);  //设置电荷泵
	OLED_WrCmd(0X10);  //关闭电荷泵
	OLED_WrCmd(0XAE);  //OLED休眠
}


//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : 将OLED从休眠中唤醒
//--------------------------------------------------------------
void OLED_ON(void)
{
	OLED_WrCmd(0X8D);  //设置电荷泵
	OLED_WrCmd(0X14);  //开启电荷泵
	OLED_WrCmd(0XAF);  //OLED唤醒
}


/*********************OLED初始化************************************/
void OLED_Init(void)
{
	delay_ms(500);//初始化之前的延时很重要！
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(Brightness); // Set SEG Output Current Brightness 亮度调节 0x00~0xff
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
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
	OLED_Fill(0x00); //初始清屏
	OLED_Set_Pos(0,0);
} 

/***************功能描述：显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7****************/
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;   //减去空格的ASCII码，得到在码表中的偏移量
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

/*******************功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（x,y），y为页范围0～7****************/
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
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); N:汉字在codetab.h中的索引
// Description    : 显示codetab.h中的汉字,16*16点阵
//--------------------------------------------------------------

/*****************功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7****************************/
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
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
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
