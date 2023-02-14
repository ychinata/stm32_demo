#include "sdcard.h"            
static u8  SD_Type=0;  //存放SD卡的类型

//https://developer.aliyun.com/article/838811

/*
函数功能：底层SD卡接口初始化
SPI1接口---SD卡接线原理
5V----5V
GND---GND
SPI1_MOSI---PA7
SPI1_MISO---PA6
SPI1_CS---PA4
SPI1_SCK--PA5
*/
 
/*
函数功能：SD卡底层接口,通过SPI时序向SD卡读写一个字节
函数参数：data是要写入的数据
返 回 值：读到的数据
*/
u8 SDCardReadWriteOneByte(u8 DataTx)
{        
    u16 cnt=0;               
    while((SPI1->SR&1<<1)==0)        //等待发送区空--等待发送缓冲为空 
    {
      cnt++;
      if(cnt>=65530)return 0;     //超时退出  u16=2个字节
    }   
    SPI1->DR=DataTx;                      //发送一个byte 
    cnt=0;
    while((SPI1->SR&1<<0)==0)       //等待接收完一个byte   
    {
      cnt++;
      if(cnt>=65530)return 0;      //超时退出
    }                               
    return SPI1->DR;                //返回收到的数据
}
 
 
/*
函数功能：底层SD卡接口初始化
SPI1接口---SD卡接线原理
5V----5V
GND---GND
SPI1_MOSI---PA7
SPI1_MISO---PA6
SPI1_CS---PA4
SPI1_SCK--PA5
*/
void SDCardSpiInit(void)
{
  /*1. 开启时钟*/
    RCC->APB2ENR|=1<<2;         //使能PORTA时钟
  
  /*2. 配置GPIO口模式*/
  GPIOA->CRL&=0x0000FFFF;
  GPIOA->CRL|=0xB8B30000;
  
  /*3. 上拉*/
  GPIOA->ODR|=1<<4;
    
        /*SPI1基本配置*/
    RCC->APB2ENR|=1<<12;    //开启SPI1时钟
    RCC->APB2RSTR|=1<<12;
    RCC->APB2RSTR&=~(1<<12);
    
    SPI1->CR1=0X0;      //清空寄存器
    SPI1->CR1|=0<<15; //选择“双线双向”模式
    SPI1->CR1|=0<<11; //使用8位数据帧格式进行发送/接收；
    SPI1->CR1|=0<<10; //全双工(发送和接收)；
    SPI1->CR1|=1<<9;  //启用软件从设备管理
    SPI1->CR1|=1<<8;  //NSS
    SPI1->CR1|=0<<7;  //帧格式，先发送高位
    SPI1->CR1|=0x0<<3;//当总线频率为36MHZ时，SPI速度为18MHZ，高速。
    SPI1->CR1|=1<<2;  //配置为主设备
    SPI1->CR1|=1<<1;  //空闲状态时， SCK保持高电平。
    SPI1->CR1|=1<<0;  //数据采样从第二个时钟边沿开始。
    SPI1->CR1|=1<<6;  //开启SPI设备。
}
 
