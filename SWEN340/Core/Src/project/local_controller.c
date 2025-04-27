#include <stdint.h>
#include "local_controller.h"
#include "printf.h"
#include "stm32l476xx.h"
#include "stm32l4xx.h"
#include "GPIO.h"
#include "systick.h"
#include "blink.h"
#include "LED.h"

#include "play.h"
#include "next.h"
#include "stop.h"
#include "pause.h"

// value for the local mode
uint8_t local = 0 ;

typedef struct Button {
	uint8_t press_event_flag ; // bool // Meant to enable the handler if the button was pressed
	uint32_t last_edge ; // time
	uint32_t last_fall ; // time
	uint32_t last_last_fall ; // time
	uint32_t last_rise ; // time
	uint8_t is_pressed ; // bool
	uint32_t double_click_timeout ; // ms
	uint32_t double_click_timeout_cooldown ; // ms
	uint32_t hold_timeout ; // ms
	uint8_t z_bounce ; // ms
	uint8_t z_bounce_is_active ; // bool
	uint32_t z_bounce_cooldown ; // time
} Button ;

// inst of the breadboard button
Button breadboard_button ; 

// init struct to handle all operations of the breadboard button
void init_press_struct( Button* p ) { 
	p->press_event_flag = 0 ;
	p->last_edge = 0 ; 
	p->last_fall = 0 ;
	p->last_rise = 0xffffffff ; // set this to not zero so that the first press does not register as a double click
	p->is_pressed = 0 ;
	p->double_click_timeout = 250000 ; // ms
	p->hold_timeout = 500000 ; // ms
	p->z_bounce = 12 ; // ms
	p->z_bounce_is_active = 0 ;
	p->z_bounce_cooldown = 0xffffffff ;
}

/**
 * Prepare the local controls for operation
 */
void initialize_local()
{
	GPIO_Init() ; // Start GPIO ports
	init_press_struct( &breadboard_button ) ;
}

/**
 * Handler for B1 Button on board. On press, switch local on and off
 */
void EXTI15_10_IRQHandler()
{
	HAL_GPIO_EXTI_IRQHandler( B1_Pin ) ;
}

/**
 * Handler for button on the breadboard. On press set the associated Button struct as needed 
 */
void EXTI9_5_IRQHandler() 
{
	HAL_GPIO_EXTI_IRQHandler( S1_Pin ) ;
}

/**
 * The callback for the interrupts that handles S1 and B1 
 */
void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{
	if ( GPIO_Pin == S1_Pin && local ) // only triggers S1 if local
	{
		printf("pressed") ;
		uint32_t time_now = get_clock() ;
		// activates only if not z_bounce is not active, if z_bounce is active but the timer expired, or the last event was the button being pushed down 
		if ( !( breadboard_button.z_bounce_is_active ) || ( breadboard_button.z_bounce_is_active && breadboard_button.z_bounce_cooldown < time_now ) || ( breadboard_button.last_edge == breadboard_button.last_fall ) ) // if ( not active ) or ( active and timer_expired ) 
		{
			if ( ! ( breadboard_button.is_pressed ) )
			{ // if not pressed
				printf("down") ;
				breadboard_button.last_last_fall = breadboard_button.last_fall ;
				breadboard_button.last_fall = time_now ;
				breadboard_button.last_edge = time_now ;
				breadboard_button.is_pressed = 1 ;
				breadboard_button.press_event_flag = 1 ;
				breadboard_button.double_click_timeout_cooldown = time_now + breadboard_button.double_click_timeout ;
			}
			else
			{ // if is pressed
				printf("up") ;
				breadboard_button.last_rise = time_now ;
				breadboard_button.last_edge = time_now ;
				breadboard_button.is_pressed = 0 ;
			}
			breadboard_button.z_bounce_is_active = 1 ;
			breadboard_button.z_bounce_cooldown = time_now + breadboard_button.z_bounce;
		}
	}

	// branch for B1
	if ( GPIO_Pin == B1_Pin )
	{
		local_toggle() ; // switches toggle on or off
		if ( local )
		{ // if switched to being local
			printf( "***MANUAL OVERRIDE MODE ACTIVE***\n" ) ;
			GPIOA->MODER |= 1 << 9 ;
		} 
		else
		{ // switched to being remote
			printf( "***REMOTE MODE ACTIVE***\n" ) ;
			GPIOA->MODER &= ~( 1 << 9 ) ;
		} 
		blink_off() ;
		LED_Off() ;
	}
		
}

