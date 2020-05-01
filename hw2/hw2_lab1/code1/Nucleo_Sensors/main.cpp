/*----------------------------------------------------------------------------
LAB EXERCISE - Environmental sensors measurments 
 ----------------------------------------
	In this exercise we will read the environmental sensors on the Nucleo sensors shield (X-NUCLEO-IKS01A1) 
	with the help of the ST Nucleo Sensor Shield library which is compatible with the MBED API.
	Then we will send the measurements via USB to our PC using serial communication. 
	We can then display the results using a terminal emulation program (e.g. Termite).
	
	At the same time the program blinks the on board LED to show alivness. 
	

	GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include "mbed.h"
#include "x_cube_mems.h"

// Create a DigitalOut objects for the LED
DigitalOut led(LED1);

// Create a Serial objects to communicate via USB
Serial pc(USBTX, USBRX);

// Create 2 Ticker objects for a recurring interrupts. One for blink a LED and the other one to update the sensor lectures periodicly.
Ticker blinky; 
Ticker update;

volatile float TEMPERATURE_C;
volatile float TEMPERATURE_F;
volatile float TEMPERATURE_K;
volatile float HUMIDITY;
volatile float PRESSURE;

bool measurements_update = false;

void blinky_handler(){
	led = !led;
}

void sensors_handler(){
	measurements_update = true;
}

int main() {
    
    /* Create a sensor shield object */
    static X_CUBE_MEMS *Sensors = X_CUBE_MEMS::Instance();
    
	  /* Attach a function to be called by the Ticker objects at a specific interval in seconds*/
    blinky.attach(&blinky_handler, 0.5);
		update.attach(&sensors_handler, 3);
	
    while(1) {

			if(measurements_update == true){
        /* Read the environmental sensors  */
        Sensors->hts221.GetTemperature((float *)&TEMPERATURE_C);
        Sensors->hts221.GetHumidity((float *)&HUMIDITY);
        Sensors->lps25h.GetPressure((float *)&PRESSURE);
        
        TEMPERATURE_F = (TEMPERATURE_C * 1.8f) + 32.0f; //Convert the temperature from Celsius to Fahrenheit
				TEMPERATURE_K = (TEMPERATURE_C + 273.15f);			//Convert the temperature from Celsius to Kelvin
        pc.printf("Temperature:\t %.2f C / %.2f F / %.2f K\r\n", TEMPERATURE_C, TEMPERATURE_F, TEMPERATURE_K);
        pc.printf("Humidity:\t %.2f%%\r\n", HUMIDITY);
        pc.printf("Pressure:\t %.2f hPa\r\n", PRESSURE); 

        pc.printf("\r\n");
				
				measurements_update = false;
			}
		__wfi();
    }
}
// *******************************ARM University Program Copyright © ARM Ltd 2015*************************************   
