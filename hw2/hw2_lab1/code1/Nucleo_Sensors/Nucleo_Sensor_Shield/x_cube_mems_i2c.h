/**
******************************************************************************
* @file    x_cube_mems_i2c.h
* @author  AST / EST
* @version V0.0.1
* @date    28-November-2014
* @brief   Header file for a special I2C class DevI2C which provides some
*          helper function for on-board communication
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

/* Define to prevent from recursive inclusion --------------------------------*/
#ifndef __X_CUBE_MEMS_I2C_H
#define __X_CUBE_MEMS_I2C_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"

typedef struct {
    int32_t AXIS_X;
    int32_t AXIS_Y;
    int32_t AXIS_Z;
} AxesRaw_TypeDef;

/* Classes -------------------------------------------------------------------*/
/** Helper class DevI2C providing some common functionality useful for on-board
 *  communication.
 */
class DevI2C : public I2C
{
 public:
  /** Create a DevI2C Master interface, connected to the specified pins
   *
   *  @param sda I2C data line pin
   *  @param scl I2C clock line pin
   */
 DevI2C(PinName sda, PinName scl) : I2C(sda, scl) {};

  /**
   * @brief  Writes a buffer from the I2C peripheral device.
   * @param  pBuffer pointer to data to be read.
   * @param  DeviceAddr specifies the peripheral device slave address
   *         (correctly masked).
   * @param  RegisterAddr specifies internal address register to read from.
   * @param  NumByteToWrite number of bytes to be written.
   * @retval 0 if ok, -1 if an I2C error has occured
   * @note   on some devices if NumByteToWrite is greater
   *         than one, the DeviceAddr must be masked correctly
   */
  int i2c_write(uint8_t* pBuffer, uint8_t DeviceAddr, uint8_t RegisterAddr, 
		uint16_t NumByteToWrite)
  {
    int ret;
    uint8_t tmp[32];
	
    //Acquire mbed mutex/semaphore/lock?
    
    /* First, send device address. Then, send data and STOP condition */
    tmp[0] = RegisterAddr;
    memcpy(tmp+1, pBuffer, NumByteToWrite);

    ret = write(DeviceAddr, (const char*)tmp, NumByteToWrite+1, 0);
    
    //Release mbed mutex/semaphore/lock?
    
    if(ret) {
      error("%s: dev = %d, reg = %d, num = %d\n",
	    __func__, DeviceAddr, RegisterAddr, NumByteToWrite);
      return -1;
    }
    return 0;
  }

  /**
   * @brief  Reads a buffer from the I2C peripheral device.
   * @param  pBuffer pointer to data to be read.
   * @param  DaviceAddr specifies the peripheral device slave address
   *         (correctly masked)..
   * @param  RegisterAddr specifies internal address register to read from.
   * @param  NumByteToRead number of bytes to be read.
   * @retval 0 if ok, -1 if an I2C error has occured
   * @note   on some devices if NumByteToRead is greater
   *         than one, the DeviceAddr must be masked correctly
   */
  int i2c_read(uint8_t* pBuffer, uint8_t DeviceAddr, uint8_t RegisterAddr, 
	       uint16_t NumByteToRead)
  {
    int ret;
    
    //Acquire mbed mutex/semaphore/lock?
    
    /* Send device address, with no STOP condition */
    ret = write(DeviceAddr, (const char*)&RegisterAddr, 1, 1);
    if(!ret) {
      /* Read data, with STOP condition  */
      ret = read(DeviceAddr, (char*)pBuffer, NumByteToRead, 0);
    }
    
    //Release mbed mutex/semaphore/lock?
    
    if(ret) {
      error("%s: dev = %d, reg = %d, num = %d\n",
	    __func__, DeviceAddr, RegisterAddr, NumByteToRead);
      return -1;
    }
    return 0;
  }
};

#endif /* __X_CUBE_MEMS_I2C_H */
