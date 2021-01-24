/****************************************************************************************/
/*																						*/
/* 	@Author 					: 				PIXELS Team Projects Committee			*/
/*	@Date						: 				30 DEC 2020								*/
/*	@Version					: 				V02										*/
/*	@File						:				TIM1_program.c							*/
/*																						*/
/****************************************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "TIM1_interface.h"
#include "TIM1_private.h"
#include "TIM1_config.h"


static volatile u8 TIM1_u8TimeIntervalMode = 5;
void(*TIM1_CallBack)(void);

void TIM1_voidInit(void)
{
	//Reset Registers
	TIM1->CR1  = 0;
	TIM1->DIER = 0;
	TIM1->SR   = 0;
	
	//Set Bit4 ARPE to activate Auto-reload preload enable, TIMx_ARR register is buffered
	SET_BIT(TIM1-> CR1 , 7);
	//Set Bit4 DIR to use Counter as downcounter
	SET_BIT(TIM1-> CR1 , 4);
	//Set Bit2 URS to generate an update interrupt or DMA request Only  at counter overflow/underflow Mode
	SET_BIT(TIM1-> CR1 , 2);
	//CLR Bit0 UIE to disable Update interrupt
	CLR_BIT(TIM1->DIER , 0);
}
void TIM1_voidSetBusyWait(u16 TIM1_u16Ticks , u32 TIM1_u32ClkFreq)
{
	//CK_CNT = fCK_PSC/(TIM1->PSC+1)
	//Counter Clk = ClkFreq/(Prescaler+1)
	//Set Prescaler
	TIM1->PSC = ( (SYSTEM_CLK/TIM1_u32ClkFreq) - 1 );
	//Auto reload value
	TIM1->ARR = TIM1_u16Ticks;
	//Set Bit0 UG to update Counter & Prescaler
	SET_BIT(TIM1->EGR , 0);
	//Set Bit0 CEN to enable counter
	SET_BIT(TIM1-> CR1, 0);
	//wait till interrupt flag
	while( GET_BIT(TIM1->SR , 0) == 0 );
	//CLR Bit0 CEN to disable counter
	CLR_BIT(TIM1-> CR1, 0);
	//Clear Interrupt Flag
	CLR_BIT(TIM1->SR , 0) ;
}


void TIM1_voidSetSingleInterval(u16 TIM1_u16Ticks , u32 TIM1_u32ClkFreq , void(*PtrToFunc)(void) )
{
	//CLR Bit0 CEN to disable counter
	CLR_BIT(TIM1-> CR1, 0);
	//CLR Bit0 UIE to disable Update interrupt
	CLR_BIT(TIM1->DIER , 0);
	TIM1_u8TimeIntervalMode = TIM1_SINGLE_INTERVAL;
	//Save Callback Function
	TIM1_CallBack = PtrToFunc;
	//CK_CNT = fCK_PSC/(TIM1->PSC+1)
	//Counter Clk = ClkFreq/(Prescaler+1)
	//Set Prescaler
	TIM1->PSC = ( (SYSTEM_CLK/TIM1_u32ClkFreq) - 1 );
	//Auto reload value
	TIM1->ARR = TIM1_u16Ticks;
	//Set Bit0 UG to update Counter & Prescaler
	SET_BIT(TIM1->EGR , 0);
	//Set Bit0 UIE to Enable Update interrupt
	SET_BIT(TIM1->DIER , 0);
	//Set Bit0 CEN to enable counter
	SET_BIT(TIM1-> CR1, 0);
}
void TIM1_voidSetPeriodicInterval(u16 TIM1_u16Ticks , u32 TIM1_u32ClkFreq , void(*PtrToFunc)(void) )
{
	TIM1_u8TimeIntervalMode = TIM1_PERIODIC_INTERVAL;
	//Save Callback Function
	TIM1_CallBack = PtrToFunc;
	//Counter Clk = ClkFreq/(Prescaler+1)
	TIM1->PSC = ( (SYSTEM_CLK/TIM1_u32ClkFreq) - 1 );
	//Auto reload value
	TIM1->ARR = TIM1_u16Ticks;
	//Set Bit0 UG to update Counter & Prescaler
	SET_BIT(TIM1->EGR , 0);
	//Set Bit0 CEN to enable counter
	SET_BIT(TIM1-> CR1, 0);
	//Set Bit0 UIE to Enable Update interrupt
	SET_BIT(TIM1->DIER , 0);
}
void TIM1_UP_IRQHandler(void)
{
	if(TIM1_u8TimeIntervalMode == TIM1_SINGLE_INTERVAL)
	{
		//CLR Bit0 UIE to Disable Update interrupt
		CLR_BIT(TIM1->DIER , 0);
		//CLR Bit0 CEN to disable counter
		CLR_BIT(TIM1-> CR1, 0);
	}
	TIM1_CallBack();
	//Clear UnderFlow Interrupt Flag
	CLR_BIT(TIM1->SR , 0) ;
}

