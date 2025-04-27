#include "pause.h"
#include "LED.h"
#include "running.h"
#include "blink.h"
#include "audio_engine.h"

/**
 * called by running and starts the flashing progress
 */
void pause() {
    blink_on() ; // externed variable to activate the flashing block in running
    LED_On() ;

    if ( music_status() )
    {
        set_offset_time() ;
        music_off() ;
    } 
    else if ( get_start_time() != 0 )
    {
        music_resume() ;
    }
    
    
}
