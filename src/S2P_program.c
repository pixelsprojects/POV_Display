/****************************************************************************************/
/*																						*/
/* 	@Author 					: 				PIXELS Team Projects Committee			*/
/*	@Date						: 				3 JAN 2021								*/
/*	@Version					: 				V02										*/
/*	@File						:				S2P_program.c							*/
/*																						*/
/****************************************************************************************/


#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "GPIO_interface.h"
#include "STK_interface.h"
#include "TIM1_interface.h"

#include "S2P_interface.h"
#include "S2P_private.h"
#include "S2P_config.h"

void HS2P_voidSend8BitSynchronous(u8 Copy_u8DataToSend)
{
	s8 Local_s8Counter=0;
	for(Local_s8Counter=7; Local_s8Counter>=0; Local_s8Counter--)
	{
		//Send bit by bit starting by MSB
		gPIO_voidSetPinValue(HS2P_SERIAL_DATA, GET_BIT(Copy_u8DataToSend , Local_s8Counter));
		
		//Send Pulse 
		gPIO_voidSetPinValue(HS2P_SHIFT_CLK, HIGH);
		//MSTK_voidSetBusyWait(1);
		TIM1_voidSetBusyWait(1 , 1000000);
		gPIO_voidSetPinValue(HS2P_SHIFT_CLK, LOW);
		//MSTK_voidSetBusyWait(1);
		TIM1_voidSetBusyWait(1 , 1000000);
		
	}
	//Send pulse to Storage CLK
	gPIO_voidSetPinValue(HS2P_STORAGE_CLK, HIGH);
	//MSTK_voidSetBusyWait(1);
	TIM1_voidSetBusyWait(1 , 1000000);
	gPIO_voidSetPinValue(HS2P_STORAGE_CLK, LOW);
	//MSTK_voidSetBusyWait(1);
	TIM1_voidSetBusyWait(1 , 1000000);
}

void HS2P_voidSend16BitSynchronous(u16 Copy_u16DataToSend)
{
	s8 Local_s8Counter=0;
	for(Local_s8Counter=15; Local_s8Counter>=0; Local_s8Counter--)
	{
		//Send bit by bit starting by MSB
		gPIO_voidSetPinValue(HS2P_SERIAL_DATA, GET_BIT(Copy_u16DataToSend , Local_s8Counter));

		//Send Pulse
		gPIO_voidSetPinValue(HS2P_SHIFT_CLK, HIGH);
		//MSTK_voidSetBusyWait(1);
		TIM1_voidSetBusyWait(1 , 1000000);
		gPIO_voidSetPinValue(HS2P_SHIFT_CLK, LOW);
		//MSTK_voidSetBusyWait(1);
		TIM1_voidSetBusyWait(1 , 1000000);

	}
	//Send pulse to Storage CLK
	gPIO_voidSetPinValue(HS2P_STORAGE_CLK, HIGH);
	//MSTK_voidSetBusyWait(1);
	TIM1_voidSetBusyWait(1 , 1000000);
	gPIO_voidSetPinValue(HS2P_STORAGE_CLK, LOW);
	//MSTK_voidSetBusyWait(1);
	TIM1_voidSetBusyWait(1 , 1000000);
}
