/*----------------------------------------------------------------------------
 Button C file
 *----------------------------------------------------------------------------*/
#include "buttons.h"
#include "stm32f4xx.h"

void init_button(void) {
	//Enable IO port C 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	
	//Set pins to input mode
	GPIOC->MODER &= ~GPIO_MODER_MODER13_0 | ~GPIO_MODER_MODER13_1 ;
	
	//Set pins to pull-up mode
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR13_0;
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR13_1;
	
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
