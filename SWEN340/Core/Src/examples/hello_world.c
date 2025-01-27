/*
 * hello_world.c
 *
 * Created on: Jan 21, 2025
 * 		Author: dgc5878
 */

#include "stdio.h"
#include "UART.h"

#define BUFFERSIZE (80);

void hello() {
	int n ;
	uint8_t buffer[BUFFER_SIZE];

	n = sprintf((char *)buffer, "Hello, World!\r\n");
	USART_Write(USART2, buffer, n);
}
