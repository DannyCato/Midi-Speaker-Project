#ifndef NEXT
#define NEXT

#include "song.h"

void next() ;

typedef struct {
	uint8_t* start ;
	uint8_t* info_track_start ;
	uint8_t* info_track_body ;
	uint8_t* general_body ;
	uint32_t info_track_length ;
} structuredSong ;

structuredSong* get_uSong() ;
void format_usable_song( song s ) ;

#endif
