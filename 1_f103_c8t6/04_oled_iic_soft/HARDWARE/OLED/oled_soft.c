
#include "stm32f10x.h"
#include "oled_soft.h"
#include "delay.h"
//#include "usart.h"
#ifdef _OLED_IIC_SOFT_GRAM_
#include "oled_soft_font.h"

//https://blog.csdn.net/lgyLGY35/article/details/118693991

//这里用的是一种更新GRAM，实际上就是SRAM的方式来修改显示的内容，因为OLED模块不支持先读后写，所以每次写的都存在GRAM中
//（看博客）

u8 OLED_GRAM[128][8];

void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_IIC_Write_Command(0xb0+i);  //设置页地址（0~7）
		OLED_IIC_Write_Command(0x00);    //设置显示位置—列低地址
		OLED_IIC_Write_Command(0x10);    //设置显示位置—列高地址	
		for(n=0;n<128;n++)
			OLED_IIC_Write_Data(OLED_GRAM[n][i]); 
	}   
}


/***********************************  重写IIC时序  *****************************************************/
//初始化GPIO
void OLED_IIC_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(OLED_RCC_APB2Periph_GPIO | RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Pin = OLED_4PIN_SCL|OLED_4PIN_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //GPIO_Mode_Out_OD       //软件模拟IIC配置成推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(OLED_GPIO, &GPIO_InitStructure); //初始化 GPIO
	GPIO_SetBits(OLED_GPIO, OLED_4PIN_SCL|OLED_4PIN_SDA); 
	
//	PWR_BackupAccessCmd(ENABLE);//允许修改RTC 和后备寄存器
//	RCC_LSEConfig(RCC_LSE_OFF);//关闭外部低速外部时钟信号功能 后，PC13 PC14 PC15 才可以当普通IO用。
//	BKP_TamperPinCmd(DISABLE);//关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用
//	PWR_BackupAccessCmd(DISABLE);//禁止修改后备寄存器
	
}

//起始信号
void OLED_IIC_START(void)
{
	OLED_SDA_OUT();
	OLED_IIC_SCL = 1;
	OLED_IIC_SDA = 1;		//START:when CLK is high,DATA change form high to low
	delay_us(4);
	OLED_IIC_SDA = 0;
	delay_us(4);
	OLED_IIC_SCL = 0;		//钳住 IIC 总线，准备发送或接收数据
}

//终止信号
void OLED_IIC_STOP(void)
{
	OLED_SDA_OUT();
	OLED_IIC_SCL = 0;
	OLED_IIC_SDA = 0;		//STOP:when CLK is high DATA change form low to high
	delay_us(4);
	OLED_IIC_SCL = 1;
	OLED_IIC_SDA = 1;		//发送 IIC 总线结束信号
	delay_us(4);
}



//等待应答信号到来
//返回值：1，接收应答失败
// 0，接收应答成功
u8 OLED_IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	OLED_SDA_IN();		//SDA 设置为输入
	OLED_IIC_SDA=1;delay_us(1);    //OLED_IIC_SDA=1;是 BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 ，直接操作ODR，不需要配置输入输出
	//程序中需要注意的地方是，虽然设置了GPIO的方向为输入，但是GPIO的电平还是可以设置的，没有矛盾，GPIO方向由CRL  CRH来决定，输出电平由ODR寄存器来决定
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
	//读取到低电平才对
	OLED_IIC_SCL=0; //时钟输出 0   //钳位，方便下次传输
	return 0;
}	

//从机可以选择产生应答信号，同样也可以选择不产生应答信号。
//产生应答信号就是从机将SDA线拉低，不产生应答信号就是SDA线一直保持高电平

//产生 ACK 应答
void OLED_IIC_ACK(void)
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


