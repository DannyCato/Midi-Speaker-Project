/*
 * printf.c
 *
 * my implementation of print f using no libraries from stdio
 *
 * Created on: 1/23/2025
 * 	By: Daniel Catorcini
 *
 * */

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "UART.h"

#include "printf.h"
//#include "LED.h"
//
//#define TRUE (1)
//#define FALSE (0)

static char LETTERS [128];

int putchar( int c )
{
	uint8_t char_array[3] = { 0x0d, c, 0x00 } ; // 0x0d is ascii for carriage return
	uint8_t* print_ptr = &char_array[1] ;
	int length = 1 ;
	if ( c == 0x0a ) // 0x0a is ascii for line feed
	{
		print_ptr = &char_array[0] ;
		length++ ;
	}
	else if ( c == 0x0d ) // 0x0d is ascii for carriage returns
	{
		print_ptr = &char_array[0] ;
		char_array[1] = 0x0a ;
		length++ ;
	}
	else if ( c == 0x08 ) // 0x08 is ascii for backspace
	{
		print_ptr = &char_array[0] ;
		char_array[0] = 0x08 ;
		char_array[1] = 0x20 ;
		char_array[2] = 0x08 ;
		length = 3 ;
	}
	USART_Write( USART2, print_ptr, length );

	return length ;
}

int puts( const char* string ) {
    return printf( string ) + putchar( 0x0a ) ; // 0x0a is ascii for line feed
}

int printf( const char *format, ... ) {
   va_list aptr ;
   char* letters = LETTERS ;

   va_start(aptr, format) ;
   vsprintf(LETTERS, format, aptr) ;
   va_end(aptr) ;

    uint8_t count = 0 ;
    while (*letters)
    {
        count += putchar( (int) *letters ) ;
        letters++ ;
    }

   return count ;
}

int printnf( int length, const char* format, ... )
{
	va_list aptr ;
	char* letters = LETTERS ;

	va_start(aptr, format) ;
	vsprintf(LETTERS, format, aptr) ;
	va_end(aptr) ;

	uint8_t adjust = 0 ;
	int i = 0 ;
	while ( *letters && i < length )
	{
		if ( *letters == '\n' || *letters == '\r' || *letters == '\b' )
		{
			adjust += putchar( (int) *letters ) ;
		}
		USART_Write( USART2, ( uint8_t* ) letters, 1 ) ;
		i++ ;
		letters++ ;
	}
	return length + adjust ;
}

