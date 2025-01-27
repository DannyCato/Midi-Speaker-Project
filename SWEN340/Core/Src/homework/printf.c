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
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <LED.h>

#include <UART.h>

#define TRUE (1)
#define FALSE (0)

/*
 * Writes to the USART
 **/
void write( char* str, uint32_t length)
{
    USART_Write( USART2, str, length );
}

/*
 * Used for escape sequence handling.
 * Sets the character c to the place in buffer indicated by index
 * then increments index
 **/
void set_char( char c, char* buffer, uint8_t* index )
{
    buffer[(*index)++] = c ;
}

uint8_t count_digits( long n ) 
{
    uint8_t answer = 0 ;
    while ( n > 0 )
    {
        ++answer ;
        n /= 10 ;
    }
    return answer ;
}

uint8_t count_digits_u( uint64_t n ) 
{
    uint8_t answer = 0 ;
    while ( n > 0 )
    {
        ++answer ;
        n /= 10 ;
    }
    return answer ;
}

void base_to_string( uint64_t num, char* buffer, uint8_t* index, uint8_t base, uint8_t is_signed )
{
	if ( base > 16)
	{
		return ;
	}

	uint8_t is_negative = 0 ;
	if ( is_signed )
	{
		uint64_t MSB = 0x8000000000000000 ; // Magic number with only the first bit set
		if ( num & MSB )
		{
			is_negative = 1 ;
			num = ~num + 1 ; // flip by two's complement
			// edge case if only most significant base is on.
			if ( num == 0x8000000000000000 ) // check for this ^^^
			{
				char* message = "overflow" ;
				while ( *message ) {
					set_char( *message, buffer, index ) ;
				}
				return ;
			}
		}
	}

	char values[16] = "0123456789ABCDEF" ;

	uint8_t front = *index ;
	uint8_t length = 0 ;
	do
	{
		set_char( values[num % base], buffer, index ) ;
		num /= base ;
		length++ ;
	} while ( num ) ;

	if ( is_negative )
	{
		set_char( '-', buffer, index ) ;
	}

	char storage ;
	for ( uint8_t i = 0 ; i < length / 2 ; i++ )
	{
		storage = buffer[front + length + i] ;
		buffer[front + length + i] = buffer[front + i] ;
		buffer[front + i] = storage ;
	}

	return ;
}

int printf( const char* format, ... ) 
{
	LED_On() ;
	va_list args ;
    va_start( args, format ) ;

    char buffer[BUFFER_SIZE];

    uint8_t index = 0 ;

//    while ( *format )
//    {
//		buffer[index++] = *format++ ;
//    }
//    buffer[index] = '\0' ;
//    write(buffer, index) ;
    while ( *format )
    {
        if ( *format == '%' ) // FORMAT
        {
            switch ( *(++format) )
            {
            case 'd' : // integer case
                jmp:
            	int n = va_arg( args, int ) ;
            	base_to_string(n, buffer, &index, 10, TRUE ) ;
                break ;
            case 'i' : // other integer case
            	goto jmp ;
            	break ;
            case 'u' : // unsigned int case
                uint64_t un = va_arg( args, uint64_t) ;
                base_to_string(un, buffer, &index, 10, FALSE ) ;
                break ;
            case 'c' : // character cased
                char c = (char) va_arg( args, int ) ;
                set_char( c, buffer, &index ) ;
                break ;
            case 's' : // string case
                char* s = va_arg( args, char *) ;
                while ( *s++ )
                {
                	set_char(*s, buffer, &index ) ;
                    if ( index >= 31 )
                    {
                        buffer[index] = '\0' ;
                        write( buffer , index ) ;
                        index = 0 ;
                    }
                }
                break ;
            case '%' : // % case
                set_char( '%', buffer, &index ) ;
                break ;
            // Canned because I'm not entirely sure how
            // I want to implement it at this moment.
            // If needed I would gladly write it though
            // case 'f' : // float case
            //     float f = va_arg( args, float ) ;
            //     ftoa() ;
            case 'x' : // hexadecimal case
                // put implementation here :)
                break;
            default :
            	set_char(*(--format), buffer, &index ) ;
                break;
            }
        }
        else if ( *format == '\n' ) // ESCAPE
        {
        	set_char(0x0a, buffer, &index ) ; // LINE FEED
        	set_char(0x0d, buffer, &index ) ; // CARRIAGE RETURN
        }
        else // REGULAR
        {
            set_char(*format, buffer, &index ) ;
        }
        buffer[index] = '\0' ;
        write( buffer, index ) ;
        index = 0 ;
        LED_Toggle() ;
        USART_Delay( 150000 ) ;
        ++format;
    }
    va_end( args ) ;
    LED_Off() ;

    return 1 ;
}