/*
函数功能：取消选择,释放SPI总线
*/
void SDCardCancelCS(void)
{
    SDCARD_CS=1;
    SDCardReadWriteOneByte(0xff);//提供额外的8个时钟
}
 
 
/*
函数 功 能：选择sd卡,并且等待卡准备OK
函数返回值：0,成功;1,失败;
*/
u8 SDCardSelectCS(void)
{
    SDCARD_CS=0;
    if(SDCardWaitBusy()==0)return 0;//等待成功
    SDCardCancelCS();
    return 1;//等待失败
}
 
 
/*
函数 功 能：等待卡准备好
函数返回值：0,准备好了;其他,错误代码
*/
u8 SDCardWaitBusy(void)
{
    u32 t=0;
    do
    {
        if(SDCardReadWriteOneByte(0XFF)==0XFF)return 0;//OK
        t++;          
    }while(t<0xFFFFFF);//等待 
    return 1;
}
 
 
/*
函数功能：等待SD卡回应
函数参数：
                    Response:要得到的回应值
返 回 值：
                    0,成功得到了该回应值
                    其他,得到回应值失败
*/
u8 SDCardGetAck(u8 Response)
{
    u16 Count=0xFFFF;//等待次数                           
    while((SDCardReadWriteOneByte(0XFF)!=Response)&&Count)Count--;//等待得到准确的回应     
    if(Count==0)return SDCard_RESPONSE_FAILURE;//得到回应失败   
    else return SDCard_RESPONSE_NO_ERROR;//正确回应
}
 
 
/*
函数功能：从sd卡读取一个数据包的内容
函数参数：
                buf:数据缓存区
                len:要读取的数据长度.
返回值：
            0,成功;其他,失败; 
*/
u8 SDCardRecvData(u8*buf,u16 len)
{                 
    if(SDCardGetAck(0xFE))return 1;//等待SD卡发回数据起始令牌0xFE
    while(len--)//开始接收数据
    {
        *buf=SDCardReadWriteOneByte(0xFF);
        buf++;
    }
    //下面是2个伪CRC（dummy CRC）
    SDCardReadWriteOneByte(0xFF);
    SDCardReadWriteOneByte(0xFF);                                                           
    return 0;//读取成功
}
 
 
/*
函数功能：向sd卡写入一个数据包的内容 512字节
函数参数：
                    buf 数据缓存区
                    cmd 指令
返 回 值：0表示成功;其他值表示失败;
*/
u8 SDCardSendData(u8*buf,u8 cmd)
{   
    u16 t;            
    if(SDCardWaitBusy())return 1;  //等待准备失效
    SDCardReadWriteOneByte(cmd);
    if(cmd!=0XFD)//不是结束指令
    {
        for(t=0;t<512;t++)SDCardReadWriteOneByte(buf[t]);//提高速度,减少函数传参时间
        SDCardReadWriteOneByte(0xFF); //忽略crc
        SDCardReadWriteOneByte(0xFF);
          t=SDCardReadWriteOneByte(0xFF); //接收响应
        if((t&0x1F)!=0x05)return 2;   //响应错误                                                            
    }                                                                                   
    return 0;//写入成功
}
 
 
 
/*
函数功能：向SD卡发送一个命令
函数参数：
                u8 cmd   命令 
                u32 arg  命令参数
                u8 crc   crc校验值 
返回值:SD卡返回的响应
*/                                                
u8 SendSDCardCmd(u8 cmd, u32 arg, u8 crc)
{
    u8 r1;  
    u8 Retry=0; 
        
    SDCardCancelCS();               //取消上次片选
    if(SDCardSelectCS())return 0XFF;//片选失效 
    //发送数据
    SDCardReadWriteOneByte(cmd | 0x40);//分别写入命令
    SDCardReadWriteOneByte(arg >> 24);
    SDCardReadWriteOneByte(arg >> 16);
    SDCardReadWriteOneByte(arg >> 8);
    SDCardReadWriteOneByte(arg);      
    SDCardReadWriteOneByte(crc); 
    if(cmd==SDCard_CMD12)SDCardReadWriteOneByte(0xff);//Skip a stuff byte when stop reading
    Retry=0X1F;
    do
    {
        r1=SDCardReadWriteOneByte(0xFF);
    }while((r1&0X80) && Retry--);     //等待响应，或超时退出
   return r1;   //返回状态值
}   
 
 
 
