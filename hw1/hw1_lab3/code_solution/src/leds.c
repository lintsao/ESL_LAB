/*----------------------------------------------------------------------------
 LED C file
 *----------------------------------------------------------------------------*/
#include "leds.h"
#include "stm32f4xx.h"

void init_led(void){

	//Enable IO port A 
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  
	//Set the pin to output mode
	GPIOA->MODER |= GPIO_MODER_MODER5_0;
	GPIOA->MODER &= ~GPIO_MODER_MODER5_1;
	
	//Set the pin to push-pull output state
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5 ;
	
	//Set pins to pull-up mode
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5_0;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5_1;
	
	//Set pin to fast speed
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR5_0;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_1;
}

// Set output pin PA_5 to high
void led_on(void) {
	 GPIOA->BSRRL |= GPIO_BSRR_BS_5;
}

// Set output pin PA_5 to low
void led_off(void) {
   GPIOA->BSRRH |= GPIO_BSRR_BR_5;
}


//Toggle the LED state
void toggle(void){
	GPIOA->ODR ^= GPIO_ODR_ODR_5;
	}


// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
