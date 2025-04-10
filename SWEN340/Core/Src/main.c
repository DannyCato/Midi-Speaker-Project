// main.c with demo
// Updated and refactored Larry Kiser copyright 2021


// include project specific header files
#include "error_handler.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "project.h"

#include "hw8.h"
	
int main(void){

	// initialization code
	System_Clock_Init(); // set System Clock = 80 MHz
	LED_Init();
	UART2_Init();
	
	// application run function
	// run_project() ;
	uint8_t delay1[] = { 0x3B, 0x80, 0x27, 0x00 } ;
	printf( "Delay of %d ", parse_delay( delay1 ) ) ;

	uint8_t delay2[] = { 0x83, 0x18, 0x91, 0x54, 0x64 } ;
	printf( "Delay of %d ", parse_delay( delay2 ) ) ;

	uint8_t delay3[] = { 0xC5, 0x92, 0x74, 0xE3, 0x74, 0x12 } ;
	printf( "Delay of %d ", parse_delay( delay3 ) ) ;

	uint8_t delay4[] = { 0x90, 0x82, 0x93, 0x64, 0xD3, 0x89 } ;
	printf( "Delay of %d ", parse_delay( delay4 ) ) ;
}

