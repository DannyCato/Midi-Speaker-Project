
#include "UART.h"
#include "printf.h"

static char COMMAND[32] ;

char read_and_print ()
{
		uint8_t ch = USART_Read( USART2 ) ;
		putchar( ( int ) ch );
		return ch ;
}

// echo the characters read
char* read_line()
{
	int i ;
	for ( i = 0 ; ( COMMAND[i] = read_and_print() ) != 0x0d ; i++) {}
	COMMAND[i] = 0 ;
	return COMMAND ;
}

void activity6()
{
	for ( ; ; )
	{
		printf( "%s\n", read_line() ) ;
	}
}
