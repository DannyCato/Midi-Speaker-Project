/*
 * endianConverter.c 
 * Switches byte endianness
 *
 * Created by: dgc5878
 * 
 * */

#include <stdint.h>

#include <endianConverter.h>

uint8_t reverse_byte( uint8_t rev_me )
{
    uint8_t ret = 0 ;
    for ( int i = 0 ; i < 7 ; i++ )
    {
        if ( rev_me & 1 ) // if the lowest bit is set
        {
            ret += 1 ; // set the lsb to 1
        }
        ret = ret << 1 ; // shift ret to left to open a new value
        rev_me = rev_me >> 1 ; // shift rev right to switch to next value
    }
    return ret ;
}

uint64_t general_converter( uint8_t* p_values, uint8_t length )
{
    uint64_t ret = 0 ; 
    for ( int i = 0; i < length; i++ )
    {

    }
    return 0 ;
}

uint16_t convert_to_uint16 (uint8_t* p_value)
{
    return ( reverse_byte(p_value[1]) << 8 ) + reverse_byte(p_value[0]) ;
}

uint32_t convert_to_uint32 (uint8_t* p_value)
{
	return 0 ;
}
