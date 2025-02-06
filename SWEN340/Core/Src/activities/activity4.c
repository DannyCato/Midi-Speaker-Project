

#include <stdint.h>
#include <song.h>
#include <printf.h>

struct midi_header
{
	char type[4] ;
	uint32_t length ;
	uint16_t format ;
	uint16_t numTrks ;
	uint16_t division ;
}  ;

void activity4()
{
	song song = get_song( 0 ) ;
	struct midi_header* md;
	md = ( struct midi_header* ) song.p_song ;

//	md->type = "MThd" ;
//	md->length = 0x0006 ;
//	md->format = 0x01 ;
//	md->numTrks = 0x02 ;
//	md->division = 0x0 ;


	printf("Midi Header\n\r") ;
	printf( "Type = %c%c%c%c\n\r", md->type[0], md->type[1], md->type[2], md->type[3] ) ;
	printf("length = %d\n\r", md->length) ;
	printf("format = %d\n\r", md->format) ;
	printf("numTrks = %d\n\r", md->numTrks) ;
	printf("division = %d\n\r", md->division) ;
}
