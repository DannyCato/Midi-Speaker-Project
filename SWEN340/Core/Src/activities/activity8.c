#include <stdint.h>
#include "systick.h"
#include "printf.h"

uint8_t i = 0 ;

void SysTick_Handler()
{
	i++ ;
}

void activity8()
{
	init_systick() ;
	GLBL_SYSTICK->CTRL |= 1 << 1;
	uint8_t runs = 0 ;
	while ( 1 )
	{
		if ( i >= 1000 )
		{
			printf( "Printing = %i\n", runs ) ;
			i = 0 ;
			runs++ ;
		}
	}
}