/**
 * Gets if local is active or not
 */
uint8_t get_local_status()
{
	return local ;
}

/**
 * toggles local
 */
void local_toggle()
{
	local ^= 1 ;
}

/**
 * Set local on
 */
void local_on()
{
	local |= 1 ;
}

/**
 * Set local off
 */
void local_off()
{
	local &= 0 ;
}

/**
 * Clear the press_event_flag of Button
 */
void clear_press_event_flag()
{
	breadboard_button.press_event_flag = 0 ;
}

// 0 = stop, 1 = play, 2 = pause 
uint8_t state = 0 ;

typedef void ( *state_func_t )();
state_func_t state_method = NULL ; // the method to be run after the timeout
state_func_t last_method = NULL ; // saved method to be running the same method over and over (important for next)
uint8_t run_next = 0 ; // additional logic to allow state to run multiple times in a row

/**
 * called by local handler whenever a state change of the S1 button occurs
 */
void press_event_handler()
{
	// this is the hold down. checks if the button is pressed and that it has been pushed down for long enough
	if ( breadboard_button.is_pressed && breadboard_button.last_fall + breadboard_button.hold_timeout < get_clock() )
	{
		clear_press_event_flag() ;
		state = 0 ;
		state_method = stop ;
		
	} // This is the double click. This fires on the second time that the button is clicked within the double click timeout and checks to see if some additional conditions are true
	else if ( breadboard_button.is_pressed && breadboard_button.last_edge == breadboard_button.last_fall && breadboard_button.last_fall - breadboard_button.last_last_fall <= breadboard_button.double_click_timeout )
	{
		clear_press_event_flag() ;
		run_next = 1 ;
		state_method = next ;
	} // This is one of the single clicks. checks to see if the LED is in the right state and if the button has been released
	else if ( ( state == 0 || state == 2 ) && breadboard_button.last_rise == breadboard_button.last_edge ) // only blinks when in that pause state
	{
		clear_press_event_flag() ;
		state = 1 ;
		state_method = play ;
	} // Also a single click checks if the button is in the right state and that it has been released
	else if ( state == 1 && breadboard_button.last_rise == breadboard_button.last_edge )
	{
		state = 2 ;
		clear_press_event_flag() ;
		state_method = pause ;
	}
	printf(" STATE = %i  ", state) ;
}

/**
 * Code that orchestrates the local state
 */
void local_handler()
{
	uint32_t time = get_clock() ;
	// this is the code that stops the button from bouncing (BONUS)
	if ( breadboard_button.z_bounce_cooldown < time && breadboard_button.z_bounce_is_active )
	{
		printf("off") ;
		breadboard_button.z_bounce_is_active = 0 ;
	} // This is a flag that is set by the interrupt handler to tell this that there is an update to the buttons current state
	if ( breadboard_button.press_event_flag )
	{
		printf("active") ;
		press_event_handler() ;
	} // this activates the state method after the double_click cooldown has completed and then checks to make sure that the state is not being run twice with an exception for next()
	if ( breadboard_button.double_click_timeout_cooldown < time && ( last_method != state_method || run_next) )
	{
		printf("timed out") ;
		run_next = 0 ;
		last_method = state_method ;
		state_method() ;
	}
//	 printf(".") ;
	
}
