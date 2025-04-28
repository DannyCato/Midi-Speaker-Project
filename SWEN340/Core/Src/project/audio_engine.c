#include "audio_engine.h"
#include <stdint.h>
#include "parse.h"
#include "systick.h"
#include "tone.h"
#include "stop.h"
#include "dac.h"
#include "printf.h"
#include <math.h>

uint8_t on = 0 ;
uint32_t start_time = 0 ;
uint32_t offset_time = 0 ;
uint8_t speaker_up = 0 ;

uint8_t next_song_flag = 1 ;

#define SOUNDS 3
sound music_queue[SOUNDS] ;
uint8_t music_index = 0 ;

/**
 * sets the value of offset_time based on the current clock
 */
void set_offset_time()
{
    offset_time = get_clock() ;
}

/**
 * sets the next_song_flag to be 1
 */
void next_song()
{
    next_song_flag = 1 ;
}

/**
 * returns what time the song began at
 * 
 * @return uint32_t, starting time
 */
uint32_t get_start_time()
{
    return start_time ;
}

/**
 * return if the song should play
 * 
 * @return uint8_t
 */
uint8_t music_status()
{
    return on ;
}

/**
 * starts the music
 */
void music_resume() 
{
    on = 1 ;
}

/**
 * resumes and does a couple extra functions. meant to be called on the instantiation of a new song
 */
void music_on()
{    
    start_time = get_clock() ;
    music_resume() ;
    do_music() ;
}

/**
 * stops the music
 */
void music_off()
{
    on = 0 ;
}

/**
 * resets everything. clears all buffers and values associated with the song
 */
void music_reset()
{
    start_time = 0 ;
    offset_time = 0 ;
    start_buffered_parse_song( get_uSong() ) ;
    music_off() ;
    reset_parse() ;
}

/**
 * clears a value of the music_queue
 */
void reset_sound_struct(int i)
{
    music_queue[i].note = 0 ;
    music_queue[i].half_cycle = 0 ;
    music_queue[i].velocity = 0 ;
}


void init_music()
{
    if ( !next_song_flag )
    {
        if ( offset_time != 0 )
        {
            offset_time = get_clock() ;
        } 
        return ;
    }
    next_song_flag = 0 ;
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
    music_queue[music_index].velocity = ( vel < 3500 ) ? 3500 : ( ( vel > 4096 ) ? 4096 : vel ) ;
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
            continue ;
        }
        if ( found )
        {
        music_queue[i - 1].velocity = music_queue[i].velocity ;
        music_queue[i - 1].note = music_queue[i].note ;
        music_queue[i - 1].half_cycle = music_queue[i].half_cycle ;
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
    
    if ( get_next_time() > clk - start_time + offset_time)
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
        stop() ;
        break ; 
    default:
        break;
    }
}