/*
函数功能：获取SD卡的CID信息，包括制造商信息
函数参数：u8 *cid_data(存放CID的内存，至少16Byte）      
返 回 值：
                    0：成功，1：错误               
*/
u8 GetSDCardCISDCardOutnfo(u8 *cid_data)
{
    u8 r1;     
    //发SDCard_CMD10命令，读CID
    r1=SendSDCardCmd(SDCard_CMD10,0,0x01);
    if(r1==0x00)
      {
            r1=SDCardRecvData(cid_data,16);//接收16个字节的数据  
    }
    SDCardCancelCS();//取消片选
    if(r1)return 1;
    else return 0;
}   
 
 
/*
函数说明：
                    获取SD卡的CSD信息，包括容量和速度信息
函数参数：
                    u8 *cid_data(存放CID的内存，至少16Byte）     
返 回 值：
                    0：成功，1：错误   
*/
u8 GetSDCardCSSDCardOutnfo(u8 *csd_data)
{
    u8 r1;   
    r1=SendSDCardCmd(SDCard_CMD9,0,0x01);    //发SDCard_CMD9命令，读CSD
    if(r1==0)
    {
        r1=SDCardRecvData(csd_data, 16);//接收16个字节的数据 
    }
    SDCardCancelCS();//取消片选
    if(r1)return 1;
    else return 0;
}  
 
 
/*
函数功能：获取SD卡的总扇区数（扇区数）   
返 回 值：
                0表示容量检测出错，其他值表示SD卡的容量(扇区数/512字节)
说   明：
                每扇区的字节数必为512字节，如果不是512字节，则初始化不能通过.  
*/
u32 GetSDCardSectorCount(void)
{
    u8 csd[16];
    u32 Capacity;  
      u16 csize;                        
    if(GetSDCardCSSDCardOutnfo(csd)!=0) return 0;   //取CSD信息，如果期间出错，返回0
    if((csd[0]&0xC0)==0x40)  //SDHC卡，按照下面方式计算
    {   
            csize = csd[9] + ((u16)csd[8] << 8) + 1;
            Capacity = (u32)csize << 10;//得到扇区数            
    }
    return Capacity;
}
 
 
 
/*
函数功能： 初始化SD卡
返 回 值： 非0表示初始化失败!
*/
u8 SDCardDeviceInit(void)
{
  u8 r1;      // 存放SD卡的返回值
  u16 retry;  // 用来进行超时计数
  u8 buf[4];  
    u16 i;
    SDCardSpiInit();        //初始化底层IO口
    
    for(i=0;i<10;i++)SDCardReadWriteOneByte(0XFF); //发送最少74个脉冲
    retry=20;
    do
    {
        r1=SendSDCardCmd(SDCard_CMD0,0,0x95);//进入IDLE状态 闲置
    }while((r1!=0X01) && retry--);
    SD_Type=0;   //默认无卡
    if(r1==0X01)
    {
        if(SendSDCardCmd(SDCard_CMD8,0x1AA,0x87)==1)  //SD V2.0
        {
            for(i=0;i<4;i++)buf[i]=SDCardReadWriteOneByte(0XFF);
            if(buf[2]==0X01&&buf[3]==0XAA)    //卡是否支持2.7~3.6V
            {
                retry=0XFFFE;
                do
                {
                    SendSDCardCmd(SDCard_CMD55,0,0X01);     //发送SDCard_CMD55
                    r1=SendSDCardCmd(SDCard_CMD41,0x40000000,0X01);//发送SDCard_CMD41
                }while(r1&&retry--);
                if(retry&&SendSDCardCmd(SDCard_CMD58,0,0X01)==0)//鉴别SD2.0卡版本开始
                {
                    for(i=0;i<4;i++)buf[i]=SDCardReadWriteOneByte(0XFF);//得到OCR值
                    if(buf[0]&0x40)SD_Type=SDCard_TYPE_V2HC;    //检查CCS
                    else SD_Type=SDCard_TYPE_V2;   
                }
            }
        }
    }
    SDCardCancelCS();       //取消片选
    if(SD_Type)return 0;  //初始化成功返回0
    else if(r1)return r1; //返回值错误值     
    return 0xaa;          //其他错误
}
 
 
/*
函数功能：读SD卡
函数参数：
                buf:数据缓存区
                sector:扇区
                cnt:扇区数
返回值:
                0,ok;其他,失败.
说  明：
                SD卡一个扇区大小512字节
*/
u8 SDCardReadData(u8*buf,u32 sector,u32 cnt)
{
    u8 r1;
    if(SD_Type!=SDCard_TYPE_V2HC)sector<<=9;//转换为字节地址
    if(cnt==1)
    {
        r1=SendSDCardCmd(SDCard_CMD17,sector,0X01);//读命令
        if(r1==0)                                                 //指令发送成功
        {
            r1=SDCardRecvData(buf,512);         //接收512个字节     
        }
    }else
    {
        r1=SendSDCardCmd(SDCard_CMD18,sector,0X01);//连续读命令
        do
        {
            r1=SDCardRecvData(buf,512);//接收512个字节    
            buf+=512;  
        }while(--cnt && r1==0);     
        SendSDCardCmd(SDCard_CMD12,0,0X01); //发送停止命令
    }   
    SDCardCancelCS();//取消片选
    return r1;//
}
 
