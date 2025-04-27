#ifndef PARSE_MIDI
#define PARSE_MIDI

#include "next.h"
#include <stdint.h>

// typedef struct channel {
//     uint8_t index ;

// } channel ;

typedef struct event {
    uint32_t delay ;
    uint32_t at_time ;
    uint8_t type ;
    uint8_t* info ;
    uint8_t info_length ;
    // uint8_t channel ; 
} event ;

void reset_parse() ;

void start_buffered_parse_song( structuredSong* s ) ;

void refresh_buffer() ;

event read_next_event() ;

uint32_t get_next_time() ;

#endif