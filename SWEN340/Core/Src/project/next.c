#include <string.h>
#include <math.h>

#include "next.h"
#include "printf.h"
#include "endianConverter.h"
#include "audio_engine.h"

uint8_t num_songs = 5 ;
uint8_t current_song = 0 ;

track itrk ;
track trk0, trk1, trk2 ;
structuredSong instance ; // I thought I could lay a pointer of this over the file, but it
// doesn't like that bc it expects the song to be filled with pointers so i needed an instance
structuredSong* uSong = &instance ;

float get_us_per_tpqn()
{
	return round( uSong->tpqn / uSong->division ) ;
}

///**
// * Debugging tool that takes a string and prints it to the terminal. Helpful for seeing if memory was corrupted
// *
// * Not deleted as I may have to use it again in the future
// */
//void print_raw(uint8_t* info, int size) {
//	for ( int i = 0 ; i < size ; i++ )
//	{
//		printf( "%02x ", *info++ ) ;
//	}
//	putchar('\n') ;
//}

uint32_t parse_normal_length( uint8_t* address, uint8_t bytes )
{
	uint32_t ret = 0 ;
	for ( uint8_t i = 0 ; i < bytes ; i++ )
	{
		ret <<= 8 ;
		ret |= *(address + i) ;
	}
	return ret ;
}

void init_mtrk( track* trk_n ) {
	trk_n->start = 0 ;
	trk_n->length = 0 ;
	trk_n->messages = 0 ;
	trk_n->active = 0 ;
}

uint8_t initialized = 0 ;

/**
 * takes a song struct and parses it to get the addresses some of the midi method headers and lengths
 *
 * @return void
 */
void format_usable_song( song s )
{
	initialized = 1 ;

	uSong->info_track = &itrk ;
	init_mtrk( &itrk ) ;
	uSong->trks[0] = &trk0 ;
	init_mtrk( &trk0 ) ;
	uSong->trks[1] = &trk1 ;
	init_mtrk( &trk1 ) ;
	uSong->trks[2] = &trk2 ;
	init_mtrk( &trk2 ) ;

	uSong->start = s.p_song ;
	uSong->format = ( uint16_t ) parse_normal_length( s.p_song + 8, 2 ) ;
	uSong->ntrks = ( uint16_t ) parse_normal_length( s.p_song + 10, 2 ) ;
	uSong->division = ( uint16_t ) parse_normal_length( s.p_song + 12, 2 ) ; 

	uint8_t i = 0 ;
	track* trkn ;
	uint8_t* next_trk_start = s.p_song + 14 ;
	do
	{
		if ( i == 0 )
		{
			trkn = uSong->info_track ;
			i++ ;
		}
		else
		{
			trkn = uSong->trks[i - 1] ;
			i++ ;
		}
		trkn->start = next_trk_start ;
		trkn->length = parse_normal_length( trkn->start + 4, 4 ) ;
		trkn->messages = trkn->start + 8 ;
		trkn->active = 1 ;
		next_trk_start = trkn->messages + trkn->length ;

		// printf("trk# = %d, trkn messages + length = %p, Start of Song = %p, calculated size = %X, value at calc size = %X. \n", i, next_trk_start, uSong->start, next_trk_start - uSong->start, s.p_song[next_trk_start - uSong->start] ) ;
		// printf("Size of Song = %X, value at size of song = %X\n", s.size, s.p_song[s.size] ) ;
	} 
	while ( ( next_trk_start - uSong->start ) < s.size && i < uSong->ntrks ) ;

}

typedef struct meta_info {
	uint8_t* name ;
	uint8_t  name_length ;
	uint8_t* copyright ;
	uint8_t  copyright_length ;
	uint32_t tpqn ;
	uint8_t  tpqn_length ;
} meta_info ;

meta_info song_info ;
uint8_t not_found[] = "No Information Found" ;

void format_necessary_meta_events()
{
	song_info.copyright = not_found ;
	song_info.copyright_length = 21 ;
	uint8_t* search_through = uSong->info_track->messages ;
	uint8_t length ;
	for ( int i = 0 ; i < uSong->info_track->length ; i++ )
	{
		if ( *search_through++ == 0xFF )
		{
			switch ( *search_through++ )
			{
			case 0x03 : // Title
				song_info.name_length = *search_through++ ; 
				song_info.name = search_through ;
				search_through += song_info.name_length ;
				i += song_info.name_length + 1 ;
				break ;
			case 0x02 : // Copyright
				song_info.copyright_length = *search_through++ ;
				song_info.copyright = search_through ;
				search_through += song_info.copyright_length ;
				i += song_info.copyright_length + 1 ;
				break ;
			case 0x51 : // Tempo
				song_info.tpqn_length = *search_through++ ;
				song_info.tpqn = 0 ;
				for ( uint8_t j = 0 ; j < 3 ; j++ )
				{
					song_info.tpqn <<= 8 ;
					song_info.tpqn |= *search_through++ ;
				}
				uSong->tpqn = song_info.tpqn ;
				i += song_info.tpqn_length + 1 ;
				break ;
			default:
				length = *search_through ;
				search_through += length ;
				i += length + 1 ;
				break;
			}
		}
	}
}

structuredSong* get_uSong()
{
	if ( !initialized)
	{
		format_usable_song( get_song( current_song ) ) ;
		format_necessary_meta_events() ;
	}
	return uSong ;
}

/**
 * General next() method to handle the next command from running.
 * This reads the song from get_song, parses it, and then reads and echoes the
 * title, copyright information, and tempo information to the terminal
 *
 * @return void
 */
void next()
{
	song s = get_song( current_song ) ; // get the song
//	print_raw( s.p_song, s.size ) ; // debugging statement that might prove helpful later
	format_usable_song( s ) ; // parse current song to get get useful general markers

	format_necessary_meta_events() ;
	printf("Title = ") ;
	printnf(song_info.name_length, "%s", song_info.name) ;
	printf("\nCopyright = ") ;
	printnf(song_info.copyright_length + 1, "%s", song_info.copyright) ;
	printf("\nTempo = ") ;
	printf("%ld\n", 60000000 / song_info.tpqn) ;

	current_song = (current_song + 1) % num_songs ; // increment current song
}
