#include <stdint.h>

#include "LED.h"
#include "UART.h"
#include "systick.h"
#include "printf.h"

static char BUFFME[32] ;

char get_char()
{
	char c = USART_Read_NonBlocking( USART2 ) ;
	if ( c != 0 )
	{
		printf( "%c\n", c ) ;
	}
	return c ;
}

void read_indef()
{
	init_systick() ;

	uint8_t count = 0 ;
	uint8_t index = 0 ;
	char c ;
	while ( 1 )
	{
		if ( ( c = get_char() ) )
		{
			BUFFME[index] = c ;
			if ( BUFFME[index++] == 0x0d )
			{
				BUFFME[index] = 0 ;
				printf( "%s\n", BUFFME ) ;
				index = 0 ;
			}
		}
		printf( "HALTING 1 SEC\n" ) ;
		delay_systick() ;
		printf( "count = %i\n", count ) ;
		LED_Toggle() ;
		count++ ;
	}
}
