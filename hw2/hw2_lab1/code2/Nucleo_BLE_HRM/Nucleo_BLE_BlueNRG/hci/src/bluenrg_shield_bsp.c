/**
******************************************************************************
* File Name          : bluenrg_shield_bsp.c
* Date               : 01/10/2014
* Description        : This file provides code for the BlueNRG Shield driver
*                      based on mbed HAL.
******************************************************************************
*
* COPYRIGHT(c) 2014 STMicroelectronics
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

#include "hci.h"
#include "spi_api.h"
#include "gpio_irq_api.h"
#include "gpio_api.h"
#include "wait_api.h"
#include "pinmap.h"
#include "bluenrg_shield_bsp.h"
//#include "Utils.h"

spi_t __spi;
gpio_irq_t irq_exti;
gpio_t gpio_pin_A0;

gpio_irq_t irq_exti_UB;
gpio_t gpio_pin_UB;

gpio_t gpio_pin_CS;
gpio_t gpio_pin_MOSI;
gpio_t gpio_pin_MISO;
gpio_t gpio_pin_SCLK;
gpio_t gpio_pin_RESET;

void EXTI_irq_handler(uint32_t id, gpio_irq_event event);

/** @addtogroup BlueNRG_Shield
 *  @{
 */

/** @defgroup BlueNRG_Shield_Driver
 *  @brief BlueNRG Shield driver based on mbed HAL
 *  @{
 */
 

/*
*	mbed EXTI IRQ Handler
*
*/
bool user_button_pressed = false;
void USER_BUTTON_EXTI_irq_handler(uint32_t id, gpio_irq_event event)
{
	user_button_pressed = true;
}
	   
void EXTI_irq_handler(uint32_t id, gpio_irq_event event)
{
  tHciDataPacket * hciReadPacket = NULL;
  uint8_t data_len;
  
  if(id == (uint32_t)BNRG_SPI_INSTANCE) //Check id of the IRQ
  {
	  while (gpio_read(&gpio_pin_A0) == 1) 
	  {
      if (list_is_empty (&hciReadPktPool) == FALSE)
      {
        /* enqueueing a packet for read */
        list_remove_head (&hciReadPktPool, (tListNode **)&hciReadPacket);
        data_len = BlueNRG_SPI_Read_All(hciReadPacket->dataBuff, HCI_PACKET_SIZE);
        
        if(data_len > 0)
        {
          HCI_Input(hciReadPacket);/* Packet will be inserted to the correct queue */
        } 
        else 
        {
          list_insert_head(&hciReadPktPool, (tListNode *)hciReadPacket);/* Insert the packet back into the pool */
        }
      }
      else
      {
        /* TODO: HCI Read Packet Pool is empty, wait for a free packet */
      }
    }
  }
  else if(id == (uint32_t)BNRG_SPI_INSTANCE + 1) //Check id of the IRQ
  {
	user_button_pressed = true;
  }
}

/**
* @brief  This function is used to initialize the SPI communication with
*         the BlueNRG Shield. All params should come from the User
* @param  SPI_MOSI      : PA_7 (Nucleo)
* @param  SPI_MISO      : PA_6
* @param  SPI_SCLK      : PB_3
* @param  SPI_CS        : PA_1
* @param  EXTI_IRQ      : PA_0
* @param  BlueNRG_RESET : PA_8
* @retval None
*/

void BNRG_SPI_Init(void)
{
  int ret;
	spi_init(&__spi, PA_7, PA_6, PB_3, NC);

#if 1
  ret = gpio_irq_init(&irq_exti_UB, USER_BUTTON, USER_BUTTON_EXTI_irq_handler,(uint32_t)(BNRG_SPI_INSTANCE+1));
  gpio_irq_set       (&irq_exti_UB, IRQ_FALL, 1);//Set mode to IRQ_RISE
  gpio_init_in(&gpio_pin_UB, USER_BUTTON);//User Button in Nucleo//Configure the GPIO Pin as Input pin and PullDefault
#endif
	
  /*Init IRQ for EXTI Interrupt*/	
  ret = gpio_irq_init(&irq_exti, A0, EXTI_irq_handler,(uint32_t)BNRG_SPI_INSTANCE);
  gpio_irq_set       (&irq_exti, IRQ_RISE, 1);//Set mode to IRQ_RISE
  gpio_init_in(&gpio_pin_A0, A0);//PA_0 in Nucleo//Configure the GPIO Pin as Input pin and PullDefault
  
  /* Reset Pin Config */    
  gpio_init (&gpio_pin_RESET, D7);//PA_8 in Nucleo
  gpio_mode (&gpio_pin_RESET, PullNone);
  gpio_dir  (&gpio_pin_RESET, PIN_OUTPUT);
  gpio_write(&gpio_pin_RESET, 1);
	
  /* NSS/CSN/CS - PA_1*/		
  gpio_init (&gpio_pin_CS, A1);//PA_1 in Nucleo
  gpio_mode (&gpio_pin_CS, PullNone);
  gpio_dir  (&gpio_pin_CS, PIN_OUTPUT);
  gpio_write(&gpio_pin_CS, 1);
}

