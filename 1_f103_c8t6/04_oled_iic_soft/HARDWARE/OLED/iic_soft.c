
/**************************实现函数********************************************
*函数原型:		void IIC_Start(void)
*功　　能:		产生IIC起始信号
*******************************************************************************/
//void OLED_IIC_Start(void)
//{
//	//SDA_OUT();     //sda线输出
//	OLED_IIC_SDA=1;
//	if(!OLED_WRITE_SDA)//return 0;	
//	OLED_IIC_SCL=1;
//	delay_us(1);
// 	OLED_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
//	if(OLED_WRITE_SDA)//return 0;
//	delay_us(1);
//	OLED_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
//	//return 1;
//}

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

/**************************实现函数********************************************
*函数原型:		void IIC_Stop(void)
*功　　能:	    //产生IIC停止信号
*******************************************************************************/	  
//void  OLED_IIC_Stop(void)
//{
//	//SDA_OUT();//sda线输出
//	OLED_IIC_SCL=0;
//	OLED_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
// 	delay_us(1);
//	OLED_IIC_SCL=1; 
//	OLED_IIC_SDA=1;//发送I2C总线结束信号
//	delay_us(1);							   	
//}

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

//写一个字节的函数      //第九个时序用SCL或者wait_ack(回看博客)
void Write_IIC_Byte(unsigned char IIC_Byte)
{
//	unsigned char i;
//  for(i=0;i<8;i++)
//	{
//		if(IIC_Byte & 0x80)
//			OLED_IIC_SDA=1;
//		else
//			OLED_IIC_SDA=0;
//			OLED_IIC_SCL=1;
//      delay_us(1);  //必须有保持SCL脉冲的延时
//			OLED_IIC_SCL=0;
//			IIC_Byte<<=1;
//	}
//			OLED_IIC_SDA = 1;//原程序这里有一个拉高SDA，根据OLED的DATASHEET，此句必须去掉。
//			OLED_IIC_SCL=1;
//			delay_us(1);
//			OLED_IIC_SCL=0;
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
//	OLED_IIC_SCL = 1;
//	delay_us(2);
//	OLED_IIC_SCL = 0
//	delay_us(2);
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

