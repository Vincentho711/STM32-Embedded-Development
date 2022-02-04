/*
 * ds1631.c
 *
 *  Created on: Jan 30, 2022
 *      Author: Vincent
 */
#include "25AA080B.h"


/**
* @brief  Write data to the 25AA080B, refer to data sheet for more details. This is a blocking function until write operation is complete.
* @param  GPIOx is the GPIO for the CS Pin, where x can be (A..K) to select the GPIO peripheral for STM32F429X device or
*                      x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices.
* @param  CS_Pin specifies the pin used as the Chip Select for the EEPROM.
*            This parameter can be one of GPIO_PIN_x where x can be (0..15).
* @param  addr is the starting address for the data to be written, 16 bits which will be transmitted in 2 bytes through SPI
* @param  spi_buf is a data buffer to send and receive SPI data, e.g. char spi_buf[20], it needs to contain the
* 			 data bytes to be written on the EEPROM
* @param  size is the number of bytes in the spi_buf to be sent to EEPROM
* @param  hspi pointer to a SPI_HandleTypeDef structure that contains
*               the configuration information for SPI module.
* @retval HAL status
*/


HAL_StatusTypeDef eeprom_write(GPIO_TypeDef* GPIOx, uint16_t CS_Pin, uint16_t addr, char* spi_buf, uint16_t size, SPI_HandleTypeDef *hspi)
{
	// Temporary buffer to store status register return byte
	char stat_buf[10];

	// First enable write enable latch (allow write operation)
	HAL_GPIO_WritePin(GPIOx, CS_Pin, GPIO_PIN_RESET);
	// Transmit 1 byte of data
	if( HAL_SPI_Transmit(hspi, (uint8_t *)&EEPROM_WREN, 1, 100) != HAL_OK)
	{
		return HAL_SPI_STATE_ERROR;
	}
	// Set CS back to HIGH
	HAL_GPIO_WritePin(GPIOx, CS_Pin, GPIO_PIN_SET);

	// Read status register to see
	HAL_GPIO_WritePin(GPIOx, CS_Pin, GPIO_PIN_RESET);
	// Transmit Read Status Register Instruction which is 1 byte of data
	if ( HAL_SPI_Transmit(hspi, (uint8_t *)&EEPROM_RDSR, 1, 100) != HAL_OK)
	{
		return HAL_SPI_STATE_ERROR;
	}

	// Read 1 byte from the status register and parse to spi_buf
	// The Write-enable latch (WEL) bit in the status register should be 1, which means status = 0b00000010
	if ( HAL_SPI_Receive(hspi, (uint8_t *)stat_buf, 1, 100) != HAL_OK)
	{
		return HAL_SPI_STATE_ERROR;
	}
	// Check whether WEL bit has been set correctly
	if (stat_buf[0] != 0b00000010)
	{
		return HAL_SPI_STATE_ERROR;
	}
	// Set CS back to HIGH
	HAL_GPIO_WritePin(GPIOx, CS_Pin, GPIO_PIN_SET);

	// We will send all the write commands in one packet of 3 bytes
	// Buffer to store 3 bytes
	uint8_t header[3];
	header[0] = EEPROM_WRITE;    // Send "Write to Memory" instruction
	header[1] = addr >> 8;  // Send MSB of address, 8 bits
	header[2] = addr;	// Send LSB of address, 8 bits
	// Write the bytes starting at given address
	// Pull CS low
	HAL_GPIO_WritePin(GPIOx, CS_Pin, GPIO_PIN_RESET);
	// Transmit the address to write to the EEPROM
	if ( HAL_SPI_Transmit(hspi, (uint8_t *) &header, 3, 100) != HAL_OK)
	{
		return HAL_SPI_STATE_ERROR;
	}
	/*
	spi_buf[0] = 0xAA;
	spi_buf[1] = 0xBB;
	spi_buf[2] = 0xCC;
	*/
	// Transmit the data bytes
	if ( HAL_SPI_Transmit(hspi, (uint8_t *)spi_buf, size, 100) != HAL_OK)
	{
		return HAL_SPI_STATE_ERROR;
	}

	// Finally pull CS back to high
	HAL_GPIO_WritePin(GPIOx, CS_Pin, GPIO_PIN_SET);

	// Clear the SPI buffer
	spi_buf[0] = 0;
	spi_buf[1] = 0;
	spi_buf[2] = 0;

	// Wait until write operation is complete, this is a blocking function
	uint8_t wip = 1;
	while (wip)
	{
		// Read status register
		HAL_GPIO_WritePin(GPIOx, CS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(hspi, (uint8_t *)&EEPROM_RDSR, 1, 100);
		HAL_SPI_Receive(hspi, (uint8_t *)stat_buf, 1, 100);
		HAL_GPIO_WritePin(GPIOx, CS_Pin, GPIO_PIN_SET);

		// Mask out WIP bit
		wip = stat_buf[0] & 0b00000001;
	}

	return HAL_OK;
}

/**
* @brief  Read data from the 25AA080B, refer to data sheet for more details
* @param  GPIOx is the GPIO for the CS Pin, where x can be (A..K) to select the GPIO peripheral for STM32F429X device or
*                      x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices.
* @param  CS_Pin specifies the pin used as the Chip Select for the EEPROM.
*            This parameter can be one of GPIO_PIN_x where x can be (0..15).
* @param  addr is the starting address for the data to be read from
* @param  spi_buf is a data buffer to receive SPI data, e.g. char spi_buf[20]
* @param  size is the number of bytes in the you want to read starting from the address
* @param  hspi pointer to a SPI_HandleTypeDef structure that contains
*               the configuration information for SPI module.
* @retval HAL status
*/

HAL_StatusTypeDef eeprom_read(GPIO_TypeDef* GPIOx, uint16_t CS_Pin, uint16_t addr, char* spi_buf, uint16_t size, SPI_HandleTypeDef *hspi)
{
	// Read the bytes starting from addr, data will be stored in spi_buf
	HAL_GPIO_WritePin(GPIOx, CS_Pin, GPIO_PIN_RESET);
	// We will send all the read commands in one packet of 3 bytes
	// Buffer to store 3 bytes
	uint8_t header[3];
	header[0] = EEPROM_READ;    // Send "Read from Memory" instruction
	header[1] = addr >> 8;  // Send MSB of address, 8 bits
	header[2] = addr;	// Send LSB of address, 8 bits

	if (HAL_SPI_Transmit(hspi, (uint8_t *)&header, 3, 100) != HAL_OK)
	{
		return HAL_SPI_STATE_ERROR;
	}
	if (HAL_SPI_Receive(hspi, (uint8_t *)spi_buf, size, 100) != HAL_OK)
	{
		return HAL_SPI_STATE_ERROR;
	}
	// Set CS Pin back to HIGH
	HAL_GPIO_WritePin(GPIOx, CS_Pin, GPIO_PIN_SET);
	return HAL_OK;
}


