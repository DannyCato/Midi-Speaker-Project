
#include <stdint.h>

// make a struct for SysTick
struct SysTick {
	uint32_t CSR ;
	uint32_t RVR ;
	uint32_t CVR ;
	uint32_t CALIB ;
} ;

void activity5()
{
	struct SysTick* st = ( struct SysTick* ) 0xE000E010 ;

	st->RVR = 7999999 ;
	st->CSR |= 0b00000101 ; // enable clock and use on-board processor

	uint32_t i = 0 ;
	uint8_t sec = 0 ;
	while ( 1 )
	{
		if ( st->CSR & ( 1 << 16 ) ) // Checks if 16th bit is set
		{
			sec++ ;
			if ( sec == 10 )
			{
				printf( "Good %d!\n\r", i++ ) ;
				sec = 0 ;
			}
		}
	}
}
