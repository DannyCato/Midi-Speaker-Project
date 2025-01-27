// main.c with demo
// Updated and refactored Larry Kiser copyright 2021


// include project specific header files
#include "error_handler.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "demo.h"
#include "printf.h"

	
int main(void){

	// initialization code
	System_Clock_Init(); // set System Clock = 80 MHz
	LED_Init();
	UART2_Init();
	
	// application run function
//	run_demo() ;
//	hello ();
	printf("as %d %u %c %s %s \n", 1, 15, 'O', "please", "I Need something decently long, over 32");
}

