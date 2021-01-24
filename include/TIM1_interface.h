/****************************************************************************************/
/*																						*/
/* 	@Author 					: 				PIXELS Team Projects Committee			*/
/*	@Date						: 				30 DEC 2020								*/
/*	@Version					: 				V02										*/
/*	@File						:				TIM1_interface.h						*/
/*																						*/
/****************************************************************************************/
#ifndef _TIM1_INTERFACE_H_
#define _TIM1_INTERFACE_H_

void TIM1_voidInit(void);
void TIM1_voidSetBusyWait(u16 TIM1_u16Ticks , u32 TIM1_u32ClkFreq);
void TIM1_voidSetSingleInterval(u16 TIM1_u16Ticks , u32 TIM1_u32ClkFreq , void(*PtrToFunc)(void) );
void TIM1_voidSetPeriodicInterval(u16 TIM1_u16Ticks , u32 TIM1_u32ClkFreq , void(*PtrToFunc)(void) );
#endif