/*
函数功能：向SD卡写数据
函数参数：
                buf:数据缓存区
                sector:起始扇区
                cnt:扇区数
返回值：
                0,ok;其他,失败.
说  明：
                SD卡一个扇区大小512字节
*/
u8 SDCardWriteData(u8*buf,u32 sector,u32 cnt)
{
    u8 r1;
    if(SD_Type!=SDCard_TYPE_V2HC)sector *= 512;//转换为字节地址
    if(cnt==1)
    {
        r1=SendSDCardCmd(SDCard_CMD24,sector,0X01);//读命令
        if(r1==0)//指令发送成功
        {
            r1=SDCardSendData(buf,0xFE);//写512个字节      
        }
    }
    else
    {
        if(SD_Type!=SDCard_TYPE_MMC)
        {
            SendSDCardCmd(SDCard_CMD55,0,0X01); 
            SendSDCardCmd(SDCard_CMD23,cnt,0X01);//发送指令 
        }
        r1=SendSDCardCmd(SDCard_CMD25,sector,0X01);//连续读命令
        if(r1==0)
        {
            do
            {
                r1=SDCardSendData(buf,0xFC);//接收512个字节   
                buf+=512;  
            }while(--cnt && r1==0);
            r1=SDCardSendData(0,0xFD);//接收512个字节 
        }
    }   
    SDCardCancelCS();//取消片选
    return r1;//
}

#include "sdcard.h"            
static u8  SD_Type=0;  //存放SD卡的类型
 
/*
函数功能：SD卡底层接口,通过SPI时序向SD卡读写一个字节
函数参数：data是要写入的数据
返 回 值：读到的数据
*/
u8 SDCardReadWriteOneByte(u8 DataTx)
{        
    u16 cnt=0;               
    while((SPI1->SR&1<<1)==0)        //等待发送区空--等待发送缓冲为空 
    {
      cnt++;
      if(cnt>=65530)return 0;     //超时退出  u16=2个字节
    }   
    SPI1->DR=DataTx;                      //发送一个byte 
    cnt=0;
    while((SPI1->SR&1<<0)==0)       //等待接收完一个byte   
    {
      cnt++;
      if(cnt>=65530)return 0;      //超时退出
    }                               
    return SPI1->DR;                //返回收到的数据
}
 
 
/*
函数功能：底层SD卡接口初始化
SPI1接口---SD卡接线原理
5V----5V
GND---GND
SPI1_MOSI---PA7
SPI1_MISO---PA6
SPI1_CS---PA4
SPI1_SCK--PA5
*/
void SDCardSpiInit(void)
{
  /*1. 开启时钟*/
    RCC->APB2ENR|=1<<2;         //使能PORTA时钟
  
  /*2. 配置GPIO口模式*/
  GPIOA->CRL&=0x0000FFFF;
  GPIOA->CRL|=0xB8B30000;
  
  /*3. 上拉*/
  GPIOA->ODR|=1<<4;
    
        /*SPI1基本配置*/
    RCC->APB2ENR|=1<<12;    //开启SPI1时钟
    RCC->APB2RSTR|=1<<12;
    RCC->APB2RSTR&=~(1<<12);
    
    SPI1->CR1=0X0;      //清空寄存器
    SPI1->CR1|=0<<15; //选择“双线双向”模式
    SPI1->CR1|=0<<11; //使用8位数据帧格式进行发送/接收；
    SPI1->CR1|=0<<10; //全双工(发送和接收)；
    SPI1->CR1|=1<<9;  //启用软件从设备管理
    SPI1->CR1|=1<<8;  //NSS
    SPI1->CR1|=0<<7;  //帧格式，先发送高位
    SPI1->CR1|=0x0<<3;//当总线频率为36MHZ时，SPI速度为18MHZ，高速。
    SPI1->CR1|=1<<2;  //配置为主设备
    SPI1->CR1|=1<<1;  //空闲状态时， SCK保持高电平。
    SPI1->CR1|=1<<0;  //数据采样从第二个时钟边沿开始。
    SPI1->CR1|=1<<6;  //开启SPI设备。
}
 
