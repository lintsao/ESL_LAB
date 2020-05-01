
#ifndef __BLUENRG_SHIELD_BRP_H_
#define __BLUENRG_SHIELD_BRP_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "gp_timer.h"

// SPI Instance
#define BNRG_SPI_INSTANCE		        0x1234 //((uint32_t)(0x40013000)) //Peripheral base Address of SPI1

/* Exported functions --------------------------------------------------------*/
void BNRG_SPI_Init(void);
void BlueNRG_RST(void);
int32_t BlueNRG_SPI_Read_All(uint8_t *buffer,
                             uint8_t buff_size);
int32_t BlueNRG_SPI_Write(uint8_t* data1,
                          uint8_t* data2,
                          uint8_t Nb_bytes1,
                          uint8_t Nb_bytes2);
void Hal_Write_Serial(const void* data1, const void* data2, int32_t n_bytes1, int32_t n_bytes2);
void Enable_SPI_IRQ(void);
void Disable_SPI_IRQ(void);
void Clear_SPI_IRQ(void);
void Clear_SPI_EXTI_Flag(void);

#endif //_BLUENRG_SHIELD_BRP_H_

