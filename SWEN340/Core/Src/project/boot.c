#include "boot.h"
#include "running.h"
#include "printf.h"
#include "help.h"
#include "systick.h"

void boot_up(uint8_t help_on_start)
{
	printnf( 7, "%s", clear_term ) ;
	if ( help_on_start )
	{
		help();
	}

	init_commands() ;
	init_systick() ;
	running() ;
}
