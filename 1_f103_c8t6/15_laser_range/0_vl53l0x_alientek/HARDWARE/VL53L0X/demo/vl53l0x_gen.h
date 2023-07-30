#ifndef __VL53L0X_GEN_H
#define __VL53L0X_GEN_H

#include "vl53l0x.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK MiniV3 STM32������
//VL53L0X-��ͨ����ģʽ ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/7/1
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  

extern VL53L0X_RangingMeasurementData_t vl53l0x_data;
extern vu16 Distance_data;//����������

VL53L0X_Error vl53l0x_set_mode(VL53L0X_Dev_t *dev,u8 mode);
VL53L0X_Error vl53l0x_start_single_test(VL53L0X_Dev_t *dev,VL53L0X_RangingMeasurementData_t *pdata,char *buf);
void vl53l0x_general_start(VL53L0X_Dev_t *dev,u8 mode);
void vl53l0x_general_test(VL53L0X_Dev_t *dev);

#endif
