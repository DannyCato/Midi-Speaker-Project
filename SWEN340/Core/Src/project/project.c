/*
 * project.c
 *
 *  Created on: Jan 8, 2023
 *      Author: bruce
 */

#include "project.h"
#include "printf.h"
#include "endianConverter.h"

void run_project()
{
	printf( "\nTest 16-bit \n" ) ;
	uint8_t bytes16[2] = { 0x01, 0x02 } ;
	uint16_t reverse16 = convert_to_uint16( bytes16 ) ;
	printf( " start = %x \n answer = %x \n", (uint16_t) (bytes16[0] << 8) + bytes16[1], reverse16 ) ;
	printf( "\nTest 32-bit\n" ) ;
	uint8_t bytes32[4] = { 0x01, 0x02, 0x03, 0x04 } ;
	uint32_t reverse32 = convert_to_uint32( bytes32 ) ;
	printf( " start = %lx \n answer = %lx \n", (uint32_t) (bytes32[0] << 24) + (bytes32[1] << 16) + (bytes32[2] << 8) + bytes32[3], reverse32 ) ;
}
