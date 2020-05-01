/*----------------------------------------------------------------------------
LAB EXERCISE - Simple LED blinky
 ----------------------------------------
In this exercise we will create a program that blinks a LED while the user button is being pressed.

	GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include "mbed.h"

// Create a DigitalOut objects for the LED
DigitalOut LED(LED1);

// Create a DigitalIn objects for the button. 
DigitalIn  BUTTON (USER_BUTTON);


/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/
int main() {
int a = 0;
int count = 0;
	while(1) {
		// The buttons is active low
    // If the button is pressed the LED blinks twice per second
		if(!BUTTON&&count%2==0){
			a=1;
			count+=1;
		}else if(!BUTTON&&count%2==1){
			a=0;
			count+=1;
		}else if(BUTTON&&count%2==1){
			a=1;
		}else if(BUTTON&&count%2==0){
			a=0;
		}
		// Otherwise the LED is switch off.
		if(a==1){
					LED = !LED;
		}else{
			LED=0;
		}
		wait(0.3);
  }
}
