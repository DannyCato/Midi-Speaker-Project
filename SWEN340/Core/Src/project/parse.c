#include "parse.h"
#include "tone.h"
#include "printf.h"
#include <stdio.h>

#define BF_Size 256
event buffer[BF_Size] ; // buffer of all tones
uint16_t parse_buffer_index[] = {0, 0, 0} ; // buffer for reading from trks
uint16_t wbuffer_index = 0 ; // index for writting to buffer. counts absolutely (needs to be modulo'd)
uint16_t rbuffer_index = 0 ; // index for reading from buffer. counts absolutely (needs to be modulo'd)
uint32_t trk_time[] = {0, 0, 0} ; // total sum of the delays of a track to log what track the next note should come from
uint8_t EOT[] = {0, 0, 0} ;
// uint32_t channels[16] ;

structuredSong* my_song ;

uint8_t my_song_is_null()
{
    return ( my_song == NULL ) ? 1 : 0 ;
}

void reset_event( event e )
{
	e.delay = 0 ;
    e.at_time = 0 ;
	e.type = 0 ;
	e.info = 0 ;
	e.info_length = 0 ;
	// e.channel = 0 ;
}

void reset_parse()
{
    my_song = get_uSong() ;
    wbuffer_index = 0 ;
    rbuffer_index = 0 ;
	for ( uint8_t i = 0 ; i < my_song->ntrks - 1 ; i++ )
	{
		parse_buffer_index[i] = 0 ;
		trk_time[i] = 0 ;
		EOT[i] = 0 ;
	}
    for ( uint16_t i = 0 ; i < BF_Size ; i++ )
    {
    	reset_event( buffer[i] ) ;
    }
}

uint8_t next_track_to_parse()
{
    return 0 ;
}

void parse_next_event( uint8_t* messages, uint8_t trk_num, uint32_t trk_length )
{
    uint16_t index = wbuffer_index % BF_Size ;
    restart_wo_incrementing:
    if ( trk_length < parse_buffer_index[trk_num] )
    {
        return ;
    }
    uint8_t* starting_addr = messages ;
    uint8_t i = 0 ;
    buffer[index].delay = 0 ;
    do
	{
		if ( buffer[index].delay != 0 )
		{
			buffer[index].delay <<= 7 ;
		}
		buffer[index].delay |= *messages & ~( 1 << 7 ) ;
	} 
	while ( *messages++ & ( 1 << 7 ) && i++ < 3 ) ;
    buffer[index].delay *= get_us_per_tpqn() ;

    if ( *messages == 0xFF )
    {
        uint8_t length = *(messages + 2) ;
        // printf("META EVENT type: %02X length %d, info = ", messages[1], length ) ;
        if ( *( messages + 1 ) == 0x2F )
        {
            buffer[index].type = 0x2F;
            EOT[trk_num] = 1 ;
            return ;
        }
        // for ( int i = 0 ; i < length ; i++ )
        // {
        //     printf("%02X ", messages[3 + i]) ;
        // }
        messages += length + 3 ;
        parse_buffer_index[trk_num] += messages - starting_addr ;
        // printf("\n%02X\n", *messages) ;
        goto restart_wo_incrementing ; // goto label for the vibes and faster for cpu
    }
    // printf("Delay: %d", buffer[index].delay) ;
    trk_time[trk_num] += buffer[index].delay ;
    buffer[index].at_time = trk_time[trk_num] ;
    buffer[index].type = ( *messages >> 4 ) ;
    // printf( ", Type: %02X", buffer[index].type ) ;
    // buffer[i].channel = *messages & 0b00001111 ;
    buffer[index].info = ++messages ;
    // printf( ", Info: %02X..%02X \n", buffer[index].info[0], buffer[index].info[1] ) ;
    
    uint8_t reset_flag = 1 ;
    if ( buffer[index].type == 8 || buffer[index].type == 9 )
    {
    	reset_flag = 0 ;
        wbuffer_index++ ;
        // printf("%d type = %d, note = %02X  \n", index, buffer[index].type, buffer[index].info[0] ) ;
    }

    if ( buffer[index].type == 0xC || buffer[index].type == 0xD )
    {
        messages += 1 ;
    }
    else
    {
        messages += 2 ;
        buffer[index].info_length = 2 ;
    }

    if ( reset_flag )
    {
    	reset_event(buffer[index]) ;
    }
    parse_buffer_index[trk_num] += messages - starting_addr ;
}

void start_buffered_parse_song( structuredSong* s ) 
{
    my_song = s ;
    track* trk0 = my_song->trks[0] ;
    for ( int i = 0 ; i < trk0->length ; i++ )
    {
        
    }
    while ( wbuffer_index - rbuffer_index < BF_Size && parse_buffer_index[0] < trk0->length && EOT[0] == 0 )
    {
        // printf(".") ;
        parse_next_event( &( trk0->messages[parse_buffer_index[0]] ), 0, trk0->length ) ;
    }
}

void refresh_buffer()
{
    if (wbuffer_index - rbuffer_index < BF_Size && my_song->trks[0]->length > parse_buffer_index[0] )
    {
        uint8_t trk_num = next_track_to_parse() ;
        parse_next_event( &( my_song->trks[trk_num]->messages[parse_buffer_index[trk_num]] ), trk_num, my_song->trks[trk_num]->length ) ;
    }
}

event read_next_event()
{
    return buffer[rbuffer_index++ % BF_Size] ;
}

uint32_t get_next_time()
{
    return buffer[rbuffer_index % BF_Size].at_time ;
}
