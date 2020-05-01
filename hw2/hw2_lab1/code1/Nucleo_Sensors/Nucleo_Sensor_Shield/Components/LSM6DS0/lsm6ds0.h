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

#ifndef __X_CUBE_MEMS_LSM6DS0_H
#define __X_CUBE_MEMS_LSM6DS0_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "x_cube_mems_i2c.h"


/* Classes -------------------------------------------------------------------*/
/** Class representing a HTS221 sensor component
 */
class LSM6DS0
{
public:
    /** Constructor
     * @param 
     */
    LSM6DS0(DevI2C &i2c) : dev_i2c(i2c) {
        LSM6DS0Initialized = 0;
        Init();
    };

    void Gyro_GetAxes(AxesRaw_TypeDef *pData);
    void Gyro_GetAxesRaw(int16_t *pData);
    void Acc_GetAxes(AxesRaw_TypeDef *pData);
    void Acc_GetAxesRaw(int16_t *pData);
    void    Init(/*HUM_TEMP_InitTypeDef *HTS221_Init*/);
    uint8_t   ReadID(void);
    void      RebootCmd(void);
    //int      Power_OFF(void); 
    //int      Power_ON(void);
    //int        LIS3MDL_Calibration();
private:
    
    uint8_t isInitialized(void)
    {
        return LSM6DS0Initialized;
    }
    
    DevI2C &dev_i2c;
    uint8_t LSM6DS0Initialized;
};

#endif // __X_CUBE_MEMS_LSM6DS0_H

