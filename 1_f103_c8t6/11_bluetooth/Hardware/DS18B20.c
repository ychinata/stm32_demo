#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OneWire.h"

//DS18B20指令
#define DS18B20_SKIP_ROM		0xCC
#define DS18B20_CONVERT_T		0x44
#define DS18B20_READ_SCRATCHPAD	0xBE

//温度数值保存
float T;
char Temperature[10];


/**
  * @brief  DS18B20开始温度变换
  * @param  无
  * @retval 无
  */
void DS18B20_ConvertT(void)
{
	OneWire_Init();
	OneWire_SendByte(DS18B20_SKIP_ROM);
	OneWire_SendByte(DS18B20_CONVERT_T);
}


/**
  * @brief  DS18B20读取温度
  * @param  无
  * @retval T 温度数值
  */
float DS18B20_ReadTemp(void)
{
	uint8_t TLSB = 0,TMSB = 0;
	uint16_t Temp = 0;
	DS18B20_ConvertT();
	Delay_ms(750);	//750ms
	OneWire_Init();
	OneWire_SendByte(DS18B20_SKIP_ROM);
	OneWire_SendByte(DS18B20_READ_SCRATCHPAD);
	TLSB = OneWire_ReceiveByte();
	TMSB = OneWire_ReceiveByte();
	Temp = (TMSB << 8) | TLSB;
	T = (float)Temp/16.0;
	
	return T;
}

/**
  * @brief  把温度转换成字符串
  * @param  无
  * @retval Temperature 存储温度的字符形式
  */
char* Temperature_String(void)
{
	uint32_t i,j;
	DS18B20_ReadTemp();
	if (T < 0)
	{
		Temperature[0] = '-';
		T = -T;
	}
	else
	{
		Temperature[0] = '+';
	}
	T = T*100;
	for (i = 1, j = 1000; i < 6; i ++, j /= 10)
	{
		if (i == 3) {Temperature[i] = '.'; j *= 10;}
		else {Temperature[i] = ((int)T/j%10)+'0';}	
	}
	return Temperature;
};

	
