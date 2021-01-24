/****************************************************************************************/
/*																						*/
/* 	@Author 					: 				PIXELS Team Projects Committee			*/
/*	@Date						: 				13 JAN 2021								*/
/*	@Version					: 				V01										*/
/*	@File						:				GPTIM_program.c							*/
/*																						*/
/****************************************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "GPTIM_interface.h"
#include "GPTIM_private.h"
#include "GPTIM_config.h"


static volatile u8 GPTIM2_u8TimeIntervalMode = 5;
void(*GPTIM2_CallBack)(void);

void GPTIM2_voidInit(void)
{
	//Reset Registers
	GPTIM2->CR1  = 0;
	GPTIM2->DIER = 0;
	GPTIM2->SR   = 0;

	//Set Bit7 ARPE to activate Auto-reload preload enable, TIMx_ARR register is buffered
	SET_BIT(GPTIM2-> CR1 , 7);
	//Set Bit4 DIR to use Counter as downcounter
	SET_BIT(GPTIM2-> CR1 , 4);
	//Set Bit2 URS to generate an update interrupt or DMA request Only  at counter overflow/underflow Mode
	SET_BIT(GPTIM2-> CR1 , 2);
	//CLR Bit0 UIE to disable Update interrupt
	CLR_BIT(GPTIM2->DIER , 0);
}
void GPTIM2_voidSetBusyWait(u16 GPTIM2_u16Ticks , u32 GPTIM2_u32ClkFreq)
{
	//CK_CNT = fCK_PSC/(TIM1->PSC+1)
	//Counter Clk = ClkFreq/(Prescaler+1)
	//Set Prescaler
	GPTIM2->PSC = ( (SYSTEM_CLK/GPTIM2_u32ClkFreq) - 1 );
	//Auto reload value
	GPTIM2->ARR = GPTIM2_u16Ticks;
	//Set Bit0 UG to update Counter & Prescaler
	SET_BIT(GPTIM2->EGR , 0);
	//Set Bit0 CEN to enable counter
	SET_BIT(GPTIM2-> CR1, 0);
	//wait till interrupt flag
	while( GET_BIT(GPTIM2->SR , 0) == 0 );
	//CLR Bit0 CEN to disable counter
	CLR_BIT(GPTIM2-> CR1, 0);
	//Clear Interrupt Flag
	CLR_BIT(GPTIM2->SR , 0) ;
}


void GPTIM2_voidSetSingleInterval(u16 GPTIM2_u16Ticks , u32 GPTIM2_u32ClkFreq , void(*PtrToFunc)(void) )
{
	GPTIM2_u8TimeIntervalMode = GPTIM2_SINGLE_INTERVAL;
	//Save Callback Function
	GPTIM2_CallBack = PtrToFunc;
	//CK_CNT = fCK_PSC/(TIM1->PSC+1)
	//Counter Clk = ClkFreq/(Prescaler+1)
	//Set Prescaler
	GPTIM2->PSC = ( (SYSTEM_CLK/GPTIM2_u32ClkFreq) - 1 );
	//Auto reload value
	GPTIM2->ARR = GPTIM2_u16Ticks;
	//Set Bit0 UG to update Counter & Prescaler
	SET_BIT(GPTIM2->EGR , 0);
	//Set Bit0 UIE to Enable Update interrupt
	SET_BIT(GPTIM2->DIER , 0);
	//Set Bit0 CEN to enable counter
	SET_BIT(GPTIM2-> CR1, 0);
}
void GPTIM2_voidSetPeriodicInterval(u16 GPTIM2_u16Ticks , u32 GPTIM2_u32ClkFreq , void(*PtrToFunc)(void) )
{
	GPTIM2_u8TimeIntervalMode = GPTIM2_PERIODIC_INTERVAL;
	//Save Callback Function
	GPTIM2_CallBack = PtrToFunc;
	//Counter Clk = ClkFreq/(Prescaler+1)
	GPTIM2->PSC = ( (SYSTEM_CLK/GPTIM2_u32ClkFreq) - 1 );
	//Auto reload value
	GPTIM2->ARR = GPTIM2_u16Ticks;
	//Set Bit0 UG to update Counter & Prescaler
	SET_BIT(GPTIM2->EGR , 0);
	//Set Bit0 CEN to enable counter
	SET_BIT(GPTIM2-> CR1, 0);
	//Set Bit0 UIE to Enable Update interrupt
	SET_BIT(GPTIM2->DIER , 0);
}

u16 GPTIM2_u16GetElapsedTime(void)
{
	u16 local_u16Reslut=0;
	local_u16Reslut = ( (GPTIM2->ARR) - (GPTIM2->CNT) );
	return local_u16Reslut;
}
u16 GPTIM2_u16GetRemainingTime(void)
{
	return GPTIM2->CNT;
}

void TIM2_IRQHandler(void)
{
	if(GPTIM2_u8TimeIntervalMode == GPTIM2_SINGLE_INTERVAL)
	{
		//CLR Bit0 UIE to Disable Update interrupt
		CLR_BIT(GPTIM2->DIER , 0);
		//CLR Bit0 CEN to disable counter
		CLR_BIT(GPTIM2-> CR1, 0);
	}
	GPTIM2_CallBack();
	//Clear UnderFlow Interrupt Flag
	CLR_BIT(GPTIM2->SR , 0) ;
}

