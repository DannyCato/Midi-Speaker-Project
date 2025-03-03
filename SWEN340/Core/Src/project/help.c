#include "printf.h"
#include "help.h"

char const clear_term[] = {0x1b, '[', 'H', 0x1b, '[', '2', 'J', '\0'} ;

void help() {
	printf( clear_term );
	printf( "%s\n %s\n  %s\n  %s\n  %s\n  %s\n",
				"***REMOTE LED CONTROL MENU***",
				"Available User Commands",
				"NEXT - Show next song info",
				"PLAY - Play the next song (LED On)",
				"PAUSE - Pause the song (LED Flash)",
				"STOP - Stop the song (LED OFF)" ) ;
}
