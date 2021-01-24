/****************************************************************************************/
/*																						*/
/* 	@Author 					: 				PIXELS Team Projects Committee			*/
/*	@Date						: 				13 JAN 2021								*/
/*	@Version					: 				V01										*/
/*	@File						:				GPTIM_interface,h						*/
/*																						*/
/****************************************************************************************/
#ifndef _GPTIM_INTERFACE_H_
#define _GPTIM_INTERFACE_H_

void GPTIM2_voidInit(void);
void GPTIM2_voidSetBusyWait(u16 GPTIM2_u16Ticks , u32 GPTIM2_u32ClkFreq);
void GPTIM2_voidSetSingleInterval(u16 GPTIM2_u16Ticks , u32 GPTIM2_u32ClkFreq , void(*PtrToFunc)(void) );
void GPTIM2_voidSetPeriodicInterval(u16 GPTIM2_u16Ticks , u32 GPTIM2_u32ClkFreq , void(*PtrToFunc)(void) );
u16  GPTIM2_u16GetElapsedTime(void);
u16  GPTIM2_u16GetRemainingTime(void);

#endif
