#include "stop.h"
#include "LED.h"
#include "running.h"
#include "blink.h"
#include "audio_engine.h"

/**
 * called by running to force the LED to be off
 */
void stop() {
    blink_off() ; // externed from running. stops the flashing process from flashing
    LED_Off() ; 

    music_reset() ;
}
