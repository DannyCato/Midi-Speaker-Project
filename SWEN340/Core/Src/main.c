// main.c with demo
// Updated and refactored Larry Kiser copyright 2021


// include project specific header files
#include "error_handler.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "project.h"

#include "printf.h"
#include "endianConverter.h"
	
int main(void){

	// initialization code
	System_Clock_Init(); // set System Clock = 80 MHz
	LED_Init();
	UART2_Init();
	
	// application run function
//	run_project() ;
// 	activity4() ;
	uint8_t byte = 0b10101010 ;
	printf ( "answer = %x", reverse_byte( byte ) ) ;
}

