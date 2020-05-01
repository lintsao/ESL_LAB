/* mbed Microcontroller Library
* Copyright (c) 2006-2013 ARM Limited
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "mbed.h"
#include "BlueNRGDevice.h"
#include "BlueNRGGap.h"
#include "BlueNRGGattServer.h"

#include "btle.h"
#include "Utils.h"
#include "osal.h"

/**
* The singleton which represents the BlueNRG transport for the BLEDevice.
*/
static BlueNRGDevice deviceInstance;

/**
* BLE-API requires an implementation of the following function in order to
* obtain its transport handle.
*/
BLEDeviceInstanceBase *
createBLEDeviceInstance(void)
{
    return (&deviceInstance);
}

/**************************************************************************/
/*!
    @brief  Constructor
*/
/**************************************************************************/
BlueNRGDevice::BlueNRGDevice(void)
{
    isInitialized = false;
}

/**************************************************************************/
/*!
    @brief  Destructor
*/
/**************************************************************************/
BlueNRGDevice::~BlueNRGDevice(void)
{
}

/**************************************************************************/
/*!
    @brief  Initialises anything required to start using BLE

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t BlueNRGDevice::init(void)
{
    /* ToDo: Clear memory contents, reset the SD, etc. */
    btle_init(BlueNRGGap::getInstance().getIsSetAddress());

    isInitialized = true;
    
    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Resets the BLE HW, removing any existing services and
            characteristics

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t BlueNRGDevice::reset(void)
{
    wait(0.5);

    /* Reset BlueNRG SPI interface */
    BlueNRG_RST();

    /* Wait for the radio to come back up */
    wait(1);
    
    isInitialized = false;

    return BLE_ERROR_NONE;
}

void BlueNRGDevice::waitForEvent(void)
{
    HCI_Process();//Send App Events??
    
}


/**************************************************************************/
/*!
    @brief  get GAP version
    
    @returns    char *      

    @retval    pointer to version string

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
const char *BlueNRGDevice::getVersion(void)
{
    char *version = new char[6];
    memcpy((void *)version, "1.0.0", 5);
    return version;
}

/**************************************************************************/
/*!
    @brief  get init state
    
    @returns    bool  

    @retval    

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
bool BlueNRGDevice::getIsInitialized(void)
{
    return isInitialized;
}

/**************************************************************************/
/*!
    @brief  get reference to GAP object
    
    @returns    Gap&      

    @retval    reference to gap object

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
Gap        &BlueNRGDevice::getGap()        
{
    return BlueNRGGap::getInstance();
}

/**************************************************************************/
/*!
    @brief  get reference to GATT server object
    
    @returns    GattServer&    

    @retval    reference to GATT server object

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
GattServer &BlueNRGDevice::getGattServer() 
{
    return BlueNRGGattServer::getInstance();
}

/**************************************************************************/
/*!
    @brief  set Tx power level
    
    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t BlueNRGDevice::setTxPower(int8_t txPower)
{
    int8_t enHighPower = 0;
    int8_t paLevel = 0;    
    int8_t dbmActuallySet = getHighPowerAndPALevelValue(txPower, enHighPower, paLevel);
    DEBUG("txPower=%d, dbmActuallySet=%d\n\r", txPower, dbmActuallySet);
    DEBUG("enHighPower=%d, paLevel=%d\n\r", enHighPower, paLevel);                    
    aci_hal_set_tx_power_level(enHighPower, paLevel);
    return BLE_ERROR_NONE;    
}