/*
函数功能：取消选择,释放SPI总线
*/
void SDCardCancelCS(void)
{
    SDCARD_CS=1;
    SDCardReadWriteOneByte(0xff);//提供额外的8个时钟
}
 
 
/*
函数 功 能：选择sd卡,并且等待卡准备OK
函数返回值：0,成功;1,失败;
*/
u8 SDCardSelectCS(void)
{
    SDCARD_CS=0;
    if(SDCardWaitBusy()==0)return 0;//等待成功
    SDCardCancelCS();
    return 1;//等待失败
}
 
 
/*
函数 功 能：等待卡准备好
函数返回值：0,准备好了;其他,错误代码
*/
u8 SDCardWaitBusy(void)
{
    u32 t=0;
    do
    {
        if(SDCardReadWriteOneByte(0XFF)==0XFF)return 0;//OK
        t++;          
    }while(t<0xFFFFFF);//等待 
    return 1;
}
 
 
/*
函数功能：等待SD卡回应
函数参数：
                    Response:要得到的回应值
返 回 值：
                    0,成功得到了该回应值
                    其他,得到回应值失败
*/
u8 SDCardGetAck(u8 Response)
{
    u16 Count=0xFFFF;//等待次数                           
    while((SDCardReadWriteOneByte(0XFF)!=Response)&&Count)Count--;//等待得到准确的回应     
    if(Count==0)return SDCard_RESPONSE_FAILURE;//得到回应失败   
    else return SDCard_RESPONSE_NO_ERROR;//正确回应
}
 
 
/*
函数功能：从sd卡读取一个数据包的内容
函数参数：
                buf:数据缓存区
                len:要读取的数据长度.
返回值：
            0,成功;其他,失败; 
*/
u8 SDCardRecvData(u8*buf,u16 len)
{                 
    if(SDCardGetAck(0xFE))return 1;//等待SD卡发回数据起始令牌0xFE
    while(len--)//开始接收数据
    {
        *buf=SDCardReadWriteOneByte(0xFF);
        buf++;
    }
    //下面是2个伪CRC（dummy CRC）
    SDCardReadWriteOneByte(0xFF);
    SDCardReadWriteOneByte(0xFF);                                                           
    return 0;//读取成功
}
 
 
/*
函数功能：向sd卡写入一个数据包的内容 512字节
函数参数：
                    buf 数据缓存区
                    cmd 指令
返 回 值：0表示成功;其他值表示失败;
*/
u8 SDCardSendData(u8*buf,u8 cmd)
{   
    u16 t;            
    if(SDCardWaitBusy())return 1;  //等待准备失效
    SDCardReadWriteOneByte(cmd);
    if(cmd!=0XFD)//不是结束指令
    {
        for(t=0;t<512;t++)SDCardReadWriteOneByte(buf[t]);//提高速度,减少函数传参时间
        SDCardReadWriteOneByte(0xFF); //忽略crc
        SDCardReadWriteOneByte(0xFF);
          t=SDCardReadWriteOneByte(0xFF); //接收响应
        if((t&0x1F)!=0x05)return 2;   //响应错误                                                            
    }                                                                                   
    return 0;//写入成功
}
 
 
 
