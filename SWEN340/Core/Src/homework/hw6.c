#include "hw6.h"
#include "UART.h"
#include "printf.h"
#include "stm32l4xx.h"

uint8_t uart_rx_buffer[B_SIZE] ;
uint8_t write_index = 0 ;
uint8_t read_index = 0 ;

void USART2_IRQHandler()
{
//	printf("called") ;
	if ( USART2->ISR & USART_ISR_RXNE )
	{
//		printf("triggered") ;
		uart_rx_buffer[write_index % B_SIZE] = USART2->RDR ;
		write_index++ ;
	}
}

void output( uint8_t output_char )
{
	printf( "%c", output_char ) ;
}

void hw6()
{
	USART2->CR1 |= 1 << 5 ;
	NVIC_EnableIRQ( USART2_IRQn ) ;
	while( 1 )
	{
		if ( write_index != read_index )
		{
			output( uart_rx_buffer[ ( read_index++ ) % B_SIZE ] ) ;
		}
//		printf("write") ;
	}
}
