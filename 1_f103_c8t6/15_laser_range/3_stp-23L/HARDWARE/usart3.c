/***********************************************
��˾����ݸ��΢�����ܿƼ����޹�˾
Ʒ�ƣ�WHEELTEC
������wheeltec.net
�Ա����̣�shop114407458.taobao.com 
����ͨ: https://minibalance.aliexpress.com/store/4455017
�汾��5.7
�޸�ʱ�䣺2021-04-29

Company: WeiHong Co.Ltd
Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version:5.7
Update��2021-04-29

All rights reserved
***********************************************/
#include "usart3.h"
#include <string.h>
/**************************************************************************
Function: Usart3 initialization
Input   : bound:Baud rate
Output  : none
�������ܣ�����3��ʼ��
��ڲ�����bound:������
����  ֵ����
**************************************************************************/

LidarPointTypedef Pack_Data[12];/* �״���յ����ݴ������������֮�� */
LidarPointTypedef Pack_sum;     /* ���������� */
extern u16 receive_cnt;
extern u8 confidence;
extern u16 distance,noise,reftof;
extern u32 peak,intg;

void uart3_init(u32 bound)
{  	 
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��UGPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART3ʱ��
	//USART3_TX  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//USART3_RX	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Usart3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure);     //��ʼ������3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3 

}

/**************************************************************************
Function: Receive interrupt function
Input   : none
Output  : none
�������ܣ�����3�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
void USART3_IRQHandler(void)//����ld14�״�����
{
    static u8 state = 0;			//״̬λ	
    static u8 crc = 0;				//У���
    static u8 cnt = 0;				//����һ֡12����ļ���
    static u8 PACK_FLAG = 0;  //�����־λ
    static u8 data_len  = 0;  //���ݳ���
    static u32 timestamp = 0; //ʱ���
    static u8 state_flag = 1; //ת�����ݽ��ձ�־λ
    u8 temp_data;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {      /* ���յ����� */
        temp_data=USART_ReceiveData(USART3); 									 
        USART_ClearITPendingBit(USART3,USART_IT_RXNE); 				 /* ����жϱ�־λ */
        if(state< 4) {																					 /* ��ʼ����֤ ǰ4�����ݾ�Ϊ0xAA */
            if(temp_data == HEADER) state ++;
            else state = 0;
        } else if(state<10&&state>3) {
            switch(state) {
                case 4:   
                    if(temp_data == device_address) {              /* �豸��ַ��֤  */                    							
                        state ++;
                        crc = crc + temp_data;									
                        break;        
                    } else 
                        state = 0,crc = 0;
                case 5:   
                    if(temp_data == PACK_GET_DISTANCE) {					 /* ��ȡ������������ */                    
                        PACK_FLAG = PACK_GET_DISTANCE;
                        state ++;	
                        crc = crc + temp_data;	
                        break;									
                    } else if(temp_data == PACK_RESET_SYSTEM) { 		 /* ��λ���� */                    
                        PACK_FLAG = PACK_RESET_SYSTEM;
                        state ++; 
                        crc = crc + temp_data;	
                        break;	
                    } else if(temp_data == PACK_STOP) {							 /* ֹͣ�������ݴ������� */                    
                        PACK_FLAG = PACK_STOP;
                        state ++; 
                        crc = crc + temp_data;	
                        break;
                    } else if(temp_data == PACK_ACK) {							 /* Ӧ�������� */
                        PACK_FLAG = PACK_ACK;
                        state ++;
                        crc = crc + temp_data;	
                        break;
                    } else if(temp_data == PACK_VERSION) {					 /* ��ȡ��������Ϣ���� */                    
                        PACK_FLAG = PACK_VERSION,
                        state ++,
                        crc = crc + temp_data;	   	     
                        break;
                    } else 
                        state = 0,crc = 0;
                case 6: 
                    if(temp_data == chunk_offset) {         /* ƫ�Ƶ�ַ */                    
                        state ++;
                        crc = crc + temp_data;
                        break; 	  
                    }	
                    else state = 0,crc = 0;
                case 7: 
                    if(temp_data == chunk_offset) {
                        state ++;
                        crc = crc + temp_data;
                        break;
                    }
                    else state = 0,crc = 0;
                case 8: 
                    data_len = (u16)temp_data;								 /* ���ݳ��ȵͰ�λ */
                    state ++; 
                    crc = crc + temp_data;
                    break;																			 
                case 9: 
                    data_len = data_len + ((u16)temp_data<<8); 			 /* ���ݳ��ȸ߰�λ */
                    state ++;
                    crc = crc + temp_data;
                    break; 
                default: break;
            }
        } else if(state == 10 ) 
            state_flag = 0;                    /*��switch������ʱstateΪ10����temp_data��Ϊ���볤�ȸ߰�λ���ݣ�������һ���ж�*/
        
        if(PACK_FLAG == PACK_GET_DISTANCE&&state_flag == 0) {     /* ��ȡһ֡���ݲ�У�� */        
            if(state>9) {            
                if(state<190) {                    
                    static u8 state_num;
                    state_num = (state-10)%15;
                    switch(state_num) {                    
                        case 0: 
                            Pack_Data[cnt].distance = (u16)temp_data ;				 /* �������ݵͰ�λ */
                            crc = crc + temp_data;
                            state++;
                            break;        
                        case 1: 
                            Pack_Data[cnt].distance = ((u16)temp_data<<8) + Pack_Data[cnt].distance;	 /* �������� */
                            crc = crc + temp_data;
                            state++;
                            break; 
                        case 2:
                            Pack_Data[cnt].noise = (u16)temp_data;				 /* ���������Ͱ�λ */
                            crc = crc + temp_data;
                            state++;
                            break; 
                        case 3:
                            Pack_Data[cnt].noise = ((u16)temp_data<<8) + Pack_Data[cnt].noise;				 /* �������� */
                            crc = crc + temp_data;
                            state++;
                            break; 
                        case 4:
                            Pack_Data[cnt].peak = (u32)temp_data;				 										 /* ����ǿ����Ϣ�Ͱ�λ */
                            crc = crc + temp_data;
                            state++;
                            break; 
                        case 5:
                            Pack_Data[cnt].peak = ((u32)temp_data<<8) + Pack_Data[cnt].peak;
                            crc = crc + temp_data;
                            state++;
                            break; 
                        case 6:
                            Pack_Data[cnt].peak = ((u32)temp_data<<16) + Pack_Data[cnt].peak;	
                            crc = crc + temp_data;
                            state++;
                            break; 
                        case 7:
                            Pack_Data[cnt].peak = ((u32)temp_data<<24) + Pack_Data[cnt].peak;				    /* ����ǿ����Ϣ */
                            crc = crc + temp_data;
                            state++;
                            break; 
                        case 8:
                            Pack_Data[cnt].confidence = temp_data;				 /* ���Ŷ� */
                            crc = crc + temp_data;
                            state++;
                            break; 
                        case 9:
                            Pack_Data[cnt].intg = (u32)temp_data;															/* ���ִ����Ͱ�λ */
                            crc = crc + temp_data;
                            state++;
                            break; 
                        case 10:
                            Pack_Data[cnt].intg = ((u32)temp_data<<8) + Pack_Data[cnt].intg;
                            crc = crc + temp_data;
                            state++;
                            break; 
                        case 11:
                            Pack_Data[cnt].intg = ((u32)temp_data<<16) + Pack_Data[cnt].intg;
                            crc = crc + temp_data;
                            state++;
                            break; 
                        case 12:
                            Pack_Data[cnt].intg = ((u32)temp_data<<24) + Pack_Data[cnt].intg;				  	 /* ���ִ��� */
                            crc = crc + temp_data;
                            state++;
                            break; 
                        case 13:
                            Pack_Data[cnt].reftof = (int16_t)temp_data;				 								 /* �¶ȱ���ֵ�Ͱ�λ */
                            crc = crc + temp_data;
                            state++;
                            break; 
                        case 14:
                            Pack_Data[cnt].reftof = ((int16_t)temp_data<<8) +Pack_Data[cnt].reftof;			/* �¶ȱ���ֵ */
                            crc = crc + temp_data;
                            state++;
                            cnt++;							 /* ������һ�������� */
                            break; 
                        default: break;
                    }
                }
                /* ʱ��� */
                if(state == 190) timestamp = temp_data,state++,crc = crc + temp_data;
                else if(state == 191) timestamp = ((u32)temp_data<<8) + timestamp,state++,crc = crc + temp_data; 
                else if(state == 192) timestamp = ((u32)temp_data<<16) + timestamp,state++,crc = crc + temp_data;
                else if(state == 193) timestamp = ((u32)temp_data<<24) + timestamp,state++,crc = crc + temp_data; 
                else if(state==194) {
                    if(temp_data == crc)   /* У��ɹ� */
                    {
                            data_process();  	 /* ���ݴ����������һ֮֡��ɽ������ݴ��� */
                            receive_cnt++;	 	 /* ������յ���ȷ���ݵĴ��� */
                    }
                    distance = Pack_Data[0].distance;
                    crc = 0;
                    state = 0;
                    state_flag = 1;
                    cnt = 0; 							 /* ��λ*/
                }               
            } //state>9
        } ///* ��ȡһ֡���ݲ�У�� */   
    } ///* ���յ����� */
}
	


