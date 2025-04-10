#include <string.h>
#include <stdio.h>

#include "next.h"
#include "printf.h"
#include "endianConverter.h"

uint8_t num_songs = 5 ;
uint8_t current_song = 0 ;

structuredSong instance ; // I thought I could lay a pointer of this over the file, but it
// doesn't like that bc it expects the song to be filled with pointers so i needed an instance
structuredSong* uSong = &instance ;

structuredSong* get_uSong()
{
	return uSong ;
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

/**
 * takes a song struct and parses it to get the addresses some of the midi method headers and lengths
 *
 * @return void
 */
void format_usable_song( song s )
{
	uSong->start = s.p_song ;
	uSong->info_track_start = s.p_song + 14 ;
	uSong->info_track_length = 0 ;
	for ( int i = 0 ; i < 4 ; i++ )
	{
		uSong->info_track_length <<= 8 ;
		uSong->info_track_length |= *(s.p_song + 18 + i) ;
	}
	uSong->info_track_body = s.p_song + 22 ;
	uSong->general_body = s.p_song + 22 + uSong->info_track_length ;
//	print_raw(uSong->start, s.size) ;
}

/**
 * Finds a meta event by its type and puts the return values into found and length
 *
 * @param type char. the meta event to be found
 * @param found char**. when meta event is found this is set to it
 * @param length int*. reads the byte after event to get information about the event
 *
 * @return void
 */
void find_event_type( char type, char** found, uint8_t* length )
{
	char* test_me = ( char* ) uSong->info_track_body ; // sets the byte that is compared
	char event_type[2] = {0xFF, type} ; // sets what the expected meta event is
	for ( uint8_t i = 0 ; i < uSong->info_track_length && i < 100 ; i++ ) // run through the length of the info track
	{
//		printf("%0x %02x \n", &test_me[0], test_me[0]) ; // prints out the address and first byte that is being compared to meta events // a debugging statement that may prove helpful
		if ( !( strncmp( event_type, test_me, 2 ) ) ) // if equal
		{
			*found = &test_me[3] ;
			*length = ( uint8_t )( test_me[2] ) ;
			break ;
		}
		test_me++ ;
	}
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

	// find and print title
	char* title = 0 ;
	uint8_t title_length = 0 ;
	find_event_type( 0x03, &title, &title_length ) ; // find and return information

	printnf( ( int ) ( title_length + 8 ), "Title = %s", title ) ; // print
	putchar('\n') ;

	// find and print copyright
	char* copy = 0 ;
	uint8_t copy_length = 0 ;
	find_event_type( 0x02, &copy, &copy_length ) ; // find and return information

	if (copy != NULL && copy_length > 0) { // if copyright info exists
		printnf( ( int ) copy_length + 24, "Copyright Information = %s", copy ) ; // print
	} else {
	    printf("No Information Found"); // handle otherwise
	}
	putchar('\n') ;

	// find, print and process tempo
	char* char_tempo = 0 ;
	uint8_t tempo_length = 0 ;
	find_event_type( 0x51, &char_tempo, &tempo_length ) ; // find and return information

	uint32_t final_tempo = 0 ;
	for ( int i = 0 ; i < tempo_length ; i++ ) // format final_tempo to be an integer
	{
		final_tempo <<= 8 ;
		final_tempo += char_tempo[i] ;
	}

	printf( "Tempo = %li", final_tempo ) ; // print
	putchar('\n') ;

	current_song = (current_song + 1) % num_songs ; // increment current song
}
