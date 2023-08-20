#include "iwdg.h"

// 2022.3.1
// h00421956
void IWDG_Init(u8 prer, u16 rlr) {
	// 设置写保护
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	IWDG_SetPrescaler(prer);
	
	// 设置重装载值
	IWDG_SetReload(rlr);
	IWDG_ReloadCounter();
	IWDG_Enable();
}

/************************************************
Date: 2022.3.2
Author: h00421956
Func: feed independent watch dog
************************************************/

void IWDG_Feed(void) {
	IWDG_ReloadCounter(); // reload

}

