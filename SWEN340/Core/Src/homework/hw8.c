// #include "hw8.h"
// #include "next.h"
// #include <stdint.h>
// #include "song.h"

// uint32_t parse_delay( uint8_t* delay ) 
// {
// 	uint8_t i = 0 ;
// 	uint32_t val = 0 ;
// 	do
// 	{
// 		if ( val != 0 )
// 		{
// 			val <<= 7 ;
// 		}
// 		val |= *delay & ~( 1 << 7 ) ;
// 	} 
// 	while ( *delay++ & ( 1 << 7 ) && i++ < 3 ) ;
// 	return val ;
// }
