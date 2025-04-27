#ifndef NEXT
#define NEXT

#include "song.h"

void next() ;

float get_us_per_tpqn() ;

typedef struct track {
	uint8_t active ;
	uint8_t* start ;
	uint32_t length ;
	uint8_t* messages ;
} track ;

typedef struct {
	uint8_t* start ;
	uint16_t format ;
	uint16_t ntrks ;
	uint16_t division ;
	uint32_t tpqn ;
	track* info_track ;
	track* trks[3] ;
	
} structuredSong ;


structuredSong* get_uSong() ;
void format_usable_song( song s ) ;

#endif
