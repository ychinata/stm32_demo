#include "vs1053b.h"    
//https://developer.aliyun.com/article/838811
/*
硬件与STM32的接线说明：
OLED显示屏：
D0----SCK-----PB14
D1----MOSI----PB13
RES—复位(低电平有效)—PB12
DC---数据和命令控制管脚—PB1
CS---片选引脚-----PA7
VS1053：
#define VS1053_DREQ     PAin(11)      //DREQ  数据请求
#define VS1053_RESET    PAout(12)   //RST   硬件复位--低电平有效
#define VS1053_XCS      PAout(13)      //XCS   片选--低电平有效
#define VS1053_XDCS     PAout(14)      //XDCS  用于数据片选、字节同步
#define VS1053_SCLK     PAout(15)
#define VS1053_OUTPUT   PBout(3)
#define VS1053_INPUT    PBin(4)
SD卡接口：
5V----5V
GND---GND
SPI1_MOSI---PA7
SPI1_MISO---PA6
SPI1_CS---PA4
SPI1_SCK--PA5
*/

/*
函数功能：移植接口--SPI时序读写一个字节
函数参数：data:要写入的数据
返 回 值：读到的数据
*/
u8 VS1053_SPI_ReadWriteByte(u8 tx_data)
{                
    u8 rx_data=0;                
  u8 i;
  for(i=0;i<8;i++)
    {
        VS1053_SCLK=0;  
        if(tx_data&0x80){VS1053_OUTPUT=1;}
        else {VS1053_OUTPUT=0;}
        tx_data<<=1;    
        VS1053_SCLK=1;
        rx_data<<=1;
        if(VS1053_INPUT)rx_data|=0x01;
    }
    return rx_data; 
}
 
 
/*
函数功能：初始化VS1053的IO口   
*/
void VS1053_Init(void)
{
     RCC->APB2ENR|=1<<0;
     AFIO->MAPR&=~(0x7<<24);  //释放PA13/14/15
     AFIO->MAPR|=0x4<<24;
    
     RCC->APB2ENR|=1<<2;
     RCC->APB2ENR|=1<<3;
     
     GPIOA->CRH&=0x00000FFF;
     GPIOA->CRH|=0x33338000;
      
     GPIOB->CRL&=0xFFF00FFF;
     GPIOB->CRL|=0x00083000;
    
     VS1053_SCLK=1;
     VS1053_XCS=1;
     VS1053_RESET=1;
 
}   
 
 
/*
函数功能：软复位VS10XX
*/
void VS1053_SoftReset(void)
{    
    u8 retry=0;                    
    while(VS1053_DREQ==0);                          //等待软件复位结束     
    VS1053_SPI_ReadWriteByte(0Xff);         //启动传输
    retry=0;
    while(VS1053_ReadReg(SPI_MODE)!=0x0800) // 软件复位,新模式  
    {
        VS1053_WriteCmd(SPI_MODE,0x0804);       // 软件复位,新模式     
        DelayMs(2);//等待至少1.35ms 
        if(retry++>100)break;     
    }   
    while(VS1053_DREQ==0);//等待软件复位结束     
    retry=0;
    while(VS1053_ReadReg(SPI_CLOCKF)!=0X9800)//设置VS10XX的时钟,3倍频 ,1.5xADD 
    {
        VS1053_WriteCmd(SPI_CLOCKF,0X9800); //设置VS10XX的时钟,3倍频 ,1.5xADD
        if(retry++>100)break;       
    }    
    DelayMs(20);
} 
 
 
/*
函数 功 能：硬复位MP3
函数返回值：1:复位失败;0:复位成功 
*/
u8 VS1053_Reset(void)
{
    u8 retry=0;
    VS1053_RESET=0;
    DelayMs(20);
    VS1053_XDCS=1;//取消数据传输
    VS1053_XCS=1; //取消数据传输
    VS1053_RESET=1;    
    while(VS1053_DREQ==0&&retry<200)//等待DREQ为高
    {
        retry++;
        DelayUs(50);
    }
    DelayMs(20);    
    if(retry>=200)return 1;
    else return 0;               
}
 
 
/*
函数功能：向VS10XX写命令
函数参数：
                address:命令地址
                data   :命令数据
*/
void VS1053_WriteCmd(u8 address,u16 data)
{  
    while(VS1053_DREQ==0);  //等待空闲             
    VS1053_XDCS=1;   
    VS1053_XCS=0;    
    VS1053_SPI_ReadWriteByte(VS_WRITE_COMMAND);//发送VS10XX的写命令
    VS1053_SPI_ReadWriteByte(address);  //地址
    VS1053_SPI_ReadWriteByte(data>>8);  //发送高八位
    VS1053_SPI_ReadWriteByte(data);         //第八位
    VS1053_XCS=1;            
} 
 
 
/*
函数参数：向VS1053写数据
函数参数：data:要写入的数据
*/
void VS1053_WriteData(u8 data)
{
    VS1053_XDCS=0;   
    VS1053_SPI_ReadWriteByte(data);
    VS1053_XDCS=1;      
}
 
 
/*
函数功能：读VS1053的寄存器 
函数参数：address：寄存器地址
返回值：读到的值
*/
u16 VS1053_ReadReg(u8 address)
{ 
    u16 temp=0;     
  while(VS1053_DREQ==0);//非等待空闲状态       
    VS1053_XDCS=1;       
    VS1053_XCS=0;        
    VS1053_SPI_ReadWriteByte(VS_READ_COMMAND);//发送VS10XX的读命令
    VS1053_SPI_ReadWriteByte(address);          //地址
    temp=VS1053_SPI_ReadWriteByte(0xff);          //读取高字节
    temp=temp<<8;
    temp+=VS1053_SPI_ReadWriteByte(0xff);       //读取低字节
    VS1053_XCS=1;      
   return temp; 
}  
 
 
/*
函数功能：读取VS1053的RAM
函数参数：addr：RAM地址
返 回 值：读到的值
*/
u16 VS1053_ReadRAM(u16 addr) 
{ 
    u16 res;                  
    VS1053_WriteCmd(SPI_WRAMADDR, addr); 
    res=VS1053_ReadReg(SPI_WRAM);  
    return res;
} 
 
 
/*
函数功能：写VS1053的RAM
函数参数：
                addr：RAM地址
                val:要写入的值 
*/
void VS1053_WriteRAM(u16 addr,u16 val) 
{             
    VS1053_WriteCmd(SPI_WRAMADDR,addr); //写RAM地址 
    while(VS1053_DREQ==0);                          //等待空闲     
    VS1053_WriteCmd(SPI_WRAM,val);          //写RAM值 
} 
 
 
/*
函数参数：发送一次音频数据，固定为32字节
返 回 值：0,发送成功
                  1,本次数据未成功发送   
*/ 
u8 VS1053_SendMusicData(u8* buf)
{
    u8 n;
    if(VS1053_DREQ!=0)  //送数据给VS10XX
    {                
        VS1053_XDCS=0;  
    for(n=0;n<32;n++)
        {
            VS1053_SPI_ReadWriteByte(buf[n]);               
        }
        VS1053_XDCS=1;                     
    }else return 1;
    return 0;//成功发送了
}
 
 
/*
函数参数：发送一次音频数据，固定为32字节
返 回 值：0,发送成功
                  1,本次数据未成功发送   
*/ 
void VS1053_SendMusicByte(u8 data)
{
    u8 n;
    while(VS1053_DREQ==0){}      
    VS1053_XDCS=0;  
    VS1053_SPI_ReadWriteByte(data);             
    VS1053_XDCS=1;                     
}
 
 
/*
函数功能：设定VS1053播放的音量
函数参数：volx:音量大小(0~254)
*/
void VS1053_SetVol(u8 volx)
{
    u16 volt=0;                   //暂存音量值
    volt=254-volx;              //取反一下,得到最大值,表示最大的表示 
      volt<<=8;
    volt+=254-volx;                 //得到音量设置后大小
    VS1053_WriteCmd(SPI_VOL,volt);//设音量 
}
 
 
/*--------------------------------------录音功能-----------------------------------------------------*/
 
 
/*
函数功能：vs10xx装载patch
函数参数：
                patch：patch首地址
                len  ：patch长度
*/
void VS1053_LoadPatch(u16 *patch,u16 len) 
{
    u16 i; 
    u16 addr, n, val;                  
    for(i=0;i<len;) 
    { 
        addr = patch[i++]; 
        n    = patch[i++]; 
        if(n & 0x8000U) //RLE run, replicate n samples 
        { 
            n  &= 0x7FFF; 
            val = patch[i++]; 
            while(n--)VS1053_WriteCmd(addr, val);  
        }
        else //copy run, copy n sample 
        { 
            while(n--) 
            { 
                val = patch[i++]; 
                VS1053_WriteCmd(addr, val); 
            } 
        } 
    }   
}
 
 
/*
函数参数：初始化WAV头
*/
void VS1053_RecoderWavInit(__WaveHeader* wavhead) //初始化WAV头            
{
    wavhead->riff.ChunkID=0X46464952;   //"RIFF"
    wavhead->riff.ChunkSize=0;              //还未确定,最后需要计算
    wavhead->riff.Format=0X45564157;    //"WAVE"
    wavhead->fmt.ChunkID=0X20746D66;    //"fmt "
    wavhead->fmt.ChunkSize=16;              //大小为16个字节
    wavhead->fmt.AudioFormat=0X01;      //0X01,表示PCM;0X01,表示IMA ADPCM
    wavhead->fmt.NumOfChannels=1;           //单声道
    wavhead->fmt.SampleRate=8000;           //8Khz采样率 采样速率
    wavhead->fmt.ByteRate=wavhead->fmt.SampleRate*2;//16位,即2个字节
    wavhead->fmt.BlockAlign=2;              //块大小,2个字节为一个块
    wavhead->fmt.BitsPerSample=16;      //16位PCM
  wavhead->data.ChunkID=0X61746164; //"data"
    wavhead->data.ChunkSize=0;              //数据大小,还需要计算  
}
 
