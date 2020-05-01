/* ARM University Program Microcontroller Library
 * 
 * Environmental Sensing Service
 *
 * This software is distributed under an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#ifndef __BLE_ENVIRONMENTAL_SENSING_SERVICE2_H__
#define __BLE_ENVIRONMENTAL_SENSING_SERVICE2_H__

#include "BLEDevice.h"

/* Environmental Sensing Service */
/* Service:  https://developer.bluetooth.org/gatt/services/Pages/ServiceViewer.aspx?u=org.bluetooth.service.environmental_sensing.xml */
/* True wind direction characteristic: https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.true_wind_direction.xml*/
/* Pressure characteristic: https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.pressure.xml */


class EnvironmentalSensingService2 {

public:
    /**
     * Constructor.
     *
     * param[in] _ble
     *               Reference to the underlying BLEDevice.
		 * param[in] True Wind Direction in degrees (16-bit unsigned, 2 decimals). Wind coming from: 0=North, 90=East, 180=South and 270=West.
     *               initial value for the temperature
		 * param[in] pressure in Pascals (32-bit unsigned, 1 decimal).
		 * 							 initial value for the pressure value.
     */
    EnvironmentalSensingService2(BLEDevice &_ble, uint16_t winddirection, uint32_t pressure) :
        ble(_ble),


				
				winddirectionchar(GattCharacteristic::UUID_TRUE_WIND_DIRECTION_CHAR, (uint8_t *)&winddirection,
				sizeof(uint16_t), sizeof(uint16_t),
				GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
				
				pressurechar(GattCharacteristic::UUID_PRESSURE_CHAR, (uint8_t *)&pressure,
				sizeof(uint32_t), sizeof(uint32_t),
				GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY)
				
				{	// Setup Service
					

				GattCharacteristic *charTable[] = {  &winddirectionchar, &pressurechar };

				GattService      EnvironmentalService(GattService::UUID_ENVIRONMENTAL_SENSING_SERVICE, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));

        ble.addService(EnvironmentalService);
    }


		/* Set a new 16-bit value for the wind direction measurement.  */
    void updateWinddirection(uint16_t winddirection) {
        ble.updateCharacteristicValue(winddirectionchar.getValueAttribute().getHandle(), (uint8_t *)&winddirection, sizeof(uint16_t));
    }		
    /* Set a new 32-bit value for the pressure measurement.  */
    void updatePressure(uint32_t pressure) {
        ble.updateCharacteristicValue(pressurechar.getValueAttribute().getHandle(), (uint8_t *)&pressure, sizeof(uint32_t));
    }

		
private:
    BLEDevice            		&ble;
    GattCharacteristic   		winddirectionchar;
    GattCharacteristic   		pressurechar;
};

#endif /* #ifndef __BLE_ENVIRONMENTAL_SENSING_SERVICE2_H__*/


// *******************************ARM University Program Copyright © ARM Ltd 2015*************************************//