///*
// * Writes to the USART
// **/
//void write( char* str, uint32_t length)
//{
//    USART_Write( USART2, (uint8_t*) str, length );
//}
//
//
////#define SIZE (BUFFER_SIZE * 4)
////
///*
// * Used for escape sequence handling.
// * Sets the character c to the place in buffer indicated by index
// * then increments index
// **/
//void set_char( char c, char* buffer, uint8_t* index )
//{
//    buffer[(*index)++] = c ;
//}
//
///*
// * Implementation of a printf function using vsprintf, but
// * it is limited to a buffer of 4 * BUFFER_SIZE in UART.c
// **/
//int printf(const char* format, ...)
//{
//	char buffer[SIZE] ;
//    memset( buffer, '\0', SIZE ) ;
//    char write_buff[BUFFER_SIZE] ;
//	va_list args ;
//	int ret ;
//
//	va_start( args, format ) ;
//	ret = vsprintf( buffer, format, args ) ;
//	va_end( args ) ;
//
//    if ( ret <= 0 )
//    {
//        return ret ;
//    }
//
//	uint8_t length = ret ;
//    uint8_t i, wb_relative_index ;
//	for ( i = 0, wb_relative_index = 0 ; i < length ; i++, wb_relative_index++ )
//    {
//        if ( i % BUFFER_SIZE == BUFFER_SIZE - 2 )
//        {
//            write_buff[BUFFER_SIZE - 1] = '\0' ;
//            write( write_buff , BUFFER_SIZE ) ;
//            wb_relative_index += 2 ;
//            i++ ;
//        }
//        write_buff[wb_relative_index % BUFFER_SIZE] = buffer[i] ;
//    }
//    write_buff[i] = '\0' ;
//    write( write_buff, ( i % 32 ) + 1 ) ;
//
//	return ret ;
//}
//
//void base_to_string( uint64_t num, char* buffer, uint8_t* index, uint8_t base, uint8_t is_signed )
//{
//	if ( base > 16)
//	{
//		return ;
//	}
//
//	uint8_t is_negative = 0 ;
//	if ( is_signed )
//	{
//		uint64_t MSB = 0x80000000 ; // Magic number with only the first bit set
//		if ( num & MSB ) // if num has first bit set (negative flag)
//		{
//			is_negative = 1 ;
//			num = ~num + 1 ; // flip by two's complement
//			// edge case if only most significant base is on.
//			if ( num == MSB ) // check for this ^^^
//			{
//				char* message = "overflow" ;
//				while ( *message ) {
//					set_char( *message++, buffer, index ) ;
//				}
//				return ;
//			}
//		}
//	}
//
//	char values[16] = "0123456789ABCDEF" ;
//
//	uint8_t front = *index ;
//	uint8_t length = 0 ;
//	do
//	{
//		set_char( values[num % base], buffer, index ) ;
//		num /= base ;
//		length++ ;
//	} while ( num ) ;
//
//	if ( is_negative )
//	{
//		set_char( '-', buffer, index ) ;
//	}
//
//	char storage ;
//	uint8_t front_ptr ;
//	uint8_t back_ptr ;
//	for ( uint8_t i = 0 ; i < length / 2 ; i++ ) // reverse string because it was put in backwards
//	{
//		front_ptr = front + i ;
//		back_ptr = length - i - 1 ;
//		storage = buffer[front + back_ptr] ;
//		buffer[front + back_ptr] = buffer[front_ptr] ;
//		buffer[front_ptr] = storage ;
//	}
//
//	return ;
//}
//
///*
// * Seperate implementation of printf that is not limited
// * by the buffer needed in vsprintf. Uses majority functions
// * created by me to do the work, but takes longer than printf
// **/
//int blprintf( const char* format, ... )
//{
//	LED_On() ;
//	va_list args ;
//    va_start( args, format ) ;
//
//    char buffer[BUFFER_SIZE];
//
//    uint8_t index = 0 ;
//    while ( *format )
//    {
//        if ( *format == '%' ) // FORMAT
//        {
//            switch ( *(++format) )
//            {
//            case 'd' : // integer case
//                jmp:
//                long n = va_arg( args, long ) ;
//                base_to_string(n, buffer, &index, 10, TRUE ) ;
//                break ;
//            case 'i' : // other integer case
//                goto jmp ;
//                break ;
//            case 'u' : // unsigned int case
//                uint64_t un = va_arg( args, int ) ;
//                base_to_string( un, buffer, &index, 10, FALSE ) ;
//                break ;
//            case 'c' : // character cased
//                int c = va_arg( args, int ) ;
//                set_char( c, buffer, &index ) ;
//                break ;
//            case 's' : // string case
//                char* s = va_arg( args, char *) ;
//                while ( *s )
//                {
//                    set_char(*s, buffer, &index ) ;
//                    if ( index >= 31 )
//                    {
//                        buffer[index] = '\0' ;
//                        write( buffer , index ) ;
//                        index = 0 ;
//                    }
//                    ++s ;
//                }
//                break ;
//            case '%' : // % case
//                set_char( '%', buffer, &index ) ;
//                break ;
//            // Canned because I'm not entirely sure how
//            // I want to implement it at this moment.
//            // If needed I would gladly write it though
//            // case 'f' : // float case
//            //     float f = va_arg( args, float ) ;
//            //     ftoa() ;
//            case 'x' : // hexadecimal case
//                uint64_t x = va_arg(args, long) ;
//                base_to_string( x, buffer, &index, 16, FALSE ) ;
//                break;
//            case 'o' : // octal case
//                uint64_t o = va_arg(args, long) ;
//                    base_to_string( o, buffer, &index, 8, FALSE ) ;
//                    break;
//            default :
//                set_char(*(--format), buffer, &index ) ;
//                break;
//            }
//        }
//        else if ( *format == '\n' ) // ESCAPE
//        {
//            set_char(0x0a, buffer, &index ) ; // LINE FEED
//            set_char(0x0d, buffer, &index ) ; // CARRIAGE RETURN
//        }
//        else // REGULAR
//        {
//            set_char(*format, buffer, &index ) ;
//        }
//        buffer[index] = '\0' ;
//        write( buffer, index ) ;
//        index = 0 ;
//        LED_Toggle() ;
////        USART_Delay( 150000 ) ; // debug delay to see if loop actually runs
//        ++format;
//    }
//    va_end( args ) ;
//    LED_Off() ;
//
//    return 1 ;
//}
