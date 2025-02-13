

#include <stdint.h>

#include "song.h"
#include "printf.h"
#include "endianConverter.h"

struct midi_header
{
	char type[4] ;
	uint32_t length ;
	uint16_t format ;
	uint16_t numTrks ;
	uint16_t division ;
}  ;

void array_setter( uint8_t* arr, uint8_t size, uint32_t number )
{
	for ( int i = size - 1 ; i >= 0 ; i-- )
	{
		arr[i] = (uint8_t) number % ( 1 << 8 ) ;
		number = number / ( 1 << 8 ) ;
	}
}

void activity4()
{
	song song = get_song( 0 ) ;
	struct midi_header* md;
	md = ( struct midi_header* ) song.p_song ;

//	md->type = "MThd" ;
//	md->length = 0x0006 ;
//	md->format = 0x01 ;
//	md->numTrks = 0x02 ;
//	md->division = 0x80 ;


	uint8_t lens[4] = {0,0,0,0} ;
	array_setter( lens, 4, md->length ) ;

	uint8_t form[2] = {0,0} ;
	array_setter( form, 2, md->format ) ;

	uint8_t nt[2] = {0,0} ;
	array_setter( nt, 2, md->numTrks ) ;

	uint8_t div[2] = {0,0} ;
	array_setter( div, 2, md->division ) ;

	printf("Midi Header\n\r") ;
	printf( "Type = %c%c%c%c\n\r", md->type[0], md->type[1], md->type[2], md->type[3] ) ;
	printf("length = %ld\n\r", convert_to_uint32( lens ) ) ;
	printf("format = %d\n\r", convert_to_uint16( form ) ) ;
	printf("numTrks = %d\n\r", convert_to_uint16( nt ) ) ;
	printf("division = %d\n\r", convert_to_uint16( div ) ) ;
}