//VS1053的WAV录音有bug,这个plugin可以修正这个问题                                 
const u16 VS1053_WavPlugin[40]=/* Compressed plugin */ 
{ 
        0x0007, 0x0001, 0x8010, 0x0006, 0x001c, 0x3e12, 0xb817, 0x3e14, /* 0 */ 
        0xf812, 0x3e01, 0xb811, 0x0007, 0x9717, 0x0020, 0xffd2, 0x0030, /* 8 */ 
        0x11d1, 0x3111, 0x8024, 0x3704, 0xc024, 0x3b81, 0x8024, 0x3101, /* 10 */ 
        0x8024, 0x3b81, 0x8024, 0x3f04, 0xc024, 0x2808, 0x4800, 0x36f1, /* 18 */ 
        0x9811, 0x0007, 0x0001, 0x8028, 0x0006, 0x0002, 0x2a00, 0x040e,  
}; 
 
 
/*
函数功能：激活PCM 录音模式
函数参数：
                agc:0,自动增益
        1024相当于1倍
        512相当于0.5倍
        最大值65535=64倍          
*/
void VS1053_RecoderInit(u16 agc)
{
    //如果是IMA ADPCM,采样率计算公式如下:
    //采样率=CLKI/256*d;   
    //假设d=0,并2倍频,外部晶振为12.288M.那么Fc=(2*12288000)/256*6=16Khz
    //如果是线性PCM,采样率直接就写采样值 
  VS1053_WriteCmd(SPI_BASS,0x0000);    
    VS1053_WriteCmd(SPI_AICTRL0,8000);  //设置采样率,设置为8Khz
    VS1053_WriteCmd(SPI_AICTRL1,agc);       //设置增益,0,自动增益.1024相当于1倍,512相当于0.5倍,最大值65535=64倍 
    VS1053_WriteCmd(SPI_AICTRL2,0);       //设置增益最大值,0,代表最大值65536=64X
    VS1053_WriteCmd(SPI_AICTRL3,6);       //左通道(MIC单声道输入)
    VS1053_WriteCmd(SPI_CLOCKF,0X2000); //设置VS10XX的时钟,MULT:2倍频;ADD:不允许;CLK:12.288Mhz
    VS1053_WriteCmd(SPI_MODE,0x1804);       //MIC,录音激活    
    DelayMs(5);                 //等待至少1.35ms 
    VS1053_LoadPatch((u16*)VS1053_WavPlugin,40);//VS1053的WAV录音需要patch
}
 
