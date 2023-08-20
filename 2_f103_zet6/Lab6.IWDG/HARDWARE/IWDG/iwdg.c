#include "iwdg.h"

// 2022.3.1
// h00421956
void IWDG_Init(u8 prer, u16 rlr) {
	// ����д����
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	IWDG_SetPrescaler(prer);
	
	// ������װ��ֵ
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

