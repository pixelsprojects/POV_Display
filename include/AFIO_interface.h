/****************************************************************************************/
/*																						*/
/* 	@Author 					: 				PIXELS Team Projects Committee			*/
/*	@Date						: 				9 OCT 2020								*/
/*	@Version					: 				V01										*/
/*	@File						:				AFIO_interface.h						*/
/*																						*/
/****************************************************************************************/
#ifndef _AFIO_INTERFACE_H_
#define _AFIO_INTERFACE_H_

#define PA	0
#define PB	1
#define PC	2

void MAFIO_voidSetEXTIConfig(u8 Copy_u8Line , u8 Copy_u8PortMap);
/*
 * Copy_u8PortMap could be: PA, PB or PC
 */

/*
 * Copy_u8Line = 0 : 15
 */
#endif