/*
函数功能：向SD卡发送一个命令
函数参数：
                u8 cmd   命令 
                u32 arg  命令参数
                u8 crc   crc校验值 
返回值:SD卡返回的响应
*/                                                
u8 SendSDCardCmd(u8 cmd, u32 arg, u8 crc)
{
    u8 r1;  
    u8 Retry=0; 
        
    SDCardCancelCS();               //取消上次片选
    if(SDCardSelectCS())return 0XFF;//片选失效 
    //发送数据
    SDCardReadWriteOneByte(cmd | 0x40);//分别写入命令
    SDCardReadWriteOneByte(arg >> 24);
    SDCardReadWriteOneByte(arg >> 16);
    SDCardReadWriteOneByte(arg >> 8);
    SDCardReadWriteOneByte(arg);      
    SDCardReadWriteOneByte(crc); 
    if(cmd==SDCard_CMD12)SDCardReadWriteOneByte(0xff);//Skip a stuff byte when stop reading
    Retry=0X1F;
    do
    {
        r1=SDCardReadWriteOneByte(0xFF);
    }while((r1&0X80) && Retry--);     //等待响应，或超时退出
   return r1;   //返回状态值
}   
 
 
 
/*
函数功能：获取SD卡的CID信息，包括制造商信息
函数参数：u8 *cid_data(存放CID的内存，至少16Byte）      
返 回 值：
                    0：成功，1：错误               
*/
u8 GetSDCardCISDCardOutnfo(u8 *cid_data)
{
    u8 r1;     
    //发SDCard_CMD10命令，读CID
    r1=SendSDCardCmd(SDCard_CMD10,0,0x01);
    if(r1==0x00)
      {
            r1=SDCardRecvData(cid_data,16);//接收16个字节的数据  
    }
    SDCardCancelCS();//取消片选
    if(r1)return 1;
    else return 0;
}   
 
 
/*
函数说明：
                    获取SD卡的CSD信息，包括容量和速度信息
函数参数：
                    u8 *cid_data(存放CID的内存，至少16Byte）     
返 回 值：
                    0：成功，1：错误   
*/
u8 GetSDCardCSSDCardOutnfo(u8 *csd_data)
{
    u8 r1;   
    r1=SendSDCardCmd(SDCard_CMD9,0,0x01);    //发SDCard_CMD9命令，读CSD
    if(r1==0)
    {
        r1=SDCardRecvData(csd_data, 16);//接收16个字节的数据 
    }
    SDCardCancelCS();//取消片选
    if(r1)return 1;
    else return 0;
}  
 
 
/*
函数功能：获取SD卡的总扇区数（扇区数）   
返 回 值：
                0表示容量检测出错，其他值表示SD卡的容量(扇区数/512字节)
说   明：
                每扇区的字节数必为512字节，如果不是512字节，则初始化不能通过.  
*/
u32 GetSDCardSectorCount(void)
{
    u8 csd[16];
    u32 Capacity;  
      u16 csize;                        
    if(GetSDCardCSSDCardOutnfo(csd)!=0) return 0;   //取CSD信息，如果期间出错，返回0
    if((csd[0]&0xC0)==0x40)  //SDHC卡，按照下面方式计算
    {   
            csize = csd[9] + ((u16)csd[8] << 8) + 1;
            Capacity = (u32)csize << 10;//得到扇区数            
    }
    return Capacity;
}
 
 
 
