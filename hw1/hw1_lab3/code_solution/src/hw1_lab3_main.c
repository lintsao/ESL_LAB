/*----------------------------------------------------------------------------
LAB EXERCISE - Low Level Interrupt Handling
 ----------------------------------------
	In this exercise you will have to write a program which reads the state of
	the on board button and turns on the on board LED.
	
	GOOD LUCK!
 *----------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "leds.h"
#include "buttons.h"
#include "interrupts.h"

/*----------------------------------------------------------------------------
  Interrupt handler function
 *----------------------------------------------------------------------------*/

void EXTI15_10_IRQHandler(void){
	//Clear pending interrupts
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn); 
	//Check that there is a pending interrupt for EXTI_13
	if(EXTI->PR & EXTI_PR_PR13) {
	//Toggle the LED
	toggle();
	}
	//Clear the EXTI pending register
	EXTI->PR |= (EXTI_PR_PR13);
}

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main() {
	// Initialise LEDs and buttons
	init_led();
	init_button();
	init_interrupts();
	
	while(1){
	__wfi();	//Wait for interrupts
	}
}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************
