/* ARM University Program Microcontroller Library
 * 
 * Environmental Sensing Service
 *
 * This software is distributed under an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#ifndef __BLE_ENVIRONMENTAL_SENSING_SERVICE_H__
#define __BLE_ENVIRONMENTAL_SENSING_SERVICE_H__

#include "BLEDevice.h"

/* Environmental Sensing Service */
/* Service:  https://developer.bluetooth.org/gatt/services/Pages/ServiceViewer.aspx?u=org.bluetooth.service.environmental_sensing.xml */
/* Humidity characteristic: https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.humidity.xml */
/* Temperature: https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.temperature.xml */

class EnvironmentalSensingService {

public:
    /**
     * Constructor.
     *
     * param[in] _ble
     *               Reference to the underlying BLEDevice.
     * param[in] humidity percentage (16-bit unsigned, 2 decimals).
     *               initial value for the humidity value.
     * param[in] temperature in degrees Celsius (16-bit signed, 2 decimals).
     *               initial value for the temperature
     */
    EnvironmentalSensingService(BLEDevice &_ble, uint16_t humidity, int16_t temperature) :
        ble(_ble),

	 			humiditychar(GattCharacteristic::UUID_HUMIDITY_CHAR, (uint8_t *)&humidity,
				sizeof(uint16_t), sizeof(uint16_t),
				GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),

				temperaturechar(GattCharacteristic::UUID_TEMPERATURE_CHAR, (uint8_t *)&temperature,
				sizeof(int16_t), sizeof(int16_t),
				GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY)
				

				{	// Setup Service
					
        GattCharacteristic *charTable[] = {&humiditychar, &temperaturechar, }; // 2 services. humidity and temp

				GattService      EnvironmentalService(GattService::UUID_ENVIRONMENTAL_SENSING_SERVICE, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));

        ble.addService(EnvironmentalService);
    }


    /* Set a new 16-bit value for the humidity measurement.  */
    void updateHumidity(uint16_t humidity) {
        ble.updateCharacteristicValue(humiditychar.getValueAttribute().getHandle(), (uint8_t *)&humidity, sizeof(uint16_t));
    }

				 /* Set a new 16-bit value for the temperature measurement.  */
    void updateTemperature(int16_t temperature) {
        ble.updateCharacteristicValue(temperaturechar.getValueAttribute().getHandle(), (uint8_t *)&temperature, sizeof(int16_t));
    }


		
private:
    BLEDevice            		&ble;
    GattCharacteristic   		humiditychar;
    GattCharacteristic   		temperaturechar;

};

#endif /* #ifndef __BLE_ENVIRONMENTAL_SENSING_SERVICE_H__*/


// *******************************ARM University Program Copyright © ARM Ltd 2015*************************************//
