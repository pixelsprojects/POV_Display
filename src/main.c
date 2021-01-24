/****************************************************************************************/
/*																						*/
/* 	@Author 					: 				PIXELS Team Projects Committee			*/
/*	@Date						: 				28 DEC 2020								*/
/*	@Version					: 				V03										*/
/*	@Description				:				Display that shows Clock & Words		*/
/*	@File						:				main.c									*/
/*																						*/
/****************************************************************************************/

/***********************************************************************************
 * 	                                                                               *
 *                                                                                 *
 * 									Header Files                                   *
 *                                                                                 *
 *                                                                                 *
 ***********************************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "NVIC_interface.h"
#include "STK_interface.h"
#include "AFIO_interface.h"
#include "EXTI_interface.h"
#include "USART_interface.h"
#include "TIM1_interface.h"
#include "GPTIM_interface.h"
#include "S2P_interface.h"


/***********************************************************************************
 * 	                                                                               *
 *                                                                                 *
 * 									Structures	                                   *
 *                                                                                 *
 *                                                                                 *
 ***********************************************************************************/

struct{
	volatile u8 	u8StartFlag    		 	;
	volatile u32 	u32Counter[50] 			;
	volatile u8  	u8IndexCounter 			;
	volatile u8 	u8Button   				;
}IR;

struct{
	volatile u8 	u8Flag 					;
}HallEffect;

struct{
	volatile u8 	u8OneCycleDone 			;
	volatile u32	u32PeriodicTime 		;
	volatile u16	u16Frequency			;
	volatile u8 	u8FreqFlag				;

}Motor;

struct{
	volatile u8 	u8FlagsIndex			;
	volatile u8 	u8DisplayFlag			;
	volatile u16 	u16Counter				;
	volatile u8 	u8ShiftWord				;
}POV;

struct{
	volatile u16 	CurrentDistanceBetweenTeeth ;  //  250 or 2500us
	volatile u8 	ArrowFlag					;
	volatile s8 	ArrowCounter 				;
}Clock;

volatile u16	Average_u16Freq[10]	=  {0}	;
const 	 f32	Distance			= 125.6637;

/***********************************************************************************
 * 	                                                                               *
 *                                                                                 *
 * 									Macros		                                   *
 *                                                                                 *
 *                                                                                 *
 ***********************************************************************************/

#define 		RIGHT_Button				100
#define 		LEFT_Button					105
#define 		POWER_Button				110
#define 		OK_Button					115
#define 		P_PLUS_Button				120
#define 		P_MINUS_Button				125
#define 		VOL_PLUS_Button				130
#define 		VOL_MINUS_Button			135

#define 		LEARN						5
#define			MAKE						10
#define 		SHARE						15
#define			WE							20
#define			ARE							25
#define			PIXELS						30
#define 		WE_ARE						35
#define			THANKS						40
#define 		TREND						45


/**
 * @brief this array is and indicator, it will be used by IR when user presses on some button
 * the display will show another word in order
 */
volatile u8 POV_u8Flags[9] = {WE,LEARN,MAKE,SHARE,WE_ARE,PIXELS,THANKS,TREND,0};
/***********************************************************************************
 * 	                                                                               *
 *                                                                                 *
 * 									Global Arrays                                  *
 *                                                                                 *
 *                                                                                 *
 ***********************************************************************************/
/**
 * @brief this is the array which responsible for displaying the short arrow in the Clock
 */
u16 POV_u16ShortArrow[5]={0x8400,0x8200,0xff00,0x4100,0x4200};
/**
 * @brief this is the array which responsible for displaying the long arrow in the Clock
 */
u16 POV_u16LongArrow[5]={0x20,0x10,0xfff8,0x8,0x10};

/**
 * @brief to display Numbers
 */
u16 POV_u16Num1[3]={0x4,0x2,0x3f};
u16 POV_u16Num2[4]={0x32,0x29,0x29,0x26};
u16 POV_u16Num3[7]={0xe,0x1,0x1,0x7,0x1,0x9,0x6};
//		0x6,0x9,0x1,0x7,0x1,0x9,0x6
u16 POV_u16Num6[4]={0x26,0x25,0x25,0x1e};
u16 POV_u16Num9[7]={0xf,0x8,0x8,0xf,0x9,0x9,0xf};
/**
 * @brief we'll use this array in the mix between displaying the short arrow and Number 6
 */
