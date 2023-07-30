#include "vl53l0x.h"
#include "GUI.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK MiniV3 STM32������
//VL53L0X-���ܲ��� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/7/1
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

VL53L0X_Dev_t vl53l0x_dev;//�豸I2C���ݲ���
VL53L0X_DeviceInfo_t vl53l0x_dev_info;//�豸ID�汾��Ϣ
uint8_t AjustOK=0;//У׼��־λ

//VL53L0X������ģʽ����
//0��Ĭ��;1:�߾���;2:������;3:����
mode_data Mode_data[]=
{
    {(FixPoint1616_t)(0.25*65536), 
	 (FixPoint1616_t)(18*65536),
	 33000,
	 14,
	 10},//Ĭ��
		
	{(FixPoint1616_t)(0.25*65536) ,
	 (FixPoint1616_t)(18*65536),
	 200000, 
	 14,
	 10},//�߾���
		
    {(FixPoint1616_t)(0.1*65536) ,
	 (FixPoint1616_t)(60*65536),
	 33000,
	 18,
	 14},//������
	
    {(FixPoint1616_t)(0.25*65536) ,
	 (FixPoint1616_t)(32*65536),
	 20000,
	 14,
	 10},//����
		
};

//API������Ϣ��ӡ
//Status�����鿴VL53L0X_Error�����Ķ���
void print_pal_error(VL53L0X_Error Status)
{
	
	char buf[VL53L0X_MAX_STRING_LENGTH];
	
	VL53L0X_GetPalErrorString(Status,buf);//����Status״̬��ȡ������Ϣ�ַ���
	
    printf("API Status: %i : %s\r\n",Status, buf);//��ӡ״̬�ʹ�����Ϣ
	
}

//ģʽ�ַ�����ʾ
//mode:0-Ĭ��;1-�߾���;2-������;3-����
void mode_string(u8 mode,char *buf)
{
	switch(mode)
	{
		case Default_Mode: strcpy(buf,"Default");        break;
		case HIGH_ACCURACY: strcpy(buf,"High Accuracy"); break;
		case LONG_RANGE: strcpy(buf,"Long Range");       break;
		case HIGH_SPEED: strcpy(buf,"High Speed");       break;
	}

}

//����VL53L0X�豸I2C��ַ
//dev:�豸I2C�����ṹ��
//newaddr:�豸��I2C��ַ
VL53L0X_Error vl53l0x_Addr_set(VL53L0X_Dev_t *dev,uint8_t newaddr)
{
	uint16_t Id;
	uint8_t FinalAddress;
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;
	u8 sta=0x00;
	
	FinalAddress = newaddr;
	
	if(FinalAddress==dev->I2cDevAddr)//���豸I2C��ַ��ɵ�ַһ��,ֱ���˳�
		return VL53L0X_ERROR_NONE;
	//�ڽ��е�һ���Ĵ�������֮ǰ����I2C��׼ģʽ(400Khz)
	Status = VL53L0X_WrByte(dev,0x88,0x00);
	if(Status!=VL53L0X_ERROR_NONE) 
	{
		sta=0x01;//����I2C��׼ģʽ����
		goto set_error;
	}
	//����ʹ��Ĭ�ϵ�0x52��ַ��ȡһ���Ĵ���
	Status = VL53L0X_RdWord(dev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &Id);
	if(Status!=VL53L0X_ERROR_NONE) 
	{
		sta=0x02;//��ȡ�Ĵ�������
		goto set_error;
	}
	if(Id == 0xEEAA)
	{
		//�����豸�µ�I2C��ַ
		Status = VL53L0X_SetDeviceAddress(dev,FinalAddress);
		if(Status!=VL53L0X_ERROR_NONE) 
		{
			sta=0x03;//����I2C��ַ����
			goto set_error;
		}
		//�޸Ĳ����ṹ���I2C��ַ
		dev->I2cDevAddr = FinalAddress;
		//����µ�I2C��ַ��д�Ƿ�����
		Status = VL53L0X_RdWord(dev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &Id);
		if(Status!=VL53L0X_ERROR_NONE) 
		{
			sta=0x04;//��I2C��ַ��д����
			goto set_error;
		}	
	}
	set_error:
	if(Status!=VL53L0X_ERROR_NONE)
	{
		print_pal_error(Status);//��ӡ������Ϣ
	}
	if(sta!=0)
	  printf("sta:0x%x\r\n",sta);
	return Status;
}

