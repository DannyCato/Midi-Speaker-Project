#ifndef ENDIANCONVERTER
#define ENDIANCONVERTER

#include "stdint.h"

	uint16_t convert_to_uint16 (uint8_t* p_value);
	uint32_t convert_to_uint32 (uint8_t* p_value);
	uint32_t convert_to_24bit ( uint8_t* p_value ) ;

#endif