u16 MIX[5]={0};
//	{0x8400,0x8200,0xff00,0x8200,0x8400};
//	{0x8400,0x8200,0xff00,0x4100,0x4200};


void Calc_voidTime(void)
{
	asm("NOP");
}
/**
 * @Description this function calculates the Motor Frequency
 * by reading the the Frequency for 10-times then taking the average
 * to enhance the precision
 *
 * @Motor_Frequency is 9
 */
//void Calc_voidFreq(void)
//{
//	Motor_u8FreqFlag++;
//	Average_u16Freq[Motor_u8FreqFlag-1] = Motor_Frequency;
//	IR_u8Flag = VOL_PLUS_Button;
//	if(Motor_u8FreqFlag == 10)
//	{
//		Motor_Frequency = Average_u16Freq[0];
//		for(u8 count = 1; count <=9; count++)
//		{
//			Motor_Frequency += Average_u16Freq[count];
//		}
//		Motor_Frequency /= 10;
//		HS2P_voidSend16BitSynchronous(Motor_Frequency);
//		IR_u8Flag = 0;
//	}
//}


/**
 * @Description the interrupt gives us the Frequency for testing only
 */
//void Motor_GetFrequency(void)
//{
//
//		if(IR.u8Flag == VOL_PLUS_Button)
//		{
//			HallEffect.Entery_u8Flag = TRUE;
//			IR.u8Flag = FALSE;
//		}
//		if(	HallEffect.Entery_u8Flag == TRUE)
//		{
//			MSTK_voidSetSingleInterval(1000000 , Calc_voidFreq);
//			Motor_Frequency = 0;
//			HallEffect.Entery_u8Flag = FALSE;
//			HS2P_voidSend16BitSynchronous(0xffff);
//		}
//		if(HallEffect.u8Flag == TRUE)
//		{
//			asm("NOP");
//		}
//		else
//		{
//			Motor.Frequency++;
//			HallEffect.u8Flag = TRUE;
//		}
//
//}

void HallEffect_voidIRQ(void)
{
	if(HallEffect.u8Flag == TRUE)
	{
		asm("NOP");
	}
	else
	{
		if(Motor.u8OneCycleDone == TRUE)
		{
			Motor.u32PeriodicTime =	GPTIM2_u16GetElapsedTime();
			if( (IR.u8Button != LEFT_Button) && (IR.u8Button != RIGHT_Button) )
			{
				if( (Motor.u32PeriodicTime > 1000) &&   (Motor.u32PeriodicTime < 60000) )
				{
					Clock.CurrentDistanceBetweenTeeth = ( (Motor.u32PeriodicTime * 135)/11111 );
				}
			}

//			if(IR_u8Flag == POWER_Button)
//				HS2P_voidSend16BitSynchronous( (Motor_u32PeriodicTime/1000) );
//				IR_u8Flag = 0;
//				CurrentDistanceBetweenTeeth = ( (Motor_u32PeriodicTime-50000)/24);
			Motor.u8OneCycleDone = FALSE;
		}
		GPTIM2_voidSetSingleInterval(60000 , 100000 , Calc_voidTime);
		Motor.u8OneCycleDone = TRUE;
		HallEffect.u8Flag = TRUE;
	}

	/* the next function for getting frequency and testing only */
	//Motor_GetFrequency();
}


