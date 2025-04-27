#include "play.h"
#include "audio_engine.h"
#include "LED.h"
#include "blink.h"
#include "dac.h"


/**
 * called by running to turn on the LED. start playing a song (maybe?)
 */
void play() {
    
    blink_off() ; // extern variable from running to adjust the blink status of the file
    LED_On() ; // turn on an LED

    init_music() ;
    music_on() ;
}

