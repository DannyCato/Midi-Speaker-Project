#include "pause.h"
#include "LED.h"
#include "running.h"
#include "blink.h"

/**
 * called by running and starts the flashing progress
 */
void pause() {
    blink_on() ; // externed variable to activate the flashing block in running
    LED_On() ;
}