void voidTakeAction(u32 u32Decision)
{
	/* TV Remote */
	switch(u32Decision)
	{
		//P+ Button
		case 0xed12bd02:
			//IR_u8Flag = POWER_Button;
			switch(IR.u8Button)
			{
			case POWER_Button:
					Clock.ArrowCounter++;
				if(Clock.ArrowCounter >19)
				{
					Clock.ArrowCounter = 0;
				}
				break;
			case OK_Button:
				POV.u8ShiftWord++;
				break;
			}
			break;
			//IR.u8Button = P_PLUS_Button;

		//P- Button
		case 0xec13bd02:
			//IR_u8Flag = POWER_Button;
			switch(IR.u8Button)
			{
			case POWER_Button:
				Clock.ArrowCounter--;
				if( Clock.ArrowCounter < 0 )
				{
					Clock.ArrowCounter = 19;
				}
				break;
			case OK_Button:
				POV.u8ShiftWord--;
				break;
			}
			break;

		//Power Button
		case 0xeb14bd02:
			IR.u8Button = POWER_Button;
			break;

		//>|| Button
		case 0xb748bd02:
			IR.u8Button = OK_Button;
			POV.u8DisplayFlag = POV_u8Flags[POV.u8FlagsIndex];
			POV.u8FlagsIndex++;
			if(POV.u8FlagsIndex == 9)
			{POV.u8FlagsIndex = 0;}
			break;

		//|<< Button
		case 0xe11ebd02:
			IR.u8Button = LEFT_Button;
			Clock.CurrentDistanceBetweenTeeth+=10;
			break;
		//>>| Button
		case 0xe31cbd02:
			IR.u8Button = RIGHT_Button;
			Clock.CurrentDistanceBetweenTeeth-=10;
			break;

		//VOL+ Button
		case 0xef10bd02:
			IR.u8Button = VOL_PLUS_Button;
			break;
		//VOL- Button
		case 0xee11bd02:
			IR.u8Button = VOL_MINUS_Button;
			break;

	}
}
void voidResetArrayElements(volatile u32 *arr , u8 size)
{
	for(u8 i = 0; i<size; i++)
	{
		arr[i] = 0;
	}
}

void IR_voidReadFrame(void)
{
	u32 u32DataFrame = 0;
	if( (IR.u32Counter[0] >= 10000) && (IR.u32Counter[0] <= 17000) )
	{
		for(u8 i = 1; i<=32; i++)
		{
			if( (IR.u32Counter[i] >= 2000) && (IR.u32Counter[i] <= 2475) )
			{
				SET_BIT(u32DataFrame , (i-1));
			}
			else
			{
				CLR_BIT(u32DataFrame , (i-1));
			}
		}
		voidTakeAction(u32DataFrame);
	}
	IR.u8StartFlag    = 0;
	IR.u8IndexCounter = 0;
	voidResetArrayElements(IR.u32Counter , 50);
}
void IR_voidGetFrame(void)
{
	if(IR.u8StartFlag == 0)
	{
		MSTK_voidSetSingleInterval(200000 , IR_voidReadFrame);
		IR.u8StartFlag = 1;
	}
	else
	{
		IR.u32Counter[IR.u8IndexCounter] = MSTK_u32GetElapsedTime();
		MSTK_voidSetSingleInterval(200000 , IR_voidReadFrame);
		IR.u8IndexCounter++;
	}
}

