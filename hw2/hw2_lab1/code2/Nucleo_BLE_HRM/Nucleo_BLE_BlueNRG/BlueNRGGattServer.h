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

#ifndef __BLUENRG_GATT_SERVER_H__
#define __BLUENRG_GATT_SERVER_H__

#include "mbed.h"
#include "blecommon.h"
#include "btle.h"
#include "GattService.h"
#include "public/GattServer.h"
#include <vector>
#include <map>

#define BLE_TOTAL_CHARACTERISTICS 10


using namespace std;

class BlueNRGGattServer : public GattServer
{
public:
    static BlueNRGGattServer &getInstance() {
        static BlueNRGGattServer m_instance;
        return m_instance;
    }
    
    /* Functions that must be implemented from GattServer */
    virtual ble_error_t addService(GattService &);
    virtual ble_error_t readValue(uint16_t handle, uint8_t buffer[], uint16_t *const lengthP);
    virtual ble_error_t updateValue(uint16_t, uint8_t[], uint16_t, bool localOnly = false);

    /* BlueNRG Functions */
    void eventCallback(void);
    //void hwCallback(void *pckt);
    ble_error_t Read_Request_CB(tHalUint16 handle);
    GattCharacteristic* getCharacteristicFromHandle(tHalUint16 charHandle);
    
private:
    static const int MAX_SERVICE_COUNT = 10;
    uint8_t serviceCount;
    uint8_t characteristicCount;
    tHalUint16 servHandle, charHandle;

    std::map<tHalUint16, tHalUint16> bleCharHanldeMap;  // 1st argument is characteristic, 2nd argument is service
    GattCharacteristic *p_characteristics[BLE_TOTAL_CHARACTERISTICS];
    tHalUint16 bleCharacteristicHandles[BLE_TOTAL_CHARACTERISTICS];

    BlueNRGGattServer() {
        serviceCount = 0;
        characteristicCount = 0;
    };

    BlueNRGGattServer(BlueNRGGattServer const &);
    void operator=(BlueNRGGattServer const &);
    
    static const int CHAR_DESC_TYPE_16_BIT=0x01; 
    static const int CHAR_DESC_TYPE_128_BIT=0x02;    
    static const int CHAR_DESC_SECURITY_PERMISSION=0x00;
    static const int CHAR_DESC_ACCESS_PERMISSION=0x03;  
    static const int CHAR_ATTRIBUTE_LEN_IS_FIXED=0x00;        
};

#endif
