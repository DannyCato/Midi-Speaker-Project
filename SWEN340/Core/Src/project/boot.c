#include "boot.h"

#include "LED.h"
#include "running.h"
#include "printf.h"
#include "help.h"
#include "systick.h"
#include "remote_controller.h"

/**
 * called before program actually starts to make sure that the environment
 * is properly setup
 */
void boot_up(uint8_t help_on_start)
{
	if ( help_on_start )
	{
		help() ;
	}

	LED_Off() ;
	init_systick() ;
	running() ;
}
