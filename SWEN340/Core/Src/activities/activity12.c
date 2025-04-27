#include <stdint.h>

#include "systick.h"
#include "printf.h"
#include "tone.h"
#include "DAC.h"

void activity12( uint32_t frequency )
{
	uint32_t half_cycle = frequency / 2 ;
    uint8_t direction = 0 ;
    uint8_t last = direction ;
    uint8_t* message[] = {"0", "1"} ;
    init_systick() ;
    toggle_systick_interrupt() ;
    DAC_Init() ;
    DAC_Start() ;

    uint32_t rate = hertz_to_midi_ticks( ( ( float ) frequency ) / 2 ) ;

    uint8_t note = 0, added = 0 ;
    uint32_t count = 0, count2 = 0 ;
    while( 1 )
    {
        if (get_clock() % ( rate << 1 ) / rate )
        {
            DAC_Set_Value( 4000 ) ;
            add_tone( note, 100 ) ;
        }
        else
        {
            DAC_Set_Value( 0 ) ;
            remove_tone( note ) ;
        }        

        count++ ;
        count2++ ;
        if ( count % 55555 == 0 )
        {
            count = 0 ;
            printf("  %d = note  ", note) ;
            DAC_Set_Value( 0 ) ;
            remove_tone(note) ;
            note = note + 1  ;
            if ( note == 116 )
            {
                note = 0 ;
            }
            rate = hertz_to_midi_ticks(get_frequency(note)) ;
        }
    }

}
