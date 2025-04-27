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
#include "local_controller.h"
#include "audio_engine.h"


char BUFFER[32];
uint8_t write_index = 0 ;
uint8_t put_index = 0 ;

// array of all keywords
const char* cmd_checkers[] = {"HELP", "NEXT", "PLAY", "PAUSE", "STOP"};

// typedef of all functions that are used. made into an array for easy expansion
typedef void ( *func_t )();
func_t functions[] = {help, next, play, pause, stop};

int check = 0 ;

/**
 * Interrupt handler for USART2
 */
void USART2_IRQHandler()
{
	uint8_t c = USART2->RDR ; // always reads so that if in local mode it does not brick
	if ( !get_local_status() ) // if not in local state
	{
		switch ( c )
		{
			case '\r': // new line
				BUFFER[write_index++] = 0 ;
				check = 1 ;
				break ;
				
			case '\b': // backspace
				check = 2 ;
				break ;

			default: // capitalize and put to buffer
				if ( c >= 97 && c <= 122 ) 
				{
					c &= 0b11011111 ; // Magic number that forces all letters to be represented by a capital through setting the is_lower_case bit to always be false
				}
				BUFFER[write_index++] = c ; 
				break;
		}
	}
}

/**
 * Checks the names of the functions against the input buffer
 */
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
	write_index = 0 ; // restart
	put_index = 0 ; // restart
}

/**
 * Checks if the last was a backspace and handles accordingly
 */
void check_backspace()
{
	if ( write_index > 0 ) { // if buffer is not empty
		write_index-- ;
		BUFFER[write_index] = 0 ;
	}
}

/**
 * Puts a new character to the terminal from the buffer
 */
void put_input_to_terminal()
{
	if ( put_index == write_index ) // early return if nothing new
	{
		return ;
	}
	else if ( put_index < write_index ) // if a characters was added
	{
		putchar( BUFFER[put_index++] ) ;
	}
	else if ( put_index > write_index ) // if a character was removed
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
		if ( !get_local_status() ) // if not local
		{
			if ( check == 1 ) // check is set by USART_IRQHandler and tells the main loop that a character worthy of a check has been pushed
			{
				printf("\n") ; // put on a new line
				check_names() ;
				check = 0 ;
			}
			else if ( check == 2 ) // erase a character
			{
				check_backspace() ;
				check = 0 ;
			}
			
			put_input_to_terminal() ; // put a new character to the terminal 
		}
		else
		{
			local_handler() ; // do local tasks
		}
		blink_handler() ; // handle blinking operations

		if ( music_status() )
		{
			do_music() ;
		}
	}
}