void data_process(void)/*���ݴ����������һ֮֡��ɽ������ݴ���*/
{
    /* ������� */
    //static u8 cnt = 0;
    u8 i;
    static u16 count = 0;
    //static u32 sum = 0;
    LidarPointTypedef Pack_sum;
    for(i=0;i<12;i++)									/* 12����ȡƽ�� */
    {
        if(Pack_Data[i].distance != 0)  /* ȥ��0�ĵ� */
        {
            count++;
            Pack_sum.distance += Pack_Data[i].distance;
            Pack_sum.noise += Pack_Data[i].noise;
            Pack_sum.peak += Pack_Data[i].peak;
            Pack_sum.confidence += Pack_Data[i].confidence;
            Pack_sum.intg += Pack_Data[i].intg;
            Pack_sum.reftof += Pack_Data[i].reftof;
        }
    }
    if(count !=0)
    {
        distance = Pack_sum.distance/count;
        noise = Pack_sum.noise/count;
        peak = Pack_sum.peak/count;
        confidence = Pack_sum.confidence/count;
        intg = Pack_sum.intg/count;
        reftof = Pack_sum.reftof/count;
        Pack_sum.distance = 0;
        Pack_sum.noise = 0;
        Pack_sum.peak = 0;
        Pack_sum.confidence = 0;
        Pack_sum.intg = 0;
        Pack_sum.reftof = 0;
        count = 0;
    }
}




