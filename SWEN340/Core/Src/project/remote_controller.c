#include <stdint.h>
#include "remote_controller.h"

uint8_t remote = 1 ;

uint8_t get_remote_status()
{
	return remote ;
}

void remote_toggle()
{
	remote ^= 1 ;
}

void remote_on()
{
	remote |= 1 ;
}

void remote_off()
{
	remote &= 0 ;
}
