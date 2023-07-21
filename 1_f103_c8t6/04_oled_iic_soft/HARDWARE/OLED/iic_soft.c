
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Start(void)
*��������:		����IIC��ʼ�ź�
*******************************************************************************/
//void OLED_IIC_Start(void)
//{
//	//SDA_OUT();     //sda�����
//	OLED_IIC_SDA=1;
//	if(!OLED_WRITE_SDA)//return 0;	
//	OLED_IIC_SCL=1;
//	delay_us(1);
// 	OLED_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
//	if(OLED_WRITE_SDA)//return 0;
//	delay_us(1);
//	OLED_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
//	//return 1;
//}

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

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Stop(void)
*��������:	    //����IICֹͣ�ź�
*******************************************************************************/	  
//void  OLED_IIC_Stop(void)
//{
//	//SDA_OUT();//sda�����
//	OLED_IIC_SCL=0;
//	OLED_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
// 	delay_us(1);
//	OLED_IIC_SCL=1; 
//	OLED_IIC_SDA=1;//����I2C���߽����ź�
//	delay_us(1);							   	
//}

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

//дһ���ֽڵĺ���      //�ھŸ�ʱ����SCL����wait_ack(�ؿ�����)
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
//      delay_us(1);  //�����б���SCL�������ʱ
//			OLED_IIC_SCL=0;
//			IIC_Byte<<=1;
//	}
//			OLED_IIC_SDA = 1;//ԭ����������һ������SDA������OLED��DATASHEET���˾����ȥ����
//			OLED_IIC_SCL=1;
//			delay_us(1);
//			OLED_IIC_SCL=0;
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
//	OLED_IIC_SCL = 1;
//	delay_us(2);
//	OLED_IIC_SCL = 0
//	delay_us(2);
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

