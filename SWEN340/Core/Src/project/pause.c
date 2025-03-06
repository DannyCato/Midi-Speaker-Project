#include "pause.h"
#include "LED.h"
#include "running.h"

/**
 * called by running and starts the flashing progress
 */
void pause() {
    blink = 1 ; // externed variable to activate the flashing block in running
    LED_On() ;
}
