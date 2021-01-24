/****************************************************************************************/
/*																						*/
/* 	@Author 					: 				PIXELS Team Projects Committee			*/
/*	@Date						: 				9 OCT 2020								*/
/*	@Version					: 				V01										*/
/*	@File						:				AFIO_program.c							*/
/*																						*/
/****************************************************************************************/

/****************************************************************************************/
/*																						*/
/*																						*/	
/*									Not Compeleted										*/	
/*																						*/
/*																						*/
/****************************************************************************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "AFIO_private.h"
#include "AFIO_interface.h"
#include "AFIO_config.h"


void MAFIO_voidSetEXTIConfig(u8 Copy_u8Line , u8 Copy_u8PortMap)
{
	
	u8 local_u8Index = (Copy_u8Line/4);
	Copy_u8Line %= 4;
	AFIO->EXTICR[local_u8Index] &= ~(0b1111<<(Copy_u8Line*4));
	AFIO->EXTICR[local_u8Index] |= (Copy_u8PortMap<<(Copy_u8Line*4));
	
	/*
	AFIO->EXTICR[(Copy_u8Line/4)] &= ~(0b1111<<((Copy_u8Line%4)*4));
	AFIO->EXTICR[(Copy_u8Line/4)] &= ~(0b1111<<((Copy_u8Line%4)*4));
	*/
	
	
}

