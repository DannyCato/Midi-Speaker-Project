
#include "UART.h"
#include "printf.h"

char read_and_print ()
{
	while ( 1 )
	{
		uint8_t ch = USART_Read( USART2 ) ;
		putchar( (int) ch ) ;
	}
}
