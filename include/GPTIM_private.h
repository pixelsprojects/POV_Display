/****************************************************************************************/
/*																						*/
/* 	@Author 					: 				PIXELS Team Projects Committee			*/
/*	@Date						: 				13 JAN 2021								*/
/*	@Version					: 				V01										*/
/*	@File						:				GPTIM_private.h							*/
/*																						*/
/****************************************************************************************/
#ifndef _GPTIM_PRIVATE_H_
#define _GPTIM_PRIVATE_H_

typedef struct{
	volatile u32 CR1		;
	volatile u32 CR2		;
	volatile u32 SMCR		;
	volatile u32 DIER		;
	volatile u32 SR			;
	volatile u32 EGR    	;
	volatile u32 CCMR1  	;
    volatile u32 CCMR2  	;
    volatile u32 CCER   	;
    volatile u32 CNT    	;
    volatile u32 PSC    	;
    volatile u32 ARR    	;
	volatile u32 Reserved1	;		//Reserved
    volatile u32 CCR1   	;
    volatile u32 CCR2   	;
    volatile u32 CCR3   	;
    volatile u32 CCR4   	;
    volatile u32 Reserved2 	;		//Reserved
	volatile u32 DCR    	;
	volatile u32 DMAR   	;
}GPTIM2_REG;

#define 		GPTIM2_BASE_ADDRESS				0x40000000
#define 		GPTIM2							((volatile GPTIM2_REG*)(GPTIM2_BASE_ADDRESS))
#define 		GPTIM2_SINGLE_INTERVAL			0
#define 		GPTIM2_PERIODIC_INTERVAL		1
#endif
