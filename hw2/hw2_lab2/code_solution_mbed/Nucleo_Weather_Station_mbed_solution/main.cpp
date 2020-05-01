/*----------------------------------------------------------------------------
LAB EXERCISE - Weather Station Appcessory
 ----------------------------------------
	In this exercise we will create a BLE weather station appcessory that will measure several environmental parameters and
	send them to our smartphone.
 *----------------------------------------------------------------------------*/
 
#include "mbed.h"
#include "x_cube_mems.h"
#include "BLEDevice.h"
#include "EnvironmentalSensingService.h"
#include "EnvironmentalSensingService2.h"
#include "DeviceInformationService.h"
#include "Utils.h"/* To turn on/off the debug messages on the console edit this file; NEED_CONSOLE_OUTPUT 1/0( it will have an impact on code-size and power consumption.) */
#define PI 3.1415169f
BLEDevice  ble;
DigitalOut led(LED1);
Ticker update;
Ticker aliveness;

float TEMPERATURE_C = 20;
float HUMIDITY = 50;
float PRESSURE = 1000;
float WIND_DIRECTION = 0;
int16_t MagRaw[3];
AxesRaw_TypeDef MAGNETIC;

const static char     DEVICE_NAME[]        = "WEATHER";
static const uint16_t uuid16_list[]        = {GattService::UUID_ENVIRONMENTAL_SENSING_SERVICE};

static volatile bool  triggerSensorPolling = false;

void disconnectionCallback(Gap::Handle_t handle, Gap::DisconnectionReason_t reason)
{
    DEBUG("Disconnected!\n\r");
    DEBUG("Restarting the advertising process\n\r");
    ble.startAdvertising(); // restart advertising
}

void connectionCallback(Gap::Handle_t handle, const Gap::ConnectionParams_t *reason)
{    DEBUG("Connected\r\n");}

void update_handler(void)
{    triggerSensorPolling = true;}

void aliveness_handler (void)
{	led = !led;	}

int main(void)
{
	  /* Create a sensor shield object */
    static X_CUBE_MEMS *Sensors = X_CUBE_MEMS::Instance();
	
    update.attach(update_handler, 2);
	  aliveness.attach(aliveness_handler,0.5);

    DEBUG("Initialising \n\r");
    ble.init();
    ble.onDisconnection(disconnectionCallback);
    ble.onConnection(connectionCallback);

	  /* Setup primary services. */
	  EnvironmentalSensingService air (ble, (uint16_t) HUMIDITY, (int16_t) TEMPERATURE_C );
	  EnvironmentalSensingService2 wind (ble, (uint16_t) WIND_DIRECTION, (uint32_t) PRESSURE);
    /* Setup auxiliary services. */
    DeviceInformationService deviceInfo(ble, "ST", "Nucleo", "SN1" );

    /* Setup advertising. */
    ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
	  ble.accumulateAdvertisingPayload(GapAdvertisingData::GENERIC_THERMOMETER);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.setAdvertisingInterval(1600); /* 1000ms; in multiples of 0.625ms. */
    ble.startAdvertising();

    while (true)
    {
        if (triggerSensorPolling & ble.getGapState().connected)
        {
					/* Read the environmental sensors  */
					Sensors->hts221.GetTemperature((float *)&TEMPERATURE_C);
					Sensors->hts221.GetHumidity((float *)&HUMIDITY);
					Sensors->lps25h.GetPressure((float *)&PRESSURE);
					Sensors->lis3mdl.GetAxes((AxesRaw_TypeDef *)&MAGNETIC);
					
					TEMPERATURE_C = TEMPERATURE_C*100;  //2 decimals
					HUMIDITY = HUMIDITY*100;						//2 decimals
					PRESSURE = PRESSURE*1000;           //hPa to Pa + 1 decimal
					
					
					//Calcule the direction where the system is pointing relative to North.
					//I have used a simple empirical method to distinguish between 8 directions. 
					if (MAGNETIC.AXIS_X < 140) WIND_DIRECTION = 0; //North
					else if (MAGNETIC.AXIS_X >= 140 && MAGNETIC.AXIS_X < 200 && -MAGNETIC.AXIS_Y > 250 ) WIND_DIRECTION = 45;  //Northeast
					else if (MAGNETIC.AXIS_X >= 140 && MAGNETIC.AXIS_X < 200 && -MAGNETIC.AXIS_Y < 250 ) WIND_DIRECTION = 315; //Northwest
					else if (MAGNETIC.AXIS_X >= 200 && MAGNETIC.AXIS_X < 280 && -MAGNETIC.AXIS_Y > 250 ) WIND_DIRECTION = 90;  //East
					else if (MAGNETIC.AXIS_X >= 200 && MAGNETIC.AXIS_X < 280 && -MAGNETIC.AXIS_Y < 250 ) WIND_DIRECTION = 270; //Weast
					else if (MAGNETIC.AXIS_X >= 280 && MAGNETIC.AXIS_X < 380 && -MAGNETIC.AXIS_Y > 250 ) WIND_DIRECTION = 135; //Southeast
					else if (MAGNETIC.AXIS_X >= 280 && MAGNETIC.AXIS_X < 380 && -MAGNETIC.AXIS_Y < 250 ) WIND_DIRECTION = 225; //Soutwest			
					else if (MAGNETIC.AXIS_X >= 380) WIND_DIRECTION = 180; //South
					
          WIND_DIRECTION *=100; 							//2 decimals
					
					air.updateTemperature(TEMPERATURE_C);	
					air.updateHumidity(HUMIDITY);
					wind.updateWinddirection(WIND_DIRECTION);
					wind.updatePressure(PRESSURE);
					
					triggerSensorPolling = false;
        } 
        else
        {
            ble.waitForEvent();
        }
    }
}

// *******************************ARM University Program Copyright © ARM Ltd 2015*************************************//

