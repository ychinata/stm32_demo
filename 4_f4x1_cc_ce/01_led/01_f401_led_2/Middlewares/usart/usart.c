


#include "usart.h"	

/********************************************************************************

//代码功能：串口 USART 1 驱动

//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

*******************************************************************************/
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;


//定义_sys_exit()以避免使用半主机模式    

void _sys_exit(int x) 
{ 
	x = x; 
} 


//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 



#if EN_USART1_RX   //如果使能了接收


//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误  

u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

u8 *RXD1_P;           //串口1 接收数据指针

u16 RXD1_Num=0;       //接收数据个数  计数

u8  RXD1_STA=0;          //接收数据状态：0, 接收数据未完成；  大于0，接收数据结束


/********************************************************************************

//函数名称：void USART1_init(u32 bound)

//代码功能：
//初始化IO 串口1 
//bound:波特率

//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

*******************************************************************************/

void USART1_init(u32 bound)
	{
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RXD1_P=USART_RX_BUF;//指针指向串口接收数据的缓冲区	
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}



/********************************************************************************

//函数名称：void USART1_IRQHandler(void)  

//代码功能：串口1  中断服务程序

//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

*******************************************************************************/


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  // 接收中断 ( 接收到的数据必须是  0x0D,0x0A ( 回车换行 )结尾 )
	{
		
		*RXD1_P =USART_ReceiveData(USART1);      // (USART1->DR);	     //读取接收到的数据
		
		 RXD1_P++;//指向下一个缓冲区
		
		 RXD1_Num++;//计数累加
		 		
		 if(RXD1_Num>USART_REC_LEN)         //如果存储超出了串口接收数据缓冲区 , 从缓冲区首地址开始存储
			{
			   RXD1_P=USART_RX_BUF;           //指针指向串口接收数据的缓冲区	
				 RXD1_Num=0;
				
			}
			if( RXD1_Num>1)     //接收数据大于2个的时候
				{
					if((USART_RX_BUF[RXD1_Num-2]==0x0d) & (USART_RX_BUF[RXD1_Num-1]==0x0a))
					{

						RXD1_STA=0x01; //设置接收数据结束标记：回车换行结束
						
					}
				}
 		 
   } 
	

	
} 


#endif	

 







/********************************************************************************

//代码功能：串口 USART 1 驱动

//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

*******************************************************************************/

