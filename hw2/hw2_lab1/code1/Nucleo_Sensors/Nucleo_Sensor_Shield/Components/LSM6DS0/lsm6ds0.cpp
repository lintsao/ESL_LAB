/**
******************************************************************************
* @file    x_cube_mems_lsm6ds0.h
* @author  AST / EST
* @version V0.0.1
* @date    9-December-2014
* @brief   Header file for component LSM6DS0
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
#include "lsm6ds0.h"
#include "lsm6ds0_platform.h"
#include <math.h>

/* Methods -------------------------------------------------------------------*/

/**
 * @brief Read data from LSM6DS0 Gyroscope and calculate angular rate in mdps.
 * @param float *pfData
 * @retval None.
 */
void LSM6DS0::Gyro_GetAxes(AxesRaw_TypeDef *pData)
{

  uint8_t tempReg = 0x00;
  int16_t pDataRaw[3];
  float sensitivity = 0;
  int ret;

  LSM6DS0::Gyro_GetAxesRaw(pDataRaw);

  //IMU_6AXES_IO_Read(&tempReg, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG1_G, 1);
  ret = dev_i2c.i2c_read(&tempReg, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG1_G, 1);

  if (ret == 0)
  {
    tempReg &= LSM6DS0_G_FS_MASK;

    switch(tempReg)
    {
      case LSM6DS0_G_FS_245:
        sensitivity = 8.75;
        break;
      case LSM6DS0_G_FS_500:
        sensitivity = 17.50;
        break;
      case LSM6DS0_G_FS_2000:
        sensitivity = 70;
        break;
    }
  }

  pData->AXIS_X = (int32_t)(pDataRaw[0] * sensitivity);
  pData->AXIS_Y = (int32_t)(pDataRaw[1] * sensitivity);
  pData->AXIS_Z = (int32_t)(pDataRaw[2] * sensitivity);
}


/**
 * @brief Read raw data from LSM6DS0 Gyroscope output register.
 * @param float *pfData
 * @retval None.
 */
void LSM6DS0::Gyro_GetAxesRaw(int16_t *pData)
{
    uint8_t tempReg[2] = {0,0};
    int ret;

    pData[0] = pData[1] = pData[2] = 0;

    //IMU_6AXES_IO_Read(&tempReg[0], LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_OUT_X_L_G + 0x80, 2);
    ret = dev_i2c.i2c_read(&tempReg[0], LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_OUT_X_L_G + 0x80, 2);

    if (ret == 0)
    {
      pData[0] = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);

      //IMU_6AXES_IO_Read(&tempReg[0], LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_OUT_Y_L_G + 0x80, 2);
      ret = dev_i2c.i2c_read(&tempReg[0], LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_OUT_Y_L_G + 0x80, 2);
    }
    
    if (ret == 0)
    {
      pData[1] = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);

      //IMU_6AXES_IO_Read(&tempReg[0], LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_OUT_Z_L_G + 0x80, 2);
      ret = dev_i2c.i2c_read(&tempReg[0], LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_OUT_Z_L_G + 0x80, 2);
    }

    if (ret == 0)
    {
      pData[2] = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);
    }
}


/**
 * @brief Read data from LSM6DS0 Accelerometer and calculate linear acceleration in mg.
 * @param float *pfData
 * @retval None.
 */
void LSM6DS0::Acc_GetAxes(AxesRaw_TypeDef *pData)
{
  uint8_t tempReg = 0x00;
  int16_t pDataRaw[3];
  float sensitivity = 0;
  int ret;

  Acc_GetAxesRaw(pDataRaw);

  //IMU_6AXES_IO_Read(&tempReg, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG6_XL, 1);
  ret = dev_i2c.i2c_read(&tempReg, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG6_XL, 1);

  if (ret == 0)
  {
    tempReg &= LSM6DS0_XL_FS_MASK;

    switch(tempReg)
    {
      case LSM6DS0_XL_FS_2G:
        sensitivity = 0.061;
        break;
      case LSM6DS0_XL_FS_4G:
        sensitivity = 0.122;
        break;
      case LSM6DS0_XL_FS_8G:
        sensitivity = 0.244;
        break;
    }
  }

  pData->AXIS_X = (int32_t)(pDataRaw[0] * sensitivity);
  pData->AXIS_Y = (int32_t)(pDataRaw[1] * sensitivity);
  pData->AXIS_Z = (int32_t)(pDataRaw[2] * sensitivity);
  
}

/**
 * @brief Read raw data from LSM6DS0 Accelerometer output register.
 * @param float *pfData
 * @retval None.
 */
void LSM6DS0::Acc_GetAxesRaw(int16_t *pData)
{
    uint8_t tempReg[2] = {0,0};
    int ret;

    pData[0] = pData[1] = pData[2] = 0;

    //IMU_6AXES_IO_Read(&tempReg[0], LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_OUT_X_L_XL + 0x80, 2);
    ret = dev_i2c.i2c_read(&tempReg[0], LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_OUT_X_L_XL + 0x80, 2);

    if (ret == 0)
    {
      pData[0] = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);

      //IMU_6AXES_IO_Read(&tempReg[0], LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_OUT_Y_L_XL + 0x80, 2);
      ret = dev_i2c.i2c_read(&tempReg[0], LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_OUT_Y_L_XL + 0x80, 2);
    }
    
    if (ret == 0)
    {
      pData[1] = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);
      
      //IMU_6AXES_IO_Read(&tempReg[0], LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_OUT_Z_L_XL + 0x80, 2);
      ret = dev_i2c.i2c_read(&tempReg[0], LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_OUT_Z_L_XL + 0x80, 2);
    }
    
    if (ret == 0)
    {
      pData[2] = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);
    }
}

