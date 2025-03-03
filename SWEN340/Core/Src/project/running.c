#include <string.h>

#include "printf.h"
#include "UART.h"
#include "LED.h"
#include "systick.h"

#include "running.h"
#include "help.h"
#include "next.h"

#define


char BUFFER[32];

char* cmd_checkers[] = {"HELP", "NEXT", "PLAY", "PAUSE", "STOP"};

typedef void ( *func_t )();
func_t functions[] = {help, next } ;

int size()

void init_commands() {

}

char get_char()
{
	char c = USART_Read_NonBlocking( USART2 ) ;
	if ( c != 0 )
	{
		printnf( 1, "%c", c ) ;
	}
	return c ;
}

void running() {
	char capitalizer = 0b11011111 ; // Magic number that forces all letters to be represented by a capital through setting the capitalizer bit to always be zero
	uint8_t c ;
	int i = 0 ;
	int blink = 1 ;
	int blink_counter = 0 ;
	while( 1 )
	{

		if ( ( c = get_char() ) ) { // check char block
			if ( c >= 97 && c <= 122 )
			{
				c &= capitalizer ;
			}
			if ( c == '\r' || c == '\n' )
			{
				if ( !(strcmp( BUFFER, cmd_list->Help ) ) )
				{
					help();
				}
				else if ( !(strcmp(BUFFER, cmd_list->Next ) ) )
				{
					printf( "NEED IMPLEMENTATION FOR %s\n", cmd_list->Next );
//					next();
				}
				else if ( !(strcmp(BUFFER, cmd_list->Play ) ) )
				{
					blink = 0 ;
					printf( "NEED IMPLEMENTATION FOR %s\n", cmd_list->Play );
//					play();
				}
				else if ( !(strcmp(BUFFER, cmd_list->Pause ) ) )
				{
					blink = 1 ;
					printf( "NEED IMPLEMENTATION FOR %s\n", cmd_list->Pause );
//					pause();
				}
				else if ( !(strcmp(BUFFER, cmd_list->Stop ) ) )
				{
					blink = 0 ;
					printf( "NEED IMPLEMENTATION FOR %s\n", cmd_list->Stop );
//					stop();
				}
				else // TESTING PART
				{
					printf( "%s\n", BUFFER ) ;
				}
				i = 0 ;
			}
			else if ( c == '\b' )
			{
				if ( i > 0 ) {
					i--;
					BUFFER[i] = 0 ;
				}
			}
			else
			{
				BUFFER[i] = c ;
				BUFFER[i + 1] = 0 ;
				i++ ;
			}
		} // end check char block

		if ( blink )  // begin blink check block
		{
			while ( !( GLBL_SYSTICK->CTRL & 0x10000 ) ) {} // hold until COUNT bit is set
			blink_counter++;
			if (blink_counter == 1000) {
				LED_Toggle();
				blink_counter = 0 ;
			}
		}
	}
}
