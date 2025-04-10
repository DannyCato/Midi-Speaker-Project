#include <stdint.h>

#include "systick.h"
#include "printf.h"
#include "tone.h"

void activity12( uint32_t frequency )
{
	uint32_t half_cycle = 1000 / frequency / 2 ;
    uint8_t direction = 0 ;
    uint8_t last = direction ;
    uint8_t* message[] = {"0", "1"} ;
    init_systick() ;
    toggle_systick_interrupt() ;

    while( 1 )
    {
        if ( ( get_clock() % (half_cycle << 1) ) < half_cycle )
        {
            direction = 1 ;
        }
        else
        {
            direction = 0 ; 
        }

        if ( last != direction )
        {
            last = direction ;
            printf("%s", message[direction] ) ;
        }
    }

}
