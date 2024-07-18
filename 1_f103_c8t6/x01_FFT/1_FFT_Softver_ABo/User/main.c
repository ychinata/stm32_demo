#include "stm32f10x.h"                 
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "Serial.h"

#include "fft.h"
#include "stdio.h"
#include "math.h"

/* USER CODE BEGIN PV */
#define NPT 256
struct compx s[NPT];
float fft_data[NPT];
long data_in[NPT]={0,84,129,118,60,-9,-51,-44,5,64,95,75,7,-77,-138,-145,-96,-16,55,83,59,3,-48,-59,-21,50,116,140,106,28,-59,-114,-114,-65,1,45,43,-4,-66,-106,-95,-33,54,125,146,109,35,-37,
	-70,-53,0,53,70,37,-35,-109,-146,-125,-54,33,95,106,66,4,-43,-45,-1,65,114,114,59,-28,-106,-140,-116,-50,21,59,48,-3,-59,-83,-55,16,96,145,138,77,-7,-75,-95,-64,-5,44,51,9,-60,-118,-129,
	-84,0,84,129,118,60,-9,-51,-44,5,64,95,75,7,-77,-138,-145,-96,-16,55,83,59,3,-48,-59,-21,50,116,140,106,28,-59,-114,-114,-65,1,45,43,-4,-66,-106,-95,-33,54,125,146,109,35,-37,-70,-53,0,
	53,70,37,-35,-109,-146,-125,-54,33,95,106,66,4,-43,-45,-1,65,114,114,59,-28,-106,-140,-116,-50,21,59,48,-3,-59,-83,-55,16,96,145,138,77,-7,-75,-95,-64,-5,44,51,9,-60,-118,-129,-84,0,84,
	129,118,60,-9,-51,-44,5,64,95,75,7,-77,-138,-145,-96,-16,55,83,59,3,-48,-59,-21,50,116,140,106,28,-59,-114,-114,-65,1,45,43,-4,-66,-106,-95,-33,54,125,146,109,35,-37,-70,-53,0,53,70,37,-35,-109};
long data_out[NPT/2];//输出信号
long Mag_data[NPT/2];//计算各次谐波幅值
/* USER CODE END PV */

/*
 * 配置项：Serial.h中的模式： SERIAL_NOT_PACKET_MODE
 * OLED引脚
 * USB TO TTL串口模块引脚：
    TXD <-> PA10(USART_1_RX)
    RXD <-> PA9 (USART_1_TX)
    GND <-> GND       
 */

/************************************************
Date: 2022.8.23
Author: h00421956
Func:串口发送字节 
************************************************/
int main1(void) {
    float fnum = 12.5556;
	// init
    LED_Init();
	OLED_Init();
	Serial_Init();
    
    //OLED_ShowString(1, 1, "Serial Recev:");
    // 4.OLED格式化显示  
    
    printf("串口发送功能测试\r\n");
    printf("data=%f\r\n", fnum);
    
	// 0.串口发送功能测试
	Serial_TxOnly_Test();
	
			    
	while (1) {              	

        
	}
}

int main(void) {
    int i=0,j=0, k=0;
    
    while (1) {
        printf("begin\r\n");
        for (j=0;j<NPT;j++) {
            s[j].real=data_in[j];
            s[j].imag=0;
        }
        FFT(s,NPT);
        for(k=0;k<NPT;k++) {
            fft_data[k]=sqrt(s[k].real*s[k].real+s[k].imag*s[k].imag);
        }
        for(i=0;i<NPT;i++) {
            printf("%f\r\n",fft_data[i]/128);
        };
        Delay_ms(1000);
    }
}