void POV_DisplayMinutes(u8 NumOfMinutes , u16 DistanceBetweenthem )
{
	//if(ArrowCounter == )
	HS2P_voidSend16BitSynchronous(0x0000 | 1<<15);

	for(u8 i = 0; i < NumOfMinutes; i++)
	{
		TIM1_voidSetBusyWait( DistanceBetweenthem , 100000);
		if(Clock.ArrowFlag == Clock.ArrowCounter)
		{
			HS2P_voidSend16BitSynchronous(   POV_u16LongArrow[0] | 1<<15	 		);
			HS2P_voidSend16BitSynchronous(   POV_u16LongArrow[1] | 1<<15	 		);
			HS2P_voidSend16BitSynchronous(  (POV_u16LongArrow[2] | 0x3)  | 1<<15	);
			HS2P_voidSend16BitSynchronous(   POV_u16LongArrow[3] | 1<<15	 		);
			HS2P_voidSend16BitSynchronous(   POV_u16LongArrow[4] | 1<<15	 		);
		}
		else{
			HS2P_voidSend16BitSynchronous(0x3 | 1<<15);
		}
		Clock.ArrowFlag++;
		HS2P_voidSend16BitSynchronous(0x0000 | 1<<15);
		TIM1_voidSetBusyWait( DistanceBetweenthem , 100000);
	}
}
void POV_DisplayClock(void)
{
	/* Display Number 6 and the arrow */
	HS2P_voidSend16BitSynchronous(MIX[0] | 1<<15 );
	HS2P_voidSend16BitSynchronous(MIX[1] | 1<<15 );
	HS2P_voidSend16BitSynchronous(MIX[2] | 1<<15 );
	HS2P_voidSend16BitSynchronous(MIX[3] | 1<<15 );
	HS2P_voidSend16BitSynchronous(MIX[4] | 1<<15 );
	//POV_DisplayMinutes(14 , 1200 );
	POV_DisplayMinutes(5 , Clock.CurrentDistanceBetweenTeeth );

				/* Display Number 9 */
	HS2P_voidSend16BitSynchronous(POV_u16Num9[0] | 1<<15 );
	HS2P_voidSend16BitSynchronous(POV_u16Num9[1] | 1<<15 );
	HS2P_voidSend16BitSynchronous(POV_u16Num9[2] | 1<<15 );
	HS2P_voidSend16BitSynchronous(POV_u16Num9[3] | 1<<15 );
	HS2P_voidSend16BitSynchronous(POV_u16Num9[4] | 1<<15 );
	HS2P_voidSend16BitSynchronous(POV_u16Num9[5] | 1<<15 );
	HS2P_voidSend16BitSynchronous(POV_u16Num9[6] | 1<<15 );
	//POV_DisplayMinutes(14 , 1200 );
	POV_DisplayMinutes(5 , Clock.CurrentDistanceBetweenTeeth );

			/* Display Number 12 */
	HS2P_voidSend16BitSynchronous(POV_u16Num1[0] | 1<<15 );
	HS2P_voidSend16BitSynchronous(POV_u16Num1[1] | 1<<15 );
	HS2P_voidSend16BitSynchronous(POV_u16Num1[2] | 1<<15 );
	HS2P_voidSend16BitSynchronous(0x0000 | 1<<15);
	HS2P_voidSend16BitSynchronous(POV_u16Num2[0] | 1<<15 );
	HS2P_voidSend16BitSynchronous(POV_u16Num2[1] | 1<<15 );
	HS2P_voidSend16BitSynchronous(POV_u16Num2[2] | 1<<15 );
	HS2P_voidSend16BitSynchronous(POV_u16Num2[3] | 1<<15 );
	//POV_DisplayMinutes(14 , 1200 );
	POV_DisplayMinutes(5 , Clock.CurrentDistanceBetweenTeeth );

			/* Display Number 3 */
	HS2P_voidSend16BitSynchronous(POV_u16Num3[0] | 1<<15 );
	HS2P_voidSend16BitSynchronous(POV_u16Num3[1] | 1<<15 );
	HS2P_voidSend16BitSynchronous(POV_u16Num3[2] | 1<<15 );
	HS2P_voidSend16BitSynchronous(POV_u16Num3[3] | 1<<15 );
	HS2P_voidSend16BitSynchronous(POV_u16Num3[4] | 1<<15 );
	HS2P_voidSend16BitSynchronous(POV_u16Num3[5] | 1<<15 );
	HS2P_voidSend16BitSynchronous(POV_u16Num3[6] | 1<<15 );
	POV_DisplayMinutes(5 , Clock.CurrentDistanceBetweenTeeth );
	Clock.ArrowFlag = 0;
//	POV_DisplayMinutes(14 , 1200 );
}
void POV_DisplayWord(u16 *Word , u16 SizeOfWord)
{
	for(u8 i = 0; i<SizeOfWord; i++)
	{
		HS2P_voidSend16BitSynchronous(Word[i]);
	}
}

