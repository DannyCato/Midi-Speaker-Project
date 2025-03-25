#include "printf.h"
#include "GPIO.h"
#include "LED.h"
#include "stm32l4xx.h"

void EXTI9_5_IRQHandler()
{
	printf("P");
	HAL_GPIO_EXTI_IRQHandler(S1_Pin) ;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	printf("I'm gonna crash out") ;
}

void activity10()
{
	GPIO_Init() ;
	GPIOA->MODER |= 1 << 9 ;

	NVIC_EnableIRQ(EXTI9_5_IRQn) ;

//	uint8_t gate = -1 ;
//	while( 1 )
//	{
//		if ( GPIOA->IDR &= 1 << 9 )
//		{
//			if ( gate == 1 )
//			{
//				LED_On() ;
//				printf("TRUTH!\n") ;
//
//			}
//			gate = 0 ;
//		} else {
//			if ( gate == 0 )
//			{
//				LED_Off() ;
//				printf("False\n") ;
//			}
//			gate = 1 ;
//		}
//	}
}
