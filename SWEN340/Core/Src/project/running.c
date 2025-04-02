#include <string.h>

#include "printf.h"
#include "UART.h"
#include "LED.h"
#include "systick.h"

#include "running.h"
#include "help.h"
#include "next.h"
#include "play.h"
#include "pause.h"
#include "stop.h"
#include "blink.h"


char BUFFER[32];
uint8_t write_index = 0 ;
uint8_t put_index = 0 ;

// array of all keywords
const char* cmd_checkers[] = {"HELP", "NEXT", "PLAY", "PAUSE", "STOP"};

// typedef of all functions that are used. made into an array for easy expansion
typedef void ( *func_t )();
func_t functions[] = {help, next, play, pause, stop};

/**
 * returns a single character from the USART buffer and echoes it to the file
 *
 * @return char the next character to be read from the buffer
 */
char get_char()
{
	char c = USART_Read_NonBlocking( USART2 ) ;
	if ( c != 0 )
	{
		printnf( 1, "%c", c ) ;
	}
	return c ;
}

int check = 0 ;

void USART2_IRQHandler()
{
	uint8_t c = USART2->RDR ;
	switch ( c )
	{
		case '\r':
			BUFFER[write_index++] = 0 ;
			check = 1 ;
			break ;
			
		case '\b':
			check = 2 ;
			break ;

		default:
			if ( c >= 97 && c <= 122 ) 
			{
				c &= 0b11011111 ; // Magic number that forces all letters to be represented by a capital through setting the is_lower_case bit to always be false
			}
			BUFFER[write_index++] = c ; 
			break;
	}
}

void check_names()
{
	int invalid = 1 ; // invalid checker set
	for ( int i = 0 ; i < ARR_SIZE( cmd_checkers ) ; i++ ) // part that actually checks each word
	{
		if ( ! (strcmp( BUFFER, cmd_checkers[i] ) ) ) // if equal
		{
			invalid = 0 ; // stop from printing invalid
			functions[i]() ; // get function pointer from array and run it
		}
	}
	if ( invalid ) // if no keywords matched
	{
		printf("Invalid Command\n") ;
	}
	write_index = 0 ;
	put_index = 0 ;
}

void check_backspace()
{
	if ( write_index > 0 ) { // if buffer is not empty
		write_index-- ;
		BUFFER[write_index] = 0 ;
	}
}

void put_input_to_terminal()
{
	if ( put_index == write_index )
	{
		return ;
	}
	else if ( put_index < write_index )
	{
		putchar( BUFFER[put_index++] ) ;
	}
	else if ( put_index > write_index )
	{
		putchar( '\b' ) ;
		put_index-- ;
	}
}

/**
 * main function for this application, called after boot-up sequence in boot.c. An endless loop that handles all the general actions of the program.
 * Specifically it coordinates the writing and reading of buffer to activate certain methods based on keywords.
 * It also handles the blinking functionality as it needs to be in-line with the general loop functionality
 */
void running() {
	


	while( 1 )
	{
		if ( check == 1 ) // check is set by USART_IRQHandler and tells the main loop that a character worthy of a check has been pushed
		{
			printf("\n") ;
			check_names() ;
			check = 0 ;
		}
		else if ( check == 2 )
		{
			check_backspace() ;
			check = 0 ;
		}
		
		put_input_to_terminal() ;

		blink_handler() ;
	}
}