//IIC 发送一个字节
void OLED_IIC_SendByte(u8 txd)
{
	u8 i;
	OLED_SDA_OUT();
	OLED_IIC_SCL = 0;  //拉低时钟开始数据传输
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


//读 1 个字节，ack=1 时，发送 ACK，ack=0，发送 nACK
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

//写命令
void OLED_IIC_Write_Command(u8 command)
{
	OLED_IIC_START();
	OLED_IIC_SendByte(0x78);  //OLED地址
	OLED_IIC_Wait_Ack();
	OLED_IIC_SendByte(0x00);  //寄存器地址
	OLED_IIC_Wait_Ack();
	OLED_IIC_SendByte(command);
	OLED_IIC_Wait_Ack();
	OLED_IIC_STOP();
}

//写数据
void OLED_IIC_Write_Data(u8 data)
{
    OLED_IIC_START();
    OLED_IIC_SendByte(0x78);//OLED地址
    OLED_IIC_Wait_Ack();
    OLED_IIC_SendByte(0x40);//寄存器地址
    OLED_IIC_Wait_Ack();
    OLED_IIC_SendByte(data);
    OLED_IIC_Wait_Ack();
    OLED_IIC_STOP();
}

/******************************* OLED 函数 ****************************************************/

//OLED初始化
void OLED_Init(void)
{																				//（指令什么意思可以对照博客 ssd1306指令详解）
	delay_ms(100); //这里的延时很重要
    
    //0720补充
    OLED_IIC_GPIO_Init();
    
	OLED_IIC_Write_Command(0xAE); //display off      										 //关闭显示
    OLED_IIC_Write_Command(0x20); //Set Memory Addressing Mode                               //设置内存地址模式
    OLED_IIC_Write_Command(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid  //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
    OLED_IIC_Write_Command(0xb0); //Set Page Start Address for Page Addressing Mode,0-7      //设置页起始地址
    OLED_IIC_Write_Command(0xc0); //Set COM Output Scan Direction                            //设置COM扫描方向; 0xC0<-->0xC1:垂直反向   
    OLED_IIC_Write_Command(0x00); //---set low column address            					 //设置列起始 低地址
    OLED_IIC_Write_Command(0x10); //---set high column address								 //设置页起始高地址
    OLED_IIC_Write_Command(0x40); //--set start line address                                 //设置显示开始行 [5:0],行数.
    OLED_IIC_Write_Command(0x81); //--set contrast control register                          //对比度设置
    OLED_IIC_Write_Command(0xff); //亮度调节 0x00~0xff                                       //1~255;默认0X7F (亮度设置,越大越亮
    OLED_IIC_Write_Command(0xa1); //--set segment re-map 0 to 127							 //设置段重映射，改变屏幕数据列地址和段驱动器间的映射关系, 这增强和OLED模块设计的可伸缩性。此命令只影响其后的数据输入, 已存储在GDDRAM中的数据将保持不变
    OLED_IIC_Write_Command(0xa6); //--set normal display                                     //设置显示方式;bit0:1,反相显示;0,正常显示
    OLED_IIC_Write_Command(0xa8); //--set multiplex ratio(1 to 64)                           //设置驱动路数
    OLED_IIC_Write_Command(0x3F); //                                                         //默认0X3F(1/64) 
    OLED_IIC_Write_Command(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
    OLED_IIC_Write_Command(0xd3); //-set display offset                                      //设置显示偏移
    OLED_IIC_Write_Command(0x00); //-not offset                                              //默认为0
    OLED_IIC_Write_Command(0xd5); //--set display clock divide ratio/oscillator frequency    //设置时钟分频因子,震荡频率
    OLED_IIC_Write_Command(0xf0); //--set divide ratio                                       //[3:0],分频因子;[7:4],震荡频率
    OLED_IIC_Write_Command(0xd9); //--set pre-charge period                                  //设置预充电周期
    OLED_IIC_Write_Command(0x22); //                                                         //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_IIC_Write_Command(0xda); //--set com pins hardware configuration                     //设置COM硬件引脚配置
    OLED_IIC_Write_Command(0x12);                                                              //设置内存地址模式
    OLED_IIC_Write_Command(0xdb); //--set vcomh                                                //设置VCOMH 电压倍率
    OLED_IIC_Write_Command(0x20); //0x20,0.77xVcc                                              //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
    OLED_IIC_Write_Command(0x8d); //--set DC-DC enable                                         //电荷泵设置
    OLED_IIC_Write_Command(0x14); //                                                           //bit2，开启/关闭
	OLED_IIC_Write_Command(0xaf); //--turn on oled panel                                       //开启显示
}


//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 	

 /**
  * @brief  OLED_ON，将OLED从休眠中唤醒
  * @param  无
  * @retval 无
  */
void OLED_ON(void)
{
    OLED_IIC_Write_Command(0X8D);  //设置电荷泵
    OLED_IIC_Write_Command(0X14);  //开启电荷泵
    OLED_IIC_Write_Command(0XAF);  //OLED唤醒
}

//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_IIC_Write_Command(0X8D);  //设置电荷泵
    OLED_IIC_Write_Command(0X10);  //开启电荷泵
    OLED_IIC_Write_Command(0XAE);  //OLED唤醒
}	


//清屏函数,清完屏,整个屏幕是黑色的  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
			OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//更新显示
}

/**
  * @brief  OLED_Fill，填充整个屏幕
  * @param  fill_Data:要填充的数据
    * @retval 无
  */
void OLED_Fill(u8 fill_Data)//全屏填充
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

//设置起始点坐标
void OLED_SetPos(unsigned char x, unsigned char y) 
{ 
    OLED_IIC_Write_Command(0xB0 + y);
    OLED_IIC_Write_Command(((x & 0xF0) >> 4) | 0x10);
    OLED_IIC_Write_Command((x & 0x0F) | 0x01);
}


//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)
		return;//超出范围了.
	pos = 7-y/8;            //第几页  其实是反过来，然后       然后会翻转
	bx = y%8;
	temp = 1<<(7-bx);       //这里使这个 1 与具体的y相匹配
	if(t)
		OLED_GRAM[x][pos] |= temp;
	else 
		OLED_GRAM[x][pos] &= ~temp;	
}

