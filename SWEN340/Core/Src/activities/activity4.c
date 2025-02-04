

#include <stdint.h>

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
	struct midi_header* md;
	md->type = "MThd" ;
	md->length = 0x0006 ;
	md->format = 0x01 ;
	md->numTrks = 0x02 ;
	md->division = 0x0 ;
}
