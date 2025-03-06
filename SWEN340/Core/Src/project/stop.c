#include "stop.h"
#include "LED.h"
#include "running.h"

/**
 * called by running to force the LED to be off
 */
void stop() {
    blink = 0 ; // externed from running. stops the flashing process from flashing
    LED_Off() ; 
}