/**
 * @brief  Read ID address of HTS221
 * @param  Device ID address
 * @retval ID name
 */
uint8_t LSM6DS0::ReadID(void)
{
    uint8_t tmp=0x00;
    int ret;
    
    /* Read WHO I AM register */
    //IMU_6AXES_IO_Read(&tmp, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_WHO_AM_I_ADDR, 1);
    ret = dev_i2c.i2c_read(&tmp, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_WHO_AM_I_ADDR, 1);

    /* Return the ID */
    return ((ret == 0) ? (uint8_t)tmp : 0);
}

/**
 * @brief  Set LSM6DS0 Initialization.
 * @param  InitStruct: it contains the configuration setting for the LSM6DS0.
 * @retval None
 */
void LSM6DS0::Init() {
    
    uint8_t tmp1 = 0x00;
    int ret;

/******* Gyroscope init *******/

    //IMU_6AXES_IO_Read(&tmp1, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG1_G, 1);
    ret = dev_i2c.i2c_read(&tmp1, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG1_G, 1);

    if (ret == 0)
    {
      /* Output Data Rate selection */
      tmp1 &= ~(LSM6DS0_G_ODR_MASK);
      tmp1 |= LSM6DS0_G_ODR_119HZ;

      /* Full scale selection */
      tmp1 &= ~(LSM6DS0_G_FS_MASK);
      tmp1 |= LSM6DS0_G_FS_2000;

      //IMU_6AXES_IO_Write(&tmp1, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG1_G, 1);
      ret = dev_i2c.i2c_write(&tmp1, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG1_G, 1);
    }
    
    if (ret == 0)
    {
      //IMU_6AXES_IO_Read(&tmp1, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG4, 1);
      ret = dev_i2c.i2c_read(&tmp1, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG4, 1);
    }

    if (ret == 0)
    {
      /* Enable X axis selection */
      tmp1 &= ~(LSM6DS0_G_XEN_MASK);
      tmp1 |= LSM6DS0_G_XEN_ENABLE;

      /* Enable Y axis selection */
      tmp1 &= ~(LSM6DS0_G_YEN_MASK);
      tmp1 |= LSM6DS0_G_YEN_ENABLE;

      /* Enable Z axis selection */
      tmp1 &= ~(LSM6DS0_G_ZEN_MASK);
      tmp1 |= LSM6DS0_G_ZEN_ENABLE;

      //IMU_6AXES_IO_Write(&tmp1, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG4, 1);
      ret = dev_i2c.i2c_write(&tmp1, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG4, 1);
    }

/******************************/
/***** Accelerometer init *****/
    if (ret == 0)
    {
      //IMU_6AXES_IO_Read(&tmp1, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG6_XL, 1);
      ret = dev_i2c.i2c_read(&tmp1, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG6_XL, 1);
    }
    
    if (ret == 0)
    {
      /* Output Data Rate selection */
      tmp1 &= ~(LSM6DS0_XL_ODR_MASK);
      tmp1 |= LSM6DS0_XL_ODR_119HZ;

      /* Full scale selection */
      tmp1 &= ~(LSM6DS0_XL_FS_MASK);
      tmp1 |= LSM6DS0_XL_FS_2G;

      //IMU_6AXES_IO_Write(&tmp1, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG6_XL, 1);
      ret = dev_i2c.i2c_write(&tmp1, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG6_XL, 1);
    }
    
    if (ret == 0)
    {
      //IMU_6AXES_IO_Read(&tmp1, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG5_XL, 1);
      ret = dev_i2c.i2c_read(&tmp1, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG5_XL, 1);
    }
    
    if (ret == 0)
    {
      /* Enable X axis selection */
      tmp1 &= ~(LSM6DS0_XL_XEN_MASK);
      tmp1 |= LSM6DS0_XL_XEN_ENABLE;

      /* Enable Y axis selection */
      tmp1 &= ~(LSM6DS0_XL_YEN_MASK);
      tmp1 |= LSM6DS0_XL_YEN_ENABLE;

      /* Enable Z axis selection */
      tmp1 &= ~(LSM6DS0_XL_ZEN_MASK);
      tmp1 |= LSM6DS0_XL_ZEN_ENABLE;

      //IMU_6AXES_IO_Write(&tmp1, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG5_XL, 1);
      ret = dev_i2c.i2c_write(&tmp1, LSM6DS0_XG_MEMS_ADDRESS, LSM6DS0_XG_CTRL_REG5_XL, 1);
    }
/******************************/

    if (ret == 0)
    {        
      if(ReadID() == I_AM_LSM6DS0_XG)
      {
        LSM6DS0Initialized = 1;
        //ret = HUM_TEMP_OK;
      }
    }
    
    return;
}
