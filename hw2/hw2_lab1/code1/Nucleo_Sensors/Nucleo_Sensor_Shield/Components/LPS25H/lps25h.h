/**
******************************************************************************
* @file    x_cube_mems_lps25.h
* @author  AST / EST
* @version V0.0.1
* @date    1-December-2014
* @brief   Header file for component LPS25H
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

#ifndef __X_CUBE_MEMS_LPS25H_H
#define __X_CUBE_MEMS_LPS25H_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "x_cube_mems_i2c.h"

/* Classes -------------------------------------------------------------------*/
/** Class representing a LPS25H sensor component
 */
class LPS25H
{
public:
    /** Constructor
     * @param 
     */
    LPS25H(DevI2C &i2c) : dev_i2c(i2c) {
        Lps25hInitialized = 0;
        Init();
    };

    void GetPressure(float* pfData);
    void ReadRawPressure(uint32_t *raw_press);
    void    Init();
    uint8_t   ReadID(void);
    void      RebootCmd(void);
    int      Power_OFF(void); 
    int      Power_ON(void);
    int        LPS25H_Calibration();
private:
    
    uint8_t isInitialized(void)
    {
        return Lps25hInitialized;
    }
    
    DevI2C &dev_i2c;
    uint8_t Lps25hInitialized;
};

#endif // __X_CUBE_MEMS_LPS25H_H

