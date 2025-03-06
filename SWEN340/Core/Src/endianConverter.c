/*
 * endianConverter.c 
 * Switches endianness
 *
 * Created by: dgc5878
 * 
 * */

#include "endianConverter.h"

uint64_t general_converter( uint8_t* p_values, uint8_t length )
{
    uint64_t ret = 0 ; 
    for ( int i = length - 1; i >= 0; i-- )
    {
    	ret = ret << 8 ;
    	ret += p_values[i] ;
    }
    return ret ;
}

uint16_t convert_to_uint16 (uint8_t* p_value)
{
    return (uint16_t) general_converter( p_value, 2 ) ;
}

uint32_t convert_to_uint32 (uint8_t* p_value)
{
	return (uint32_t) general_converter( p_value, 4 ) ;
}

uint32_t convert_to_24bit ( uint8_t* p_value )
{
	return (uint32_t) general_converter( p_value, 3 ) ;
}
