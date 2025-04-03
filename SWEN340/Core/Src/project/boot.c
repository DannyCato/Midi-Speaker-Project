#include "boot.h"

#include "LED.h"
#include "printf.h"
#include "running.h"
#include "printf.h"
#include "help.h"
#include "systick.h"
#include "local_controller.h"

/**
 * called before program actually starts to make sure that the environment
 * is properly setup
 */
void boot_up(uint8_t help_on_start)
{
	reset_clock() ; 

	NVIC_EnableIRQ( USART2_IRQn ) ; // enable interrupts for USART2
	USART2->CR1 |= 1 << 5 ;	 // enable RXNEIE for it

	NVIC_EnableIRQ( EXTI15_10_IRQn ) ; // enable interrupts for B1
	NVIC_EnableIRQ( EXTI9_5_IRQn ) ; // enable interrupts for S1
	initialize_local() ;

	if ( help_on_start )
	{
		help() ;
	}
	LED_Off() ;

	init_systick() ;
	toggle_systick_interrupt() ;

	printf( "***REMOTE MODE ACTIVE***\n" ) ;
	running() ;
}
