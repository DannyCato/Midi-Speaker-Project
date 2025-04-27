#include "audio_engine.h"
#include <stdint.h>
#include "parse.h"
#include "systick.h"
#include "tone.h"
#include "dac.h"
#include "printf.h"
#include <math.h>

uint8_t on = 0 ;
uint32_t start_time = 0 ;
uint8_t speaker_up = 0 ;

#define SOUNDS 3
sound music_queue[SOUNDS] ;
uint8_t music_index = 0 ;

uint8_t music_status()
{
    return on ;
}

void music_on()
{

    start_buffered_parse_song( get_uSong() ) ;
    start_time = get_clock() ;
    on = 1 ;
    do_music() ;
}

void music_off()
{
    on = 0 ;
}

void music_reset()
{
    music_off() ;
    reset_parse() ;
}

void reset_sound_struct(int i)
{
    music_queue[i].note = 0 ;
    music_queue[i].half_cycle = 0 ;
    music_queue[i].velocity = 0 ;
}


void init_music()
{
    for ( int i = 0 ; i < SOUNDS ; i++ )
    {
        reset_sound_struct(i) ;
    }
    music_reset() ;
}


void push( event e )
{
    music_queue[music_index].note = e.info[0] ;
    uint32_t vel = e.info[1] * 16 ;
    music_queue[music_index].velocity = ( vel < 3000 ) ? 3000 : ( ( vel > 4096 ) ? 4096 : vel ) ;
    music_queue[music_index].half_cycle = ( hertz_to_midi_ticks( get_frequency( e.info[0] ) ) ) >> 1 ;
    music_index++ ;
}

void pop( event e )
{
    uint8_t note = e.info[0] ;
    uint8_t found = 0 ;
    for ( int i = 0 ; i < SOUNDS ; i++ )
    {
        if ( music_queue[i].note == note )
        {
            found = 1 ;
            reset_sound_struct( i ) ;
            music_index-- ;
            break ;
        }
        if ( found )
        {
            music_queue[i - 1].velocity = music_queue[i].velocity ;
            music_queue[i - 1].note = music_queue[i].note ;
            music_queue[i - 1].half_cycle = music_queue->half_cycle ;
        }
    }
}

void drive_music(uint32_t clk)
{
	// printf("###") ;
    for (uint8_t i = 0 ; i < SOUNDS ; i++)
    {
        if ( music_queue[i].velocity == 0 )
        {
            continue ;
        }
        if ( clk % ( music_queue[i].half_cycle << 1 ) > music_queue[i].half_cycle )
        {
            DAC_Set_Value( music_queue[i].velocity ) ;
        }
        else 
        {
            DAC_Set_Value( 0 ) ;
        }
    }
}

void do_music()
{
    // printf("_") ;
    uint32_t clk = get_clock() ;
    
    if ( music_queue[0].velocity != 0 )
    {
        drive_music(clk) ;
    }
    
    if ( get_next_time() > clk - start_time )
    {
        // printf("waiting until %d ticks have passed, ", get_next_time() ) ;
        return ;
    } 
    event e = read_next_event() ;
    
    switch ( e.type )
    {
    case 8 :
        // printf("removing note, ") ;
        pop( e ) ;
        break ;
    case 9 :
        // printf("play note %02X, ", e.info[0]) ;
        push( e ) ;
        break ; 
    case 0x2F :
        // printf("End of Song \n") ;
        music_off() ;
        break ; 
    default:
        break;
    }
}
