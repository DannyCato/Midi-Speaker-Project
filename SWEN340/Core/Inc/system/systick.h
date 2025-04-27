/*
 * systick.h
 *
 *  Created on: Jan 8, 2023
 *      Author: bruce
 */

#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

#include <stdint.h>

#pragma once

#ifndef SYSTICK_REG
#define SYSTICK_REG

typedef struct SysTickRegister { // DEFINE A GLOBAL SYSTICK STRUCT
	uint32_t CTRL ;
	uint32_t LOAD ;
	uint32_t VAL ;
	uint32_t CALIB ;
} SysTickRegister ;

#define ST_MEM_ADDRESS 0xE000E010 // MEMORY ADDRESS OF THE SYSTICK REGISTERS

// CREATE A POINTER STRUCT SYSTICK
#define GLBL_SYSTICK ( ( volatile SysTickRegister* ) ST_MEM_ADDRESS ) // CREATE A GLOBAL SYSTICK STRUCT OVER THE MEMORY ADDRESS

#endif

// This function is to Initialize SysTick registers
void init_systick();

// This function is to create delay using SysTick timer counter
void delay_systick();

void toggle_systick_interrupt() ;

void reset_clock() ; 

uint32_t get_clock() ;

#endif /* INC_SYSTICK_H_ */
