/**
******************************************************************************
* @file    x_cube_mems_hts221.h
* @author  AST / EST
* @version V0.0.1
* @date    1-December-2014
* @brief   Header file for component HTS221
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
#include "hts221.h"
#include "hts221_platform.h"


/* Temperature in degree for calibration  */
float T0_degC, T1_degC;

/* Output temperature value for calibration */
int16_t T0_out, T1_out;


/* Humidity for calibration  */
float H0_rh, H1_rh;

/* Output Humidity value for calibration */
int16_t H0_T0_out, H1_T0_out;

/* Methods -------------------------------------------------------------------*/

/**
 * @brief  Read HTS221 output register, and calculate the temperature.
 * @param  pfData : Data out pointer
 * @retval None
 */
int HTS221::GetTemperature(float* pfData)
{
    int16_t T_out, temperature_t;
    uint8_t tempReg[2] = {0,0};
    uint8_t tmp = 0x00;
    float T_degC;
    int ret=-1;//TODO:Define Error types?
    
    if(isInitialized()==0)
      {
        return ret;
      }
    
    ret = dev_i2c.i2c_read(&tmp, HTS221_ADDRESS, HTS221_CTRL_REG1_ADDR, 1);

    /* Output Data Rate selection */
    tmp &= (HTS221_ODR_MASK);
    
    if(tmp == 0x00){
    
      ret = dev_i2c.i2c_read(&tmp, HTS221_ADDRESS, HTS221_CTRL_REG2_ADDR, 1);
      
      /* Serial Interface Mode selection */
      tmp &= ~(HTS221_ONE_SHOT_MASK);
      tmp |= HTS221_ONE_SHOT_START;

      ret = dev_i2c.i2c_write(&tmp, HTS221_ADDRESS, HTS221_CTRL_REG2_ADDR, 1);
    
      do{
      
        ret = dev_i2c.i2c_read(&tmp, HTS221_ADDRESS, HTS221_STATUS_REG_ADDR, 1);
      }while(!(tmp&&0x01));
    
    }

    ret = dev_i2c.i2c_read(&tempReg[0], HTS221_ADDRESS , HTS221_TEMP_OUT_L_ADDR + 0x80, 2);
    T_out = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);

    T_degC = ((float)(T_out - T0_out))/(T1_out - T0_out) * (T1_degC - T0_degC) + T0_degC;

    temperature_t = (int16_t)(T_degC * pow((double)10,(double)TEMP_DECIMAL_DIGITS));

    *pfData = ((float)temperature_t)/pow((double)10,(double)TEMP_DECIMAL_DIGITS);
    
    return ret;
}


/**
 * @brief  Read HTS221 output register, and calculate the humidity.
 * @param  pfData : Data out pointer
 * @retval None
 */
int HTS221::GetHumidity(float* pfData)
{
    int16_t H_T_out, humidity_t;
    uint8_t tempReg[2] = {0,0};
    uint8_t tmp = 0x00;
    float H_rh;
    int ret;
    
    if(isInitialized()==0)
      {
        pfData = 0;
        return -1;
      }
    
    //HUM_TEMP_IO_Read(&tmp, HTS221_ADDRESS, HTS221_CTRL_REG1_ADDR, 1);
    ret = dev_i2c.i2c_read(&tmp, HTS221_ADDRESS , HTS221_CTRL_REG1_ADDR, 1);

    /* Output Data Rate selection */
    tmp &= (HTS221_ODR_MASK);
    
    if(tmp == 0x00){
    
      //HUM_TEMP_IO_Read(&tmp, HTS221_ADDRESS, HTS221_CTRL_REG2_ADDR, 1);
      ret = dev_i2c.i2c_read(&tmp, HTS221_ADDRESS , HTS221_CTRL_REG2_ADDR, 1);

      /* Serial Interface Mode selection */
      tmp &= ~(HTS221_ONE_SHOT_MASK);
      tmp |= HTS221_ONE_SHOT_START;

      //HUM_TEMP_IO_Write(&tmp, HTS221_ADDRESS, HTS221_CTRL_REG2_ADDR, 1);
      ret = dev_i2c.i2c_write(&tmp, HTS221_ADDRESS , HTS221_CTRL_REG2_ADDR, 1);
    
      do{
      
        //HUM_TEMP_IO_Read(&tmp, HTS221_ADDRESS, HTS221_STATUS_REG_ADDR, 1);
        ret = dev_i2c.i2c_read(&tmp, HTS221_ADDRESS , HTS221_STATUS_REG_ADDR, 1);
         
      }while(!(tmp&&0x02));
    
    }
    
    
    //HUM_TEMP_IO_Read(&tempReg[0], HTS221_ADDRESS, HTS221_HUMIDITY_OUT_L_ADDR + 0x80, 2);
    ret = dev_i2c.i2c_read(&tempReg[0], HTS221_ADDRESS , HTS221_HUMIDITY_OUT_L_ADDR + 0x80, 2);
    H_T_out = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);

    H_rh = ((float)(H_T_out - H0_T0_out))/(H1_T0_out - H0_T0_out) * (H1_rh - H0_rh) + H0_rh;

    humidity_t = (uint16_t)(H_rh * pow((double)10,(double)HUM_DECIMAL_DIGITS));

    *pfData = ((float)humidity_t)/pow((double)10,(double)HUM_DECIMAL_DIGITS);
    
    return ret;
}


/**
 * @brief  Read ID address of HTS221
 * @param  Device ID address
 * @retval ID name
 */