/*
函数功能： 初始化SD卡
返 回 值： 非0表示初始化失败!
*/
u8 SDCardDeviceInit(void)
{
  u8 r1;      // 存放SD卡的返回值
  u16 retry;  // 用来进行超时计数
  u8 buf[4];  
    u16 i;
    SDCardSpiInit();        //初始化底层IO口
    
    for(i=0;i<10;i++)SDCardReadWriteOneByte(0XFF); //发送最少74个脉冲
    retry=20;
    do
    {
        r1=SendSDCardCmd(SDCard_CMD0,0,0x95);//进入IDLE状态 闲置
    }while((r1!=0X01) && retry--);
    SD_Type=0;   //默认无卡
    if(r1==0X01)
    {
        if(SendSDCardCmd(SDCard_CMD8,0x1AA,0x87)==1)  //SD V2.0
        {
            for(i=0;i<4;i++)buf[i]=SDCardReadWriteOneByte(0XFF);
            if(buf[2]==0X01&&buf[3]==0XAA)    //卡是否支持2.7~3.6V
            {
                retry=0XFFFE;
                do
                {
                    SendSDCardCmd(SDCard_CMD55,0,0X01);     //发送SDCard_CMD55
                    r1=SendSDCardCmd(SDCard_CMD41,0x40000000,0X01);//发送SDCard_CMD41
                }while(r1&&retry--);
                if(retry&&SendSDCardCmd(SDCard_CMD58,0,0X01)==0)//鉴别SD2.0卡版本开始
                {
                    for(i=0;i<4;i++)buf[i]=SDCardReadWriteOneByte(0XFF);//得到OCR值
                    if(buf[0]&0x40)SD_Type=SDCard_TYPE_V2HC;    //检查CCS
                    else SD_Type=SDCard_TYPE_V2;   
                }
            }
        }
    }
    SDCardCancelCS();       //取消片选
    if(SD_Type)return 0;  //初始化成功返回0
    else if(r1)return r1; //返回值错误值     
    return 0xaa;          //其他错误
}
 
 
/*
函数功能：读SD卡
函数参数：
                buf:数据缓存区
                sector:扇区
                cnt:扇区数
返回值:
                0,ok;其他,失败.
说  明：
                SD卡一个扇区大小512字节
*/
u8 SDCardReadData(u8*buf,u32 sector,u32 cnt)
{
    u8 r1;
    if(SD_Type!=SDCard_TYPE_V2HC)sector<<=9;//转换为字节地址
    if(cnt==1)
    {
        r1=SendSDCardCmd(SDCard_CMD17,sector,0X01);//读命令
        if(r1==0)                                                 //指令发送成功
        {
            r1=SDCardRecvData(buf,512);         //接收512个字节     
        }
    }else
    {
        r1=SendSDCardCmd(SDCard_CMD18,sector,0X01);//连续读命令
        do
        {
            r1=SDCardRecvData(buf,512);//接收512个字节    
            buf+=512;  
        }while(--cnt && r1==0);     
        SendSDCardCmd(SDCard_CMD12,0,0X01); //发送停止命令
    }   
    SDCardCancelCS();//取消片选
    return r1;//
}
 
/*
函数功能：向SD卡写数据
函数参数：
                buf:数据缓存区
                sector:起始扇区
                cnt:扇区数
返回值：
                0,ok;其他,失败.
说  明：
                SD卡一个扇区大小512字节
*/
u8 SDCardWriteData(u8*buf,u32 sector,u32 cnt)
{
    u8 r1;
    if(SD_Type!=SDCard_TYPE_V2HC)sector *= 512;//转换为字节地址
    if(cnt==1)
    {
        r1=SendSDCardCmd(SDCard_CMD24,sector,0X01);//读命令
        if(r1==0)//指令发送成功
        {
            r1=SDCardSendData(buf,0xFE);//写512个字节      
        }
    }
    else
    {
        if(SD_Type!=SDCard_TYPE_MMC)
        {
            SendSDCardCmd(SDCard_CMD55,0,0X01); 
            SendSDCardCmd(SDCard_CMD23,cnt,0X01);//发送指令 
        }
        r1=SendSDCardCmd(SDCard_CMD25,sector,0X01);//连续读命令
        if(r1==0)
        {
            do
            {
                r1=SDCardSendData(buf,0xFC);//接收512个字节   
                buf+=512;  
            }while(--cnt && r1==0);
            r1=SDCardSendData(0,0xFD);//接收512个字节 
        }
    }   
    SDCardCancelCS();//取消片选
    return r1;//
}   
