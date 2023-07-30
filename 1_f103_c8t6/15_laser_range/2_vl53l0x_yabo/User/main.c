

#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include "bsp_led.h"
#include "VL53L0X.h"
#include "IIC_A.h"


#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"


#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_i2c_platform.h"

VL53L0X_Error vl53l0x_init(void);

VL53L0X_Dev_t vl53l0x_dev;
VL53L0X_DeviceInfo_t vl53l0x_dev_info;
VL53L0X_RangingMeasurementData_t vl53l0x_data;
VL53L0X_Error vl53l0x_status;

/***************************************************************************************/

void print_pal_error(VL53L0X_Error Status)
{
    char buf[VL53L0X_MAX_STRING_LENGTH];
    VL53L0X_GetPalErrorString(Status, buf);
    printf("API Status: %i : %s\n", Status, buf);
}

void print_range_status(VL53L0X_RangingMeasurementData_t* pdata)
{
    char buf[VL53L0X_MAX_STRING_LENGTH];
    uint8_t RangeStatus;
    // New Range Status: data is valid when pdata->RangeStatus = 0
    RangeStatus = pdata->RangeStatus;

    VL53L0X_GetRangeStatusString(RangeStatus, buf);
    //printf("Range Status: %i : %s\n", RangeStatus, buf);
}

VL53L0X_Error vl53l0x_start_single_test(VL53L0X_Dev_t *pdev, \
                            VL53L0X_RangingMeasurementData_t *pdata)
{
    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    
    if(vl53l0x_status != VL53L0X_ERROR_NONE)
        return vl53l0x_status;

    status = VL53L0X_PerformSingleRangingMeasurement(pdev, pdata);
    if(status != VL53L0X_ERROR_NONE){
        printf("error:Call of VL53L0X_PerformSingleRangingMeasurement\n");
        return status;
    }
    print_range_status(pdata);
    printf("Measured distance: %i\n\n", pdata->RangeMilliMeter-25);
    return status;
}


VL53L0X_Error vl53l0x_measure_init(VL53L0X_Dev_t *pMyDevice)
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    //FixPoint1616_t LimitCheckCurrent;
    uint32_t refSpadCount;
    uint8_t isApertureSpads;
    uint8_t VhvSettings;
    uint8_t PhaseCal;
    // Device Initialization
    Status = VL53L0X_StaticInit(pMyDevice); 
    if(Status != VL53L0X_ERROR_NONE){
        printf ("Call of VL53L0X_StaticInit\n");
        print_pal_error(Status);
        return Status;
    }
    // Device Initialization
    Status = VL53L0X_PerformRefCalibration(pMyDevice, &VhvSettings, &PhaseCal); 
    if(Status != VL53L0X_ERROR_NONE){
        printf ("Call of VL53L0X_PerformRefCalibration\n");
        print_pal_error(Status);
        return Status;
    }

    // needed if a coverglass is used and no calibration has been performed
    Status = VL53L0X_PerformRefSpadManagement(pMyDevice,&refSpadCount, &isApertureSpads);
    if(Status != VL53L0X_ERROR_NONE){
        printf ("Call of VL53L0X_PerformRefSpadManagement\n");
        printf ("refSpadCount = %d, isApertureSpads = %d\n", refSpadCount, isApertureSpads);
        print_pal_error(Status);
        return Status;
    }

    // no need to do this when we use VL53L0X_PerformSingleRangingMeasurement
    Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_SINGLE_RANGING); // Setup in single ranging mode
    if(Status != VL53L0X_ERROR_NONE){
        printf ("Call of VL53L0X_SetDeviceMode\n");
        print_pal_error(Status);
        return Status;
    }

    // Enable/Disable Sigma and Signal check
    if(Status == VL53L0X_ERROR_NONE){
        Status = VL53L0X_SetLimitCheckEnable(pMyDevice,
        		VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
    }
    if(Status == VL53L0X_ERROR_NONE){
        Status = VL53L0X_SetLimitCheckEnable(pMyDevice,
        		VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
    }
				
    if(Status == VL53L0X_ERROR_NONE){
        Status = VL53L0X_SetLimitCheckValue(pMyDevice,
        		VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,
        		(FixPoint1616_t)(0.25*65536));
	}			
    if(Status == VL53L0X_ERROR_NONE){
        Status = VL53L0X_SetLimitCheckValue(pMyDevice,
        		VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,
        		(FixPoint1616_t)(18*65536));			
    }
    if(Status == VL53L0X_ERROR_NONE){
        Status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice,
        		200000);
    }
    if(Status != VL53L0X_ERROR_NONE){
        printf ("Sigma and Signal check error\n");
        print_pal_error(Status);
        return Status;
    }
    return Status;
}

VL53L0X_Error vl53l0x_init(void)
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    VL53L0X_Dev_t *pMyDevice = &vl53l0x_dev;

    pMyDevice->I2cDevAddr      = 0x52;
    pMyDevice->comms_type      =  1;
    pMyDevice->comms_speed_khz =  400;
    
    VL53L0X_i2c_init();

    Status = VL53L0X_DataInit(pMyDevice); // Data initialization
    if(Status != VL53L0X_ERROR_NONE){
        print_pal_error(Status);
        return Status;
    }

    Status = VL53L0X_GetDeviceInfo(pMyDevice, &vl53l0x_dev_info);
    if(Status != VL53L0X_ERROR_NONE){
        print_pal_error(Status);
        return Status;
    }
    printf("VL53L0X_GetDeviceInfo:\n");
    printf("Device Name : %s\n", vl53l0x_dev_info.Name);
    printf("Device Type : %s\n", vl53l0x_dev_info.Type);
    printf("Device ID : %s\n", vl53l0x_dev_info.ProductId);
    printf("ProductRevisionMajor : %d\n", vl53l0x_dev_info.ProductRevisionMajor);
    printf("ProductRevisionMinor : %d\n", vl53l0x_dev_info.ProductRevisionMinor);

    if ((vl53l0x_dev_info.ProductRevisionMajor != 1) && (vl53l0x_dev_info.ProductRevisionMinor != 1)){
        printf("Error expected cut 1.1 but found cut %d.%d\n",
        vl53l0x_dev_info.ProductRevisionMajor, vl53l0x_dev_info.ProductRevisionMinor);
        Status = VL53L0X_ERROR_NOT_SUPPORTED;
        print_pal_error(Status);
        return Status;
    }

    Status = vl53l0x_measure_init(pMyDevice);
    vl53l0x_status = Status;
    if(Status != VL53L0X_ERROR_NONE){
        print_pal_error(Status);
        return Status;
    }

    return Status;
}

/*
 * t : 定时时间 
 * Ticks : 多少个时钟周期产生一次中断 
 * f : 时钟频率 72000000
 * t = Ticks * 1/f = (72000000/100000) * (1/72000000) = 10us 
 */ 
 
 /**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{
	
    /* 端口初始化 */
    GPIO_Config();   
    //LED1:红灯控制；LED2:绿灯控制；LED3:蓝灯控制

    /* 配置SysTick 为1us中断一次 */
    SysTick_Init();
    /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
    USART_Config();
	
	vl53l0x_init();
	
	  //si4463_mainloop();
    while(1)
    {
        if(vl53l0x_status == VL53L0X_ERROR_NONE)
        {
            vl53l0x_start_single_test(&vl53l0x_dev, &vl53l0x_data);
        }
    }
	

}