/**
* @brief  Read from BlueNRG SPI buffer and store data into local buffer
* @param  buffer:    buffer where data from SPI are stored
* @param  buff_size: buffer size
* @retval number of read bytes
*/
int32_t BlueNRG_SPI_Read_All(uint8_t *buffer, uint8_t buff_size)
{
  uint16_t byte_count;
  uint8_t len = 0;
  uint8_t i = 0;
  uint8_t char_ff = 0xff;
  volatile uint8_t read_char, tmpreg;
  
  uint8_t header_master[5] = {0x0b, 0x00, 0x00, 0x00, 0x00};
  uint8_t header_slave[5];
  
  /* CS reset */
  gpio_write(&gpio_pin_CS, 0);
	
  /* Read the header */
  for (i = 0; i < 5; i++)
  { 
		tmpreg = spi_master_write(&__spi, header_master[i]);
		header_slave[i] = (uint8_t)(tmpreg);
  }    
  
  
  if (header_slave[0] == 0x02) {
    /* device is ready */
    byte_count = (header_slave[4]<<8)|header_slave[3];
    
    if (byte_count > 0) {      
      /* avoid to read more data that size of the buffer */
      if (byte_count > buff_size){
        byte_count = buff_size;
      }
      
      for (len = 0; len < byte_count; len++){
				read_char = spi_master_write(&__spi, char_ff);
        buffer[len] = read_char;
      }    
    }    
  }
  /* Release CS line */
	gpio_write(&gpio_pin_CS, 1);
  
  return len;   
}

/**
* @brief  Write data from local buffer to SPI
* @param  data1:     first data buffer to be written
* @param  data2:     second data buffer to be written
* @param  Nb_bytes1: size of first data buffer to be written
* @param  Nb_bytes2: size of second data buffer to be written
* @retval number of read bytes
*/
int32_t BlueNRG_SPI_Write(uint8_t* data1, uint8_t* data2, uint8_t Nb_bytes1, uint8_t Nb_bytes2)
{  
  uint32_t i;
  volatile uint8_t read_char;
  int32_t result = 0;
  volatile uint8_t tmpreg;
	
  unsigned char header_master[5] = {0x0a, 0x00, 0x00, 0x00, 0x00};
  unsigned char header_slave[5]  = {0xaa, 0x00, 0x00, 0x00, 0x00};
  
  Disable_SPI_IRQ();
  
  /* CS reset */
	gpio_write(&gpio_pin_CS, 0);
  
  /* Exchange header */  
  for (i = 0; i < 5; i++)
  { 
		tmpreg = spi_master_write(&__spi, header_master[i]);
		header_slave[i] = tmpreg;
  }      
  
  if (header_slave[0] == 0x02) {
    /* SPI is ready */
    if (header_slave[1] >= (Nb_bytes1+Nb_bytes2)) {
      /*  Buffer is big enough */
      for (i = 0; i < Nb_bytes1; i++) {
				read_char = spi_master_write(&__spi, *(data1 + i));
      }
      for (i = 0; i < Nb_bytes2; i++) {
				read_char = spi_master_write(&__spi, *(data2 + i));
      }
    } else {
      /* Buffer is too small */
      result = -2;
    }
  } else {
    /* SPI is not ready */
    result = -1;
  }
  
  /* Release CS line */
	gpio_write(&gpio_pin_CS, 1);
  
  Enable_SPI_IRQ();
  
  return result;
}

/**
 * Writes data to a serial interface. *
 * @param  data1    1st buffer
 * @param  data2    2nd buffer
 * @param  n_bytes1 number of bytes in 1st buffer
 * @param  n_bytes2 number of bytes in 2nd buffer
 */
void Hal_Write_Serial(const void* data1, const void* data2, int32_t n_bytes1, int32_t n_bytes2)
{
  struct timer t;
  
  Timer_Set(&t, CLOCK_SECOND/10);
  
  while(1){
    if(BlueNRG_SPI_Write((uint8_t *)data1,(uint8_t *)data2, n_bytes1, n_bytes2)==0) break;
    if(Timer_Expired(&t)){
      break;
    }
  }
}

/**
 * @brief  Disable SPI IRQ
 * @param  None
 * @retval None
 */
void Disable_SPI_IRQ(void)
{  
	gpio_irq_disable(&irq_exti);
}

/**
 * @brief  Enable SPI IRQ
 * @param  None
 * @retval None
 */
void Enable_SPI_IRQ(void)
{  
	gpio_irq_enable(&irq_exti);
}

/**
 * @brief  Clear Pending SPI IRQ
 * @param  None
 * @retval None
 */
void Clear_SPI_IRQ(void)
{
  //Not Used
}

/**
 * @brief  Clear EXTI (External Interrupt) line for SPI IRQ
 * @param  None
 * @retval None
 */
void Clear_SPI_EXTI_Flag(void)
{
	//Not Used
}

/**
 * @brief  Reset the BlueNRG
 * @param  None
 * @retval None
 */
void BlueNRG_RST(void)
{    
  gpio_write(&gpio_pin_RESET, 0);
  wait_us(5);
	
  gpio_write(&gpio_pin_RESET, 1);
  wait_us(5);
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/