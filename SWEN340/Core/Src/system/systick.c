#include "systick.h"

#include "stm32l4xx_hal.h"

// This function is to Initialize SysTick registers
void init_systick()
{
	// Use the SysTick global structure pointer to do the following in this
	// exact order with separate lines for each step:
	//
	// Disable SysTick by clearing the CTRL (CSR) register.
	SysTick->CTRL &= 0 ;
	// Set the LOAD (RVR) to 8 million to give us a 100 milliseconds timer.
	SysTick->LOAD = 7999999 ;
	// Set the clock source bit in the CTRL (CSR) to the internal clock.
	SysTick->CTRL |= (1 << 2) ;
	// Set the enable bit in the CTRL (CSR) to start the timer.
	SysTick->CTRL |= 1 ;
}

// This fuction is to create delay using SysTick timer counter
void delay_systick()
{
	// Using the SysTick global structure pointer do the following:
	// Create a for loop that loops 10 times
	for ( int i = 0 ; i < 10 ; i++ )
	{
	// Inside that for loop check the COUNTFLAG bit in the CTRL (CSR)
	// register in a loop. When that bit is set exit this inner loop
	// to do another pass in the outer loop of 10.
		while ( SysTick->CTRL ^ SysTick_CTRL_COUNTFLAG_Msk ) {} // XOR so that the mask can always be true but if both are true then it exits
	}
}
