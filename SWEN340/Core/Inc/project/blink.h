#ifndef BLINK
#define BLINK

void SysTick_Handler() ;

void blink_handler() ;

uint8_t get_blink_status() ;

void blink_on() ;

void blink_off() ;

void blink_toggle() ;

#endif
