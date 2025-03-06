#include "play.h"
#include "LED.h"
#include "running.h"

/**
 * called by running to turn on the LED. start playing a song (maybe?)
 */
void play() {
    blink = 0 ; // extern variable from running to adjust the blink status of the file
    LED_On() ; // turn on an LED
}
