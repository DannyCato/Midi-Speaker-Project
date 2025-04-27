#ifndef AUDIO_ENGINE
#define AUDIO_ENGINE

#include <stdint.h>

typedef struct sound {
    uint8_t note ;
    uint8_t velocity ;
    uint16_t half_cycle ; // of the wave
} sound ;

void music_on() ;

void music_off() ;

void music_reset() ;

void init_music() ;

void do_music() ;

uint8_t music_status() ;

#endif // AUDIO_ENGINE