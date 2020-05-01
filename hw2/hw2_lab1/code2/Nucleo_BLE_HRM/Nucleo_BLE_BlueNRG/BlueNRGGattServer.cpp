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

#include "BlueNRGGattServer.h"
#include "mbed.h"
#include "BlueNRGGap.h"
#include "Utils.h"

/**************************************************************************/
/*!
    @brief  Adds a new service to the GATT table on the peripheral

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t BlueNRGGattServer::addService(GattService &service)
{
    /* ToDo: Make sure we don't overflow the array, etc. */
    /* ToDo: Make sure this service UUID doesn't already exist (?) */
    /* ToDo: Basic validation */
    
    tBleStatus ret;
    uint8_t type;
    uint16_t short_uuid;
    uint8_t primary_short_uuid[2];
    uint8_t primary_base_uuid[16];
    uint8_t char_base_uuid[16];
    const uint8_t *base_uuid;
    const uint8_t *base_char_uuid;
    
    type = (service.getUUID()).shortOrLong();
    DEBUG("AddService(): Type:%d\n\r", type);
    
    /* Add the service to the BlueNRG */
    short_uuid = (service.getUUID()).getShortUUID();
    STORE_LE_16(primary_short_uuid, short_uuid);
    
    if(type==UUID::UUID_TYPE_LONG) {
        base_uuid = (service.getUUID()).getBaseUUID();   
        
        COPY_UUID_128(primary_base_uuid, base_uuid[15],base_uuid[14],base_uuid[13],base_uuid[12],base_uuid[11],base_uuid[10],base_uuid[9],
        base_uuid[8],base_uuid[7],base_uuid[6],base_uuid[5],base_uuid[4],primary_short_uuid[1],primary_short_uuid[0],base_uuid[1],base_uuid[0]);
    }
    
    if(type==UUID::UUID_TYPE_SHORT) {
        ret = aci_gatt_add_serv(UUID_TYPE_16, primary_short_uuid, PRIMARY_SERVICE, 7, 
        &servHandle);
    }
    else if(type==UUID::UUID_TYPE_LONG) {
        ret = aci_gatt_add_serv(UUID_TYPE_128, primary_base_uuid, PRIMARY_SERVICE, 7, 
        &servHandle);
    }
    
    service.setHandle(servHandle);
    //serviceHandleVector.push_back(servHandle);
    DEBUG("added servHandle handle =%u\n\r", servHandle);
    tHalUint16 bleCharacteristic;
    
    //iterate to include all characteristics
    for (uint8_t i = 0; i < service.getCharacteristicCount(); i++) {
        GattCharacteristic *p_char = service.getCharacteristic(i);
        uint16_t char_uuid = (p_char->getValueAttribute().getUUID()).getShortUUID();   
        
        uint8_t int_8_uuid[2];
        STORE_LE_16(int_8_uuid, char_uuid);
        
        if(type==UUID::UUID_TYPE_LONG) {
            base_char_uuid = (p_char->getValueAttribute().getUUID()).getBaseUUID();
            
            COPY_UUID_128(char_base_uuid, base_char_uuid[15],base_char_uuid[14],base_char_uuid[13],base_char_uuid[12],base_char_uuid[11],base_char_uuid[10],base_char_uuid[9],
            base_char_uuid[8],base_char_uuid[7],base_char_uuid[6],base_char_uuid[5],base_char_uuid[4],int_8_uuid[1],int_8_uuid[0],base_char_uuid[1],base_char_uuid[0]);
        }
        
        DEBUG("Char Properties 0x%x\n\r", p_char->getProperties());
        /*
        * Gatt_Evt_Mask -> HardCoded (0)
        * Encryption_Key_Size -> Hardcoded (16)
        * isVariable (variable length value field) -> Hardcoded (1)
        */
        tGattServerEvent Gatt_Evt_Mask = 0x0;
        
        if((p_char->getProperties() &
                    (GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE|
                        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE))) {
            DEBUG("Setting up Gatt GATT_SERVER_ATTR_WRITE Mask\n\r");
            Gatt_Evt_Mask = Gatt_Evt_Mask | GATT_SERVER_ATTR_WRITE;
        }
        if((p_char->getProperties() &
                    (GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ|
                        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY| GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE))) {
            DEBUG("Setting up Gatt GATT_INTIMATE_APPL_WHEN_READ_N_WAIT Mask\n\r");
            Gatt_Evt_Mask = Gatt_Evt_Mask | GATT_INTIMATE_APPL_WHEN_READ_N_WAIT; 
        }    //This will support also GATT_SERVER_ATTR_READ_WRITE since it will be covered by previous if() check.
        
        if(type==UUID::UUID_TYPE_SHORT) {
            ret =  aci_gatt_add_char(service.getHandle(), UUID_TYPE_16, int_8_uuid, p_char->getValueAttribute().getMaxLength() /*2*/ /*Value Length*/,
            p_char->getProperties(), ATTR_PERMISSION_NONE, Gatt_Evt_Mask /*Gatt_Evt_Mask*/,
            16 /*Encryption_Key_Size*/, 1 /*isVariable*/, &bleCharacteristic);
        }
        else if(type==UUID::UUID_TYPE_LONG) {
            ret =  aci_gatt_add_char(service.getHandle(), UUID_TYPE_128, char_base_uuid, p_char->getValueAttribute().getMaxLength() /*2*/ /*Value Length*/,
            p_char->getProperties(), ATTR_PERMISSION_NONE, Gatt_Evt_Mask /*Gatt_Evt_Mask*/,
            16 /*Encryption_Key_Size*/, 1 /*isVariable*/, &bleCharacteristic);
        }
        /* Update the characteristic handle */
        uint16_t charHandle = characteristicCount;   
        
        bleCharHanldeMap.insert(std::pair<tHalUint16, tHalUint16>(bleCharacteristic, servHandle)); 
        
        p_characteristics[characteristicCount++] = p_char;
        p_char->getValueAttribute().setHandle(bleCharacteristic);    //Set the characteristic count as the corresponding char handle
        DEBUG("added bleCharacteristic handle =%u\n\r", bleCharacteristic);
        
        if ((p_char->getValueAttribute().getValuePtr() != NULL) && (p_char->getValueAttribute().getInitialLength() > 0)) {
            updateValue(p_char->getValueAttribute().getHandle(), p_char->getValueAttribute().getValuePtr(), p_char->getValueAttribute().getInitialLength(), false /* localOnly */);
        }
        
        // add descriptors now
        uint16_t descHandle = 0;
        for(uint8_t descIndex=0; descIndex<p_char->getDescriptorCount(); descIndex++) {
            GattAttribute *descriptor = p_char->getDescriptor(descIndex);
            uint16_t shortUUID = descriptor->getUUID().getShortUUID();
            const tHalUint8 uuidArray[] = {(shortUUID>>8)&0xFF, (shortUUID&0xFF)};
            ret = aci_gatt_add_char_desc(service.getHandle(), p_char->getValueAttribute().getHandle(), 
            CHAR_DESC_TYPE_16_BIT, uuidArray, descriptor->getMaxLength(), descriptor->getInitialLength(), 
            descriptor->getValuePtr(), CHAR_DESC_SECURITY_PERMISSION, CHAR_DESC_ACCESS_PERMISSION, GATT_SERVER_ATTR_READ_WRITE,
            MIN_ENCRY_KEY_SIZE, CHAR_ATTRIBUTE_LEN_IS_FIXED, &descHandle);
            if(ret==(tBleStatus)0) {
                DEBUG("Descriptor added successfully, descriptor handle=%d\n\r", descHandle);
                descriptor->setHandle(descHandle);
            }
        }
    }    
    
    serviceCount++;
    
    //FIXME: There is no GattService pointer array in GattServer. 
    //        There should be one? (Only the user is aware of GattServices!) Report to forum.
    
    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Reads the value of a characteristic, based on the service
            and characteristic index fields

    @param[in]  charHandle
                The handle of the GattCharacteristic to read from
    @param[in]  buffer
                Buffer to hold the the characteristic's value
                (raw byte array in LSB format)
    @param[in]  len
                The number of bytes read into the buffer

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t BlueNRGGattServer::readValue(uint16_t charHandle, uint8_t buffer[], uint16_t *const lengthP)
{
    DEBUG("ReadValue() Not Supported\n\r");
    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Updates the value of a characteristic, based on the service
            and characteristic index fields

    @param[in]  charHandle
                The handle of the GattCharacteristic to write to
    @param[in]  buffer
                Data to use when updating the characteristic's value
                (raw byte array in LSB format)
    @param[in]  len
                The number of bytes in buffer

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t BlueNRGGattServer::updateValue(uint16_t charHandle, uint8_t buffer[], uint16_t len, bool localOnly)
{
    tBleStatus ret;    
    tHalUint8 buff[2];

    DEBUG("updating bleCharacteristic charHandle =%u, corresponding serviceHanle= %u\n\r", charHandle, bleCharHanldeMap.find(charHandle)->second);  
   
    ret = aci_gatt_update_char_value(bleCharHanldeMap.find(charHandle)->second, charHandle, 0, len, buffer);

    if (ret != BLE_STATUS_SUCCESS){
        DEBUG("Error while updating characteristic.\n\r") ;
        return BLE_ERROR_PARAM_OUT_OF_RANGE ; //Not correct Error Value 
        //FIXME: Define Error values equivalent to BlueNRG Error Codes.
    }

    //Generate Data Sent Event Here? (GattServerEvents::GATT_EVENT_DATA_SENT)  //FIXME: Is this correct?
    //Check if characteristic property is NOTIFY|INDICATE, if yes generate event
    GattCharacteristic *p_char = BlueNRGGattServer::getInstance().getCharacteristicFromHandle(charHandle);
    if(p_char->getProperties() &  (GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
                | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE)) {
        BlueNRGGattServer::getInstance().handleEvent(GattServerEvents::GATT_EVENT_DATA_SENT, charHandle);
    }

    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Reads a value according to the handle provided

    @param[in]  charHandle
                The handle of the GattCharacteristic to read from

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
ble_error_t BlueNRGGattServer::Read_Request_CB(tHalUint16 handle)
{
    //signed short refvalue;
    uint16_t gapConnectionHandle = BlueNRGGap::getInstance().getConnectionHandle();
    
    //EXIT:
    if(gapConnectionHandle != 0)
    aci_gatt_allow_read(gapConnectionHandle);
}

/**************************************************************************/
/*!
    @brief  Returns the GattCharacteristic according to the handle provided

    @param[in]  charHandle
                The handle of the GattCharacteristic

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @section EXAMPLE

    @code

    @endcode
*/
/**************************************************************************/
GattCharacteristic* BlueNRGGattServer::getCharacteristicFromHandle(tHalUint16 attrHandle)
{
    GattCharacteristic *p_char = NULL;
    int i;
    uint16_t handle;

    //DEBUG("BlueNRGGattServer::getCharacteristicFromHandle()>>Attribute Handle received 0x%x\n\r",attrHandle);
    for(i=0; i<characteristicCount; i++)
    {
        handle = p_characteristics[i]->getValueAttribute().getHandle();
        
        if(i==characteristicCount-1)//Last Characteristic check
        {
            if(attrHandle>=bleCharacteristicHandles[handle])
            {
                p_char = p_characteristics[i];
                DEBUG("Found Characteristic Properties 0x%x\n\r",p_char->getProperties());
                break;
            }            
        }
        else {
            //Testing if attribute handle is between two Characteristic Handles
            if(attrHandle>=bleCharacteristicHandles[handle] && attrHandle<bleCharacteristicHandles[handle+1])
            {
                p_char = p_characteristics[i];
                //DEBUG("Found Characteristic Properties 0x%x\n\r",p_char->getProperties());
                break;
            } else continue;
        }
    }

    return p_char;
}


