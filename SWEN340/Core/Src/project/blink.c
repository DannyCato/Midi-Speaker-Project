#include <stdint.h>
#include "blink.h"
#include "systick.h"
#include "printf.h"
#include "LED.h"

// boolean if the light should be blinking
uint8_t blink = 0 ;
// the amount of time the blink will last in either state 
uint32_t half_cylce = 1000000 ;

/**
 * Hanldes blinking if the blink boolean is set true
 */
void blink_handler()
{
    if ( blink )  // begin blink check block
    {
        uint32_t blink_counter =  get_clock() % ( half_cylce * 2 ) ;
        if ( blink_counter > half_cylce ) // once it reaches 1000, let the LED turn on
        {
            LED_On() ;
        } 
        else // otherwise turn off
        {
            LED_Off() ;
        }
    } // end blink check block
}

/**
 * Gets if the light is blinking
 */
uint8_t get_blink_status()
{
    return blink ;
}

/**
 * Turns blink on
 */
void blink_on()
{
    blink |= 1 ;
}

/**
 * Turns blink off
 */
void blink_off()
{
    blink &= 0 ;
}

/**
 * Toggles blink
 */
void blink_toggle()
{
    blink ^= 1 ;
}