//清除一个点
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



//画线
//x:0~128
//y:0~64
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2)
{
	u8 i,k,k1,k2;
	
	if((x2>128)||(y2>64)||(x1>x2)||(y1>y2))  
//    if((x1<0)||(x2>128)||(y1<0)||(y2>64)||(x1>x2)||(y1>y2)) 
        return ;
	if(x1==x2)    //画竖线
	{
			for(i=0;i<(y2-y1);i++)
			{
				OLED_DrawPoint(x1,y1+i,1);
			}
	}
	else if(y1==y2)   //画横线
	{
			for(i=0;i<(x2-x1);i++)
			{
				OLED_DrawPoint(x1+i,y1,1);
			}
	}
	else      //画斜线
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


//x,y:圆心坐标
//r:圆的半径
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
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
			a--;
        }
    }
}


//两种常见的取模方式，一个是 6 * 8，另一个则是8 * 16的
//第一个说的是在8行6列的矩形表格中取出我们想要的字符，第二个则是在16行8列的矩形表格中取出字符(因为oled中每一页只有8个行，所以就需要使用下一页的空间)。
//对于 6 * 8的这种，字符表是每行两个字符，所以是12 08 这样的，每行12个十六进制数，每六个为一个
//而 8*16这种的，是16 08这样，每一行有16个十六进制数，一行代表一个


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size:选择字体 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1)
{
	u8 i,m,temp,size2,chr1;
	u8 y0 = y;
	 size2=( size1/8 + ((size1%8)?1:0) ) * (size1/2);  //得到字体一个字符对应点阵集所占的字节数
	chr1=chr-' ';  //计算偏移后的值
	for(i=0;i<size2;i++)
	{
		if(size1==12)
        {temp = asc2_1206[chr1][i];} //调用1206字体
		else if(size1==16)
        {temp = asc2_1608[chr1][i];} //调用1608字体
		else if(size1==24)
        {temp = asc2_2412[chr1][i];} //调用2412字体
		else return;
		for(m=0;m<8;m++)           //写入数据
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


//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
	{
		OLED_ShowChar(x,y,*chr,size1);
		x += size1 / 2;
		if(x > 128-size1)  //换行
		{
			x=0;
			y += size1;
		}
		chr++;
  }
}

//m^n
u32 OLED_Pow(u8 m,u8 n)   //m的n次方
{
	u32 result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
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

//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
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
				{temp=Hzk1[chr1][i];}//调用16*16字体
				else if(size1==24)
				{temp=Hzk2[chr1][i];}//调用24*24字体
				else if(size1==32)       
				{temp=Hzk3[chr1][i];}//调用32*32字体
				else if(size1==64)
				{temp=Hzk4[chr1][i];}//调用64*64字体
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

//num 显示汉字的个数
//space 每一遍显示的间隔
void OLED_ScrollDisplay(u8 num,u8 space)
{
	u8 i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
			OLED_ShowChinese(128,24,t,16); //写入一个汉字保存在OLED_GRAM[][]数组中
			t++;
		}
		if(t==num)
		{
			for(r=0;r<16*space;r++)      //显示间隔
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
		for(i=0;i<144;i++)   //实现左移
		{
			for(n=0;n<8;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh_Gram();
	}
}						
						
//x0,y0：起点坐标
//x1,y1：终点坐标
//BMP[]：要写入的图片数组
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
    OLED_ShowString(0,0,(u8 *)"ABCDEFGHIJKLMNO",OLED_FONTSIZE);  //第四行
    OLED_ShowString(0,16,(u8 *)"ABCDEFGHIJKLMNO",OLED_FONTSIZE);  //第四行
    OLED_ShowString(0,32,(u8 *)"IIC SOFT",OLED_FONTSIZE);  //第四行
    OLED_ShowString(0,48,(u8 *)"2023.7.21",OLED_FONTSIZE);  //第四行
    OLED_Refresh_Gram();
}

#endif
