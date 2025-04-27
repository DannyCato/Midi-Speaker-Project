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

void music_resume() ;

uint32_t get_start_time() ;

void next_song() ;

void set_offset_time() ;

#endif // AUDIO_ENGINE