//vl53l0x��λ����
//dev:�豸I2C�����ṹ��
void vl53l0x_reset(VL53L0X_Dev_t *dev)
{
	uint8_t addr;
	addr = dev->I2cDevAddr;//�����豸ԭI2C��ַ
    VL53L0X_Xshut=0;//ʧ��VL53L0X
	delay_ms(30);
	VL53L0X_Xshut=1;//ʹ��VL53L0X,�ô��������ڹ���(I2C��ַ��ָ�Ĭ��0X52)
	delay_ms(30);	
	dev->I2cDevAddr=0x52;
	vl53l0x_Addr_set(dev,addr);//����VL53L0X������ԭ���ϵ�ǰԭI2C��ַ
	VL53L0X_DataInit(dev);	
}

//��ʼ��vl53l0x
//dev:�豸I2C�����ṹ��
//��ΪGPIO_INIT������
VL53L0X_Error vl53l0x_init(VL53L0X_Dev_t *dev)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;
	VL53L0X_Dev_t *pMyDevice = dev;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //��ʹ������IO PORTAʱ�� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	           // �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);				   //�����趨������ʼ��GPIOA

	pMyDevice->I2cDevAddr = VL53L0X_Addr;//I2C��ַ(�ϵ�Ĭ��0x52)
	pMyDevice->comms_type = 1;           //I2Cͨ��ģʽ
	pMyDevice->comms_speed_khz = 400;    //I2Cͨ������
	
	VL53L0X_i2c_init();//��ʼ��IIC����
	
	VL53L0X_Xshut=0;//ʧ��VL53L0X
	delay_ms(30);
	VL53L0X_Xshut=1;//ʹ��VL53L0X,�ô��������ڹ���
	delay_ms(30);
	
    vl53l0x_Addr_set(pMyDevice,0x54);//����VL53L0X������I2C��ַ
    if(Status!=VL53L0X_ERROR_NONE) goto error;
	Status = VL53L0X_DataInit(pMyDevice);//�豸��ʼ��
	if(Status!=VL53L0X_ERROR_NONE) goto error;
	delay_ms(2);
	Status = VL53L0X_GetDeviceInfo(pMyDevice,&vl53l0x_dev_info);//��ȡ�豸ID��Ϣ
    if(Status!=VL53L0X_ERROR_NONE) goto error;
		
	  AjustOK=0;
	
	error:
	if(Status!=VL53L0X_ERROR_NONE)
	{
		print_pal_error(Status);//��ӡ������Ϣ
		return Status;
	}
  	
	return Status;
}

//20230730������װ
void VL53L0X_MainInit(u8 mode)
{
	uint8_t data;
	uint8_t len;
	uint16_t word;
	uint8_t data1;
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    
    if(vl53l0x_init(&vl53l0x_dev)) {    //vl53l0x��ʼ��
        printf("VL53L0X_Init Error!!!\r\n");
        delay_ms(200);
    } else {        
        printf("VL53L0X_Init OK\r\n");

        VL53L0X_RdByte(&vl53l0x_dev,0xC0,&data);
        printf("regc0 = 0x%x\n\r",data);

        VL53L0X_RdByte(&vl53l0x_dev,0xC1,&data1);
        printf("regc1 = 0x%x\n\r",data1);

        VL53L0X_RdByte(&vl53l0x_dev,0xC2,&len);
        printf("regc2 = 0x%x\n\r",len); 

        Status = VL53L0X_WrByte(&vl53l0x_dev,0x51,0x12);
        VL53L0X_RdByte(&vl53l0x_dev,0x51,&len);  //����λû��������
        printf("reg51 = 0x%x, status = %d\n\r", len, Status);

        Status = VL53L0X_WrWord(&vl53l0x_dev,0x62,0x9e12);
        VL53L0X_RdWord(&vl53l0x_dev,0x62,&word);
        printf("reg61 = 0x%x, status = %d\n\r", word, Status);
    }
	
	if(vl53l0x_set_mode(&vl53l0x_dev,mode)) {  //���ò���ģʽ
        printf("Mode Set Error!!!\r\n");
    } else
		printf("Mode Set OK!!!\r\n");    
    
}

//20230730������װ
u32 VL53L0X_GetDistance(void)
{
    uint8_t range[20];
    u32 distance = 0;
    
	static char buf[VL53L0X_MAX_STRING_LENGTH];//����ģʽ�ַ����ַ�������       
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;//����״̬
    
    Status = vl53l0x_start_single_test(&vl53l0x_dev,&vl53l0x_data,buf);//ִ��һ�β���
    if(Status==VL53L0X_ERROR_NONE) {
        // ģ��δУ�������(δʹ��vl53l0x_cali.c)���ֶ�����30mm.��ʱ����20230730
        if (Distance_data > 30) {
            Distance_data -= 30;
        }
        
        sprintf((char*)range,"%4d mm", Distance_data);   
        Gui_DrawFont_GBK16(0,64,BLUE,GRAY0, range);
        printf("d: %4imm\r\n",Distance_data);//��ӡ��������
        distance = (u32)Distance_data;
    } else {
        distance = 99999; // ����һ������ֵ
        printf("Measurement is Error!!!\r\n");
    }
    return distance;
}

/*
//VL53L0X�����Գ���
void vl53l0x_test(void)
{
	 while(vl53l0x_init(&vl53l0x_dev))//vl53l0x��ʼ��
	 {
		printf("VL53L0X_Init Error!!!\r\n");
		delay_ms(500);
	 }
	 printf("VL53L0X_Init OK\r\n");
	 
	 while(1)
	 {
		 //	vl53l0x_calibration_test(&vl53l0x_dev);   break;//У׼ģʽ
		 //vl53l0x_general_test(&vl53l0x_dev);       break;//��ͨ����ģʽ
		 //vl53l0x_interrupt_test(&vl53l0x_dev);     break;//�жϲ���ģʽ  
		 vl53l0x_general_test(&vl53l0x_dev);
		 delay_ms(500);
	 }
}
*/
//----------���º���ΪUSMART����------------//

//��ȡvl53l0x������ID��Ϣ
void vl53l0x_info(void)
{
	printf("\r\n-------vl53l0x�������豸��Ϣ-------\r\n\r\n");
	printf("  Name:%s\r\n",vl53l0x_dev_info.Name);
	printf("  Addr:0x%x\r\n",vl53l0x_dev.I2cDevAddr);
	printf("  ProductId:%s\r\n",vl53l0x_dev_info.ProductId);
	printf("  RevisionMajor:0x%x\r\n",vl53l0x_dev_info.ProductRevisionMajor);
	printf("  RevisionMinor:0x%x\r\n",vl53l0x_dev_info.ProductRevisionMinor);
	printf("\r\n-----------------------------------\r\n");
}

//��ȡһ�β�����������
//modeģʽ���� 0:Ĭ��;1:�߾���;2:������;3:����
void One_measurement(u8 mode)
{
	vl53l0x_set_mode(&vl53l0x_dev,mode);
	VL53L0X_PerformSingleRangingMeasurement(&vl53l0x_dev,&vl53l0x_data);
	printf("\r\n d: %4d mm.\r\n",vl53l0x_data.RangeMilliMeter);
		
}
