#include <stdint.h>
#include "blink.h"
#include "LED.h"

uint8_t blink = 0 ;
uint16_t blink_counter = 0 ;
void SysTick_Handler()
{
	blink_counter++ ;
}

void blink_handler()
{
    if ( blink )  // begin blink check block
    {
        if (blink_counter >= 1000) { // once it reaches 1000, let the LED be toggeled
            LED_Toggle();
            blink_counter = 0 ;
        }
    } // end blink check block
}

uint8_t get_blink_status()
{
    return blink ;
}

void blink_on()
{
    blink |= 1 ;
}

void blink_off()
{
    blink &= 0 ;
}

void blink_toggle()
{
    blink ^= 1 ;
}