void main(void)
{
	/********************************************************************************************************
	 *
	 *
	 * 										Words on Display into Array
	 *
	 *
	 ********************************************************************************************************/

	u16 POV_u16PIXELS[30]={
			//P
			0x7f, 0x9, 0x9, 0x6, 0x0,
			//I
			0x41, 0x7f, 0x41, 0x0,
			//X
			0x41, 0x22, 0x14, 0x8, 0x14, 0x22, 0x41, 0x0,
			//E
			0x7f, 0x49, 0x49, 0x0,
			//L
		0x7f, 0x40, 0x40, 0x0,
			//S
			0x2e, 0x49, 0x49, 0x32, 0x0
	};
	u16 POV_u16WE[14]={
			//W
			0x1f, 0x20, 0x1, 0x20, 0x1e, 0x20, 0x1, 0x20, 0x1f, 0x0,
			//E
			0x7f, 0x49, 0x49, 0x0
	};
u16 POV_u16ARE[17]={
		//A
		0x7c, 0x12, 0x11, 0x11, 0x12, 0x7c, 0x0,
		//R
		0x7f, 0x9, 0x9, 0x16, 0x60, 0x0,
		//E
		0x7f, 0x49, 0x49, 0x0
};
u16 POV_u16LEARN[29]={
		//L
		0x7f, 0x40, 0x40, 0x0,
		//E
		0x7f, 0x49, 0x49, 0x0,
		//A
		0x7c, 0x12, 0x11, 0x11, 0x12, 0x7c, 0x0,
		//R
		0x7f, 0x9, 0x9, 0x16, 0x60, 0x0,
		//N
		0x7f, 0x4, 0x8, 0x10, 0x20, 0x40, 0x7f, 0x0
};
u16 POV_u16MAKE[25]={
		//M
		0x7f, 0x2, 0x4, 0xc, 0x4, 0x2, 0x7f, 0x0,
		//A
		0x7c, 0x12, 0x11, 0x11, 0x12, 0x7c, 0x0,
		//K
		0x7f, 0x8, 0x24, 0x22, 0x41, 0x0,
		//E
		0x7f, 0x49, 0x49, 0x0
};
u16 POV_u16SHARE[28]={
		//S
		0x2e, 0x49, 0x49, 0x32, 0x0,
		//H
		0x7f, 0x8, 0x8, 0x8, 0x7f, 0x0,
		//A
		0x7c, 0x12, 0x11, 0x11, 0x12, 0x7c, 0x0,
		//R
		0x7f, 0x9, 0x9, 0x16, 0x60, 0x0,
		//E
		0x7f, 0x49, 0x49, 0x0
};

u16 POV_u16THANKS[36]={
		//T
		0x1, 0x7f, 0x1, 0x0,
		//H
		0x7f, 0x8, 0x8, 0x8, 0x7f, 0x0,
		//A
		0x7c, 0x12, 0x11, 0x11, 0x12, 0x7c, 0x0,
		//N
		0x7f, 0x4, 0x8, 0x10, 0x20, 0x40, 0x7f, 0x0,
		//K
		0x7f, 0x8, 0x24, 0x22, 0x41, 0x0,
		//S
		0x2e, 0x49, 0x49, 0x32, 0x0
};

u16 POV_u16TREND[]={
		//T
		0x1, 0x7f, 0x1, 0x0,
		//R
		0x7f, 0x9, 0x9, 0x16, 0x60, 0x0,
		//E
		0x7f, 0x49, 0x49, 0x0,
		//N
		0x7f, 0x4, 0x8, 0x10, 0x20, 0x40, 0x7f, 0x0,
		//D
		0x7f, 0x41, 0x41, 0x22, 0x1c, 0x0
};

/********************************************************************************************************
 *
 *
 * 										Variables Initialization
 *
 *
 ********************************************************************************************************/


	IR.u8StartFlag						= 0			;
	IR.u8Button 						= 0			;
	IR.u8IndexCounter					= 0 		;

	HallEffect.u8Flag	 				= 0			;

	Motor.u8OneCycleDone 				= 0			;
	Motor.u32PeriodicTime				= 11111.1 	;
	Motor.u16Frequency					= 0			;
	Motor.u8FreqFlag					= 0			;

	POV.u16Counter						= 3520		;
	POV.u8DisplayFlag					= 0			;
	POV.u8ShiftWord						= 64		;

	Clock.ArrowFlag						= 0			;
	Clock.ArrowCounter 					= 0			;
	Clock.CurrentDistanceBetweenTeeth	= 135		;




	/* Initialize Clock 		*/
	RCC_voidInitSysClock();
	RCC_voidEnableClock(RCC_APB2, 2); 	//Enable RCC for GPIOA
	RCC_voidEnableClock(RCC_APB2, 3); 	//Enable RCC for GPIOB
	RCC_voidEnableClock(RCC_APB2, 0); 	//Enable RCC for AFIO
	RCC_voidEnableClock(RCC_APB2, 11);	//Enable RCC for TIMER1
	RCC_voidEnableClock(RCC_APB1, 0);	//Enable RCC for GPTIMER2

	/* Timers Initialization 	*/
	MSTK_voidInit();  					//Initialize SysTick
	TIM1_voidInit();					//Initialize Timer1
	GPTIM2_voidInit();					//Initialize GPTimer2

	/* GPIO Initialization 		*/
	gPIO_voidSetPinDir(GPIOA , PIN0 , OUT_PUSHPULL_10MHz);
	gPIO_voidSetPinDir(GPIOA , PIN1 , OUT_PUSHPULL_10MHz);
	gPIO_voidSetPinDir(GPIOA , PIN2 , OUT_PUSHPULL_10MHz);

	/* EXTI PINs Initialization */
	gPIO_voidSetPinDir(GPIOB , PIN0 , IN_FLOATING);
	gPIO_voidSetPinDir(GPIOB , PIN1 , IN_FLOATING);

	MNVIC_voidEnableInterrupt(6);	  	//Enable NVIC for EXTI0
	MNVIC_voidEnableInterrupt(7);	  	//Enable NVIC for EXTI1
	MNVIC_voidEnableInterrupt(25);		//Enable NVIC for Timer1
	MNVIC_voidEnableInterrupt(28);		//Enable NVIC for GPTimer2

	//EXTI1 Set in Group0 and SubGroup0 to change its Priority
	MNVIC_voidSetPriority(7 , 0 , 0 , NVIC_GROUP_SUB_DISTRIBUTION);
	//EXTI0 Set in Group1 and SubGroup1 to change its Priority
	MNVIC_voidSetPriority(6 , 1 , 1 , NVIC_GROUP_SUB_DISTRIBUTION);


	MAFIO_voidSetEXTIConfig(0 , PB);	//Set EXTI0 on GPIO B0
	MAFIO_voidSetEXTIConfig(1 , PB);	//Set EXTI1 on GPIO B1

	MEXTI_voidSetCallBack(HallEffect_voidIRQ, 0);
	MEXTI_voidSetCallBack(IR_voidGetFrame , 1);

	MEXTI_voidSetSignalLatch(0 , FALLING_EDGE);
	MEXTI_voidSetSignalLatch(1 , FALLING_EDGE);

	HS2P_voidSend16BitSynchronous(0x0000);

	MIX[0] = (POV_u16ShortArrow[0] | POV_u16Num6[0]);
	MIX[1] = (POV_u16ShortArrow[1] | POV_u16Num6[1]);
	MIX[2] = (POV_u16ShortArrow[2] | POV_u16Num6[2]);
	MIX[3] = (POV_u16ShortArrow[3] | POV_u16Num6[3]);
	MIX[4] =  POV_u16ShortArrow[4] ;
	while(1)
	{

		if(HallEffect.u8Flag == TRUE)
		{
			switch(IR.u8Button)
			{
			case POWER_Button:
			POV_DisplayClock();
				break;
			case RIGHT_Button:
			POV_DisplayClock();
				break;
			case LEFT_Button:
			POV_DisplayClock();
				break;

			case OK_Button:
				switch(POV.u8DisplayFlag)
				{
				case WE	:
					TIM1_voidSetBusyWait(POV.u8ShiftWord , 1000);
					POV_DisplayWord(POV_u16WE , 14);
					break;
				case LEARN	:
					TIM1_voidSetBusyWait(POV.u8ShiftWord , 1000);
					POV_DisplayWord(POV_u16LEARN , 29);
					break;
				case MAKE	:
					TIM1_voidSetBusyWait(POV.u8ShiftWord , 1000);
					POV_DisplayWord(POV_u16MAKE , 25);
					break;
				case SHARE	:
					TIM1_voidSetBusyWait(POV.u8ShiftWord , 1000);
					POV_DisplayWord(POV_u16SHARE , 28);
					break;
				case WE_ARE	:
					TIM1_voidSetBusyWait(POV.u8ShiftWord , 1000);
					POV_DisplayWord(POV_u16WE , 14);
					TIM1_voidSetBusyWait(20 , 1000000);
					POV_DisplayWord(POV_u16ARE , 17);
					break;
				case PIXELS	:
					TIM1_voidSetBusyWait(POV.u8ShiftWord , 1000);
					POV_DisplayWord(POV_u16PIXELS , 30);
					break;
				case THANKS	:
					TIM1_voidSetBusyWait(POV.u8ShiftWord , 1000);
					POV_DisplayWord(POV_u16THANKS , 36);
					break;
				case TREND	:
					TIM1_voidSetBusyWait(POV.u8ShiftWord , 1000);
					POV_DisplayWord(POV_u16TREND , 28);
					break;
				case 0	:
						HS2P_voidSend16BitSynchronous(0x0000);
						break;
				}
				break;
			}
			HallEffect.u8Flag = 0;
		}
	}
}

