/****************************************************************************************/
/*																						*/
/* 	@Author 					: 				PIXELS Team Projects Committee			*/
/*	@Date						: 				30 DEC 2020								*/
/*	@Version					: 				V02										*/
/*	@File						:				TIM1_private.h							*/
/*																						*/
/****************************************************************************************/
#ifndef _TIM1_PRIVATE_H_
#define _TIM1_PRIVATE_H_

typedef struct{
	volatile u32 CR1	;
	volatile u32 CR2	;
	volatile u32 SMCR	;
	volatile u32 DIER	;
	volatile u32 SR		;
	volatile u32 EGR    ;
	volatile u32 CCMR1  ;
    volatile u32 CCMR2  ;
    volatile u32 CCER   ;
    volatile u32 CNT    ;
    volatile u32 PSC    ;
    volatile u32 ARR    ;
	volatile u32 RCR    ;
    volatile u32 CCR1   ;
    volatile u32 CCR2   ;
    volatile u32 CCR3   ;
    volatile u32 CCR4   ;
    volatile u32 BDTR   ;
	volatile u32 DCR    ;
	volatile u32 DMAR   ;
}TIM1_REG;

#define 		TIM1_BASE_ADDRESS				0x40012C00
#define 		TIM1							((volatile TIM1_REG*)(TIM1_BASE_ADDRESS))
#define 		TIM1_SINGLE_INTERVAL			0
#define 		TIM1_PERIODIC_INTERVAL			1
#endif
