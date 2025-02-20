#include "boot.h"
#include "printf.h"

static char clear_term[] = {0x9b, 0x32, 0x4a} ;

void boot_up()
{
	printf( "%s", clear_term ) ;
	printf( "%s\n %s\n  %s\n  %s\n  %s\n",
			"***REMOTE LED CONTROL MENU***",
			"Available User Commands",
			"NEXT - Show next song info",
			"PAUSE - Pause the song (LED Flash)",
			"STOP - Stop the song (LED OFF)" ) ;

}
