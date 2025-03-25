#include <stdint.h>

#include "printf.h"
#include "GPIO.h"
#include "LED.h"


// print a message every time yuor button is pressed

void activity9()
{
	GPIO_Init() ;
	GPIOA->MODER |= 1 << 9 ;

	uint8_t gate = -1 ;
	while( 1 )
	{
		if ( GPIOA->IDR &= 1 << 9 )
		{
			if ( gate == 1 )
			{
				LED_On() ;
				printf("TRUTH!\n") ;

			}
			gate = 0 ;
		} else {
			if ( gate == 0 )
			{
				LED_Off() ;
				printf("False\n") ;
			}
			gate = 1 ;
		}
	}
}
