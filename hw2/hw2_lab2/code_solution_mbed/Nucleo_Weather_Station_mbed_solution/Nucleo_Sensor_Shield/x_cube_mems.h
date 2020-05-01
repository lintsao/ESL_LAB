/**
  ******************************************************************************
  * @file    x_cube_mems.h
  * @author  AST / EST
  * @version V0.0.1
  * @date    01-December-2014
  * @brief   Header file for class X_CUBE_MEMS representing an MEMS expansion board
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


#ifndef __X_CUBE_MEMS_H
#define __X_CUBE_MEMS_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"

#include "x_cube_mems_i2c.h"
#include "Components/HTS221/hts221.h"
#include "Components/LPS25H/lps25h.h"
#include "Components/LIS3MDL/lis3mdl.h"
#include "Components/LSM6DS0/lsm6ds0.h"

/* Classes -------------------------------------------------------------------*/
/** Class X_CUBE_MEMS represents the MEMS Sensor expansion board X-NUCLEO-IKS01A1
 *
 *  The expansion board has the following IPs:\n
 *  - HTS221 Relative Humidity and Temperature Sensor
 *  - LIS3MDL 3-Axis Magnetometer
 *  - LPS25H MEMS Pressure Sensor
 *  - LSM6DS0 3D Acceleromenter and 3D Gyroscope
 * * 
 * @code
 * // MEMS Sensors expansion board singleton instance
 * static X_CUBE_MEMS *mems_expansion_board = X_CUBE_MEMS::Instance();
 * @endcode
 */
class X_CUBE_MEMS
{
protected:
    X_CUBE_MEMS(void);

public:
    static X_CUBE_MEMS* Instance(void);

    DevI2C dev_i2c;
    HTS221 hts221;
    LPS25H lps25h;
    LIS3MDL lis3mdl;
    LSM6DS0 lsm6ds0;

private:
    static X_CUBE_MEMS *_instance;
};

#endif /* __X_CUBE_MEMS_H */
