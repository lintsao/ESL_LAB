/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
* File Name          : osal.c
* Author             : AMS - HEA&RF BU
* Version            : V1.0.0
* Date               : 19-July-2012
* Description        : Implementation of OS abstraction layer functions used by
*                      the library.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <string.h>
#include <osal.h>
 
/**
 * Osal_MemCpy
 * 
 */
 
void* Osal_MemCpy(void *dest,const void *src,tHalUint size)
{
    return(memcpy(dest,src,size)); 
}

/**
 * Osal_MemSet
 * 
 */
 
void* Osal_MemSet(void *ptr, tHalInt value,tHalUint size)
{
    return(memset(ptr,value,size));
}

/******************************************************************************
 * local Functions
 *****************************************************************************/ 