uint8_t HTS221::ReadID(void)
{
    uint8_t tmp;
    int ret;
    
    /* Read WHO I AM register */
    //HUM_TEMP_IO_Read(&tmp, HTS221_ADDRESS, HTS221_WHO_AM_I_ADDR, 1);
    ret = dev_i2c.i2c_read(&tmp, HTS221_ADDRESS, HTS221_WHO_AM_I_ADDR, 1);

    /* Return the ID */
    return ((ret == 0) ? (uint8_t)tmp : 0);
}

/**
 * @brief  Set HTS221 Initialization.
 * @param  InitStruct: it contains the configuration setting for the HTS221.
 * @retval None
 */
void HTS221::Init() {
    
    uint8_t tmp = 0x00;
    int ret;
    
    Power_ON();

    HTS221_Calibration();
    
    //HUM_TEMP_IO_Read(&tmp, HTS221_ADDRESS, HTS221_CTRL_REG1_ADDR, 1);
    ret = dev_i2c.i2c_read(&tmp, HTS221_ADDRESS, HTS221_CTRL_REG1_ADDR, 1);
    
    if (ret == 0)
    {
      /* Output Data Rate selection */
      tmp &= ~(HTS221_ODR_MASK);
      tmp |= HTS221_ODR_12_5Hz;

      //HUM_TEMP_IO_Write(&tmp, HTS221_ADDRESS, HTS221_CTRL_REG1_ADDR, 1);
      ret = dev_i2c.i2c_write(&tmp, HTS221_ADDRESS, HTS221_CTRL_REG1_ADDR, 1);
    }
    
    if (ret == 0)
    {
      if(ReadID() == I_AM_HTS221)
      {
        HumTempInitialized = 1;
        //ret = HUM_TEMP_OK;
      }
    }
    
    return;
}

int HTS221::Power_ON() {
    
    uint8_t tmpReg;

    /* Read the register content */
    int ret;    
    ret = dev_i2c.i2c_read(&tmpReg, HTS221_ADDRESS, HTS221_CTRL_REG1_ADDR, 1);
    if(ret) return ret;

    /* Set the power down bit */
    tmpReg |= HTS221_MODE_ACTIVE;

    /* Write register */
    ret = dev_i2c.i2c_write(&tmpReg, HTS221_ADDRESS, HTS221_CTRL_REG1_ADDR, 1);
    if(ret) return ret;       
    return ret;
}

int HTS221::HTS221_Calibration() {
  
    if(HumTempInitialized == 1)
    {
      return 1; //TODO: Error Codes definitions
    }
    
  /* Temperature Calibration */
    /* Temperature in degree for calibration ( "/8" to obtain float) */
    uint16_t T0_degC_x8_L, T0_degC_x8_H, T1_degC_x8_L, T1_degC_x8_H;
    uint8_t H0_rh_x2, H1_rh_x2;
    uint8_t tempReg[2] = {0,0};

    int ret;    
    ret = dev_i2c.i2c_read(tempReg, HTS221_ADDRESS, HTS221_T0_degC_X8_ADDR, 1);
    if(ret) return ret;
    
    T0_degC_x8_L = (uint16_t)tempReg[0];

    ret = dev_i2c.i2c_read(tempReg, HTS221_ADDRESS, HTS221_T1_T0_MSB_X8_ADDR, 1);
    if(ret) return ret;
    T0_degC_x8_H = (uint16_t) (tempReg[0] & 0x03);

    T0_degC = ((float)((T0_degC_x8_H<<8) | (T0_degC_x8_L)))/8;

    ret = dev_i2c.i2c_read(tempReg, HTS221_ADDRESS, HTS221_T1_degC_X8_ADDR, 1);
    T1_degC_x8_L = (uint16_t)tempReg[0];

    ret = dev_i2c.i2c_read(tempReg, HTS221_ADDRESS, HTS221_T1_T0_MSB_X8_ADDR, 1);
    T1_degC_x8_H = (uint16_t) (tempReg[0] & 0x0C);
    T1_degC_x8_H = T1_degC_x8_H >> 2;

    T1_degC = ((float)((T1_degC_x8_H<<8) | (T1_degC_x8_L)))/8;

    ret = dev_i2c.i2c_read(tempReg, HTS221_ADDRESS, HTS221_T0_OUT_L_ADDR + 0x80, 2);
    T0_out = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);

    ret = dev_i2c.i2c_read(tempReg, HTS221_ADDRESS, HTS221_T1_OUT_L_ADDR + 0x80, 2);
    T1_out = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);

    /* Humidity Calibration */
    /* Humidity in degree for calibration ( "/2" to obtain float) */

    ret = dev_i2c.i2c_read(&H0_rh_x2, HTS221_ADDRESS, HTS221_H0_RH_X2_ADDR, 1);

    ret = dev_i2c.i2c_read(&H1_rh_x2, HTS221_ADDRESS, HTS221_H1_RH_X2_ADDR, 1);

    ret = dev_i2c.i2c_read(&tempReg[0], HTS221_ADDRESS, HTS221_H0_T0_OUT_L_ADDR + 0x80, 2);
    H0_T0_out = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);

    ret = dev_i2c.i2c_read(&tempReg[0], HTS221_ADDRESS, HTS221_H1_T0_OUT_L_ADDR + 0x80, 2);
    H1_T0_out = ((((int16_t)tempReg[1]) << 8)+(int16_t)tempReg[0]);
    
    H0_rh = ((float)H0_rh_x2)/2;
    H1_rh = ((float)H1_rh_x2)/2;
    
    return ret;
}
