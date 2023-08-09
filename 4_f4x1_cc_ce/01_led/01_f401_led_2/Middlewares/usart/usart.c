


#include "usart.h"	

/********************************************************************************

//���빦�ܣ����� USART 1 ����

//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

*******************************************************************************/
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;


//����_sys_exit()�Ա���ʹ�ð�����ģʽ    

void _sys_exit(int x) 
{ 
	x = x; 
} 


//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 



#if EN_USART1_RX   //���ʹ���˽���


//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���  

u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

u8 *RXD1_P;           //����1 ��������ָ��

u16 RXD1_Num=0;       //�������ݸ���  ����

u8  RXD1_STA=0;          //��������״̬��0, ��������δ��ɣ�  ����0���������ݽ���


/********************************************************************************

//�������ƣ�void USART1_init(u32 bound)

//���빦�ܣ�
//��ʼ��IO ����1 
//bound:������

//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

*******************************************************************************/

void USART1_init(u32 bound)
	{
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RXD1_P=USART_RX_BUF;//ָ��ָ�򴮿ڽ������ݵĻ�����	
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}



/********************************************************************************

//�������ƣ�void USART1_IRQHandler(void)  

//���빦�ܣ�����1  �жϷ������

//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

*******************************************************************************/


void USART1_IRQHandler(void)                	//����1�жϷ������
{
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  // �����ж� ( ���յ������ݱ�����  0x0D,0x0A ( �س����� )��β )
	{
		
		*RXD1_P =USART_ReceiveData(USART1);      // (USART1->DR);	     //��ȡ���յ�������
		
		 RXD1_P++;//ָ����һ��������
		
		 RXD1_Num++;//�����ۼ�
		 		
		 if(RXD1_Num>USART_REC_LEN)         //����洢�����˴��ڽ������ݻ����� , �ӻ������׵�ַ��ʼ�洢
			{
			   RXD1_P=USART_RX_BUF;           //ָ��ָ�򴮿ڽ������ݵĻ�����	
				 RXD1_Num=0;
				
			}
			if( RXD1_Num>1)     //�������ݴ���2����ʱ��
				{
					if((USART_RX_BUF[RXD1_Num-2]==0x0d) & (USART_RX_BUF[RXD1_Num-1]==0x0a))
					{

						RXD1_STA=0x01; //���ý������ݽ�����ǣ��س����н���
						
					}
				}
 		 
   } 
	

	
} 


#endif	

 







/********************************************************************************

//���빦�ܣ����� USART 1 ����

//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

*******************************************************************************/

