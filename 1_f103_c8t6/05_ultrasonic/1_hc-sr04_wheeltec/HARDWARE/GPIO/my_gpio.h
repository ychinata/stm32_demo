#ifndef _MY_GPIO_H_
#define _MY_GPIO_H_
//#include <sys.h>

typedef enum {
    ENUM_GPIO_A = 0x00,
    ENUM_GPIO_B = 0x01,
    ENUM_GPIO_C = 0x02,
} GPIOn_Enum;    //GPIO组枚举类型

typedef enum {
    ENUM_Pin_0 = 0x00,
    ENUM_Pin_1 = 0x01,
    ENUM_Pin_2 = 0x02,
    ENUM_Pin_3 = 0x03,
    ENUM_Pin_4 = 0x04,
    ENUM_Pin_5 = 0x05,
    ENUM_Pin_6 = 0x06,
    ENUM_Pin_7 = 0x07,
    ENUM_Pin_8 = 0x08,
    ENUM_Pin_9 = 0x09,
    ENUM_Pin_10 = 0x0A,
    ENUM_Pin_11 = 0x0B,
    ENUM_Pin_12 = 0x0C,
    ENUM_Pin_13 = 0x0D,
    ENUM_Pin_14 = 0x0E,
    ENUM_Pin_15 = 0x0F,  
} Pinx_Enum;    //IO口枚举类型

typedef enum {
    OUT_PP   =  0x00,     //默认推挽输出
    AF_PP    =  0x03,     //复用推挽输出
    OUT_OD   =  0x02,     //开漏输出
    IU       =  0x01,   //上拉输入
    ID       =  0x04, //下拉输入
    IF       =  0x00,  //浮空输入
    AIN      =  0x1E,  //模拟输入
}GPIO_MODE_enum;  //定义IO口模式


//void gpio_init(GPIOn_Enum GPIOn,Pinx_Enum Pinx,GPIO_MODE_enum mode);
//void gpio_set(GPIOn_Enum GPIOn,Pinx_Enum Pinx,uint8_t mode);
//uint8_t gpio_get(GPIOn_Enum GPIOn,Pinx_Enum Pinx);


#endif
