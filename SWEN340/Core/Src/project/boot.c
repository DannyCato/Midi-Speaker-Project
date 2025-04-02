#include "boot.h"

#include "LED.h"
#include "running.h"
#include "printf.h"
#include "help.h"
#include "systick.h"
#include "stm32l4xx.h"
#include "GPIO.h"

/**
 * called before program actually starts to make sure that the environment
 * is properly setup
 */
void boot_up(uint8_t help_on_start)
{
	NVIC_EnableIRQ( USART2_IRQn ) ; // enable interrupts for USART2
	USART2->CR1 |= 1 << 5 ;	 // enable RXNEIE for it

	if ( help_on_start )
	{
		help() ;
	}
	LED_Off() ;

	GPIO_Init() ;

	init_systick() ;
	GLBL_SYSTICK->CTRL |= 1 << 1 ; // enable systick interrupts

	running() ;
}
