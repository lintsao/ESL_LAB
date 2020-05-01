/**
******************************************************************************
* @file    x_cube_mems_lps25.cpp
* @author  AST / EST
* @version V0.0.1
* @date    1-December-2014
* @brief   Implementation file for component LPS25H
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

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "lps25h.h"
#include "lps25h_platform.h"

/* Methods -------------------------------------------------------------------*/

/**
 * @brief Read LPS25H output register, and calculate the pressure in mbar.
 * @param float *pressure. Pressure value in mbar.
 * @retval LPS25H_ERROR or LPS25H_OK.
 */
void LPS25H::GetPressure(float* pfData)
{
    
    uint32_t raw_press = 0;
    
    if(isInitialized()==0)
      {
        pfData = 0;
        return;
      }
    
    ReadRawPressure(&raw_press);

    /* return the built value */
    //tempInt = raw_press / 4096;

    *pfData = (float)raw_press /4096.0f;
}

/**
 * @brief Read LPS25H output register, and calculate the raw  pressure.
 * @param uint32_t: raw_press. Pressure raw value.
 * @retval LPS25H_ERROR or LPS25H_OK.
 */
void LPS25H::ReadRawPressure(uint32_t *raw_press)
{
    uint8_t buffer[3], i;
    uint32_t tempVal=0;
    int ret;

    /* Read the register content */
    //PRESSURE_IO_Read(buffer, LPS25H_SlaveAddress, LPS25H_PRESS_POUT_XL_ADDR+0x80, 3);
    ret = dev_i2c.i2c_read(buffer, LPS25H_ADDRESS_HIGH, LPS25H_PRESS_POUT_XL_ADDR+0x80, 3);

    if (ret == 0)
    {
      /* Build the raw data */
      for (i = 0 ; i < 3 ; i++)
          tempVal |= (((uint32_t) buffer[i]) << (8 * i));

      /* convert the 2's complement 24 bit to 2's complement 32 bit */
      if (tempVal & 0x00800000)
          tempVal |= 0xFF000000;

      /* return the built value */
      *raw_press = ((uint32_t) tempVal);
    }
}

/**
 * @brief  Read ID address of HTS221
 * @param  Device ID address
 * @retval ID name
 */
uint8_t LPS25H::ReadID(void)
{
    uint8_t tmp;

    /* Read the register content */
    int ret;    
    //PRESSURE_IO_Read(&tmp, LPS25H_SlaveAddress, LPS25H_WHO_AM_I_ADDR, 1);
    ret = dev_i2c.i2c_read(&tmp, LPS25H_ADDRESS_HIGH, LPS25H_WHO_AM_I_ADDR, 1);
        
    /* Return the ID */
    return ((ret == 0) ? (uint8_t)tmp : 0);
}

/**
 * @brief  Set HTS221 Initialization.
 * @param  InitStruct: it contains the configuration setting for the HTS221.
 * @retval None
 */
void LPS25H::Init() {
    int ret;    
    uint8_t tmp1 = 0x00;

    Power_ON();

    //PRESSURE_IO_Read(&tmp1, LPS25H_SlaveAddress, LPS25H_CTRL_REG1_ADDR, 1);
    ret = dev_i2c.i2c_read(&tmp1, LPS25H_ADDRESS_HIGH, LPS25H_CTRL_REG1_ADDR, 1);
    
    if (ret == 0)
    {
      /* Output Data Rate selection */
      tmp1 &= ~(LPS25H_ODR_MASK);
      tmp1 |= LPS25H_ODR_1Hz;

      /* Interrupt circuit selection */
      tmp1 &= ~(LPS25H_DIFF_EN_MASK);
      tmp1 |= LPS25H_DIFF_ENABLE;

      /* Block Data Update selection */
      tmp1 &= ~(LPS25H_BDU_MASK);
      tmp1 |= LPS25H_BDU_CONT;

      /* Serial Interface Mode selection */
      tmp1 &= ~(LPS25H_SPI_SIM_MASK);
      tmp1 |= LPS25H_SPI_SIM_3W;

      //PRESSURE_IO_Write(&tmp1, LPS25H_SlaveAddress, LPS25H_CTRL_REG1_ADDR, 1);
      ret = dev_i2c.i2c_write(&tmp1, LPS25H_ADDRESS_HIGH, LPS25H_CTRL_REG1_ADDR, 1);
    }
    
    if (ret == 0)
    {        
      //PRESSURE_IO_Read(&tmp1, LPS25H_SlaveAddress, LPS25H_RES_CONF_ADDR, 1);
      ret = dev_i2c.i2c_read(&tmp1, LPS25H_ADDRESS_HIGH, LPS25H_RES_CONF_ADDR, 1);
    }

    if (ret == 0)
    {
      /* Pressure Res selection */
      tmp1 &= ~(LPS25H_P_RES_MASK);
      tmp1 |= LPS25H_P_RES_AVG_32;

      /* Temperature Res selection */
      tmp1 &= ~(LPS25H_T_RES_MASK);
      tmp1 |= LPS25H_T_RES_AVG_16;

      //PRESSURE_IO_Write(&tmp1, LPS25H_SlaveAddress, LPS25H_RES_CONF_ADDR, 1);
      ret = dev_i2c.i2c_write(&tmp1, LPS25H_ADDRESS_HIGH, LPS25H_RES_CONF_ADDR, 1);
    }
    
    if (ret == 0)
    {
      if(ReadID() == I_AM_LPS25H)
      {
          Lps25hInitialized = 1;
          //ret = HUM_TEMP_OK;
      }
    }
    
    return;
}

int LPS25H::Power_ON() {
    uint8_t tmpreg;
    int ret;

    /* Read the register content */
    //PRESSURE_IO_Read(&tmpreg, LPS25H_SlaveAddress, LPS25H_CTRL_REG1_ADDR, 1);
    ret = dev_i2c.i2c_read(&tmpreg, 0xBA, 0x20, 1);
    
    /* Set the power down bit */
    tmpreg |= LPS25H_MODE_ACTIVE;

    /* Write register */
    //PRESSURE_IO_Write(&tmpreg, LPS25H_SlaveAddress, LPS25H_CTRL_REG1_ADDR, 1);
    ret = dev_i2c.i2c_write(&tmpreg, LPS25H_ADDRESS_HIGH, LPS25H_CTRL_REG1_ADDR, 1);
    return ret;
    
}

int LPS25H::LPS25H_Calibration() {
  
    int ret = 0;
    
    if(Lps25hInitialized == 1)
    {
      return 1; //TODO: Error Codes definitions
    }
    
    return ret;
}
