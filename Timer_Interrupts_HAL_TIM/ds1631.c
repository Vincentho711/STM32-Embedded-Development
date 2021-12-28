/*
 * ds1631.c
 *
 *  Created on: Dec 24, 2021
 *      Author: Vincent
 */
#include "ds1631.h"

// Set up to run on hi2c1

/**
* @brief  Initialise DS1631, remember to set up pull up resistors for I2C
* @param  dev_address DS1631 address set by A0,A1, A2 pins
* 				datasheet provides 8bit address, therefore no need
* 				to left shift in i2c functions (When A0,A1,A2 = 0,0,0 , ADDR = 0x90 (0x48 <<1))
* @param  config_reg_add configuration register for DS1631 (size 8b)
* 				configuration register address is 0xAC
* @retval HAL status
*/

HAL_StatusTypeDef DS1631_init(uint8_t dev_address, uint8_t config_reg_add)
{
	HAL_StatusTypeDef ret;

	// Create a buffer with 16 8-bit storage
	uint8_t buffer[16];

	// Set byte to contain data to write to config register
	// Write 0x02 to set continuous temperature conversion
	buffer[0] = 0x02;

	// Write to the config register to set up continuous conversion mode
	ret = HAL_I2C_Mem_Write(&hi2c1, dev_address, config_reg_add, 1, buffer, 1, 50);
	if (ret != HAL_OK)
	{
		return ret;
	}

	// Issue a "Start convert T command"
	// Write command byte as 0x51 to data buffer and transmit
	buffer[0]  = 0x51;
	ret = HAL_I2C_Master_Transmit(&hi2c1, dev_address, buffer, 1, 50);
	if (ret != HAL_OK)
	{
		return ret;
	}

	return HAL_OK;
}


/**
* @brief  Read temperature from DS1631, remember to set up pull up resistors for I2C
* @param  dev_address DS1631 address set by A0,A1, A2 pins
* 				datasheet provides 8bit address, therefore no need
* 				to left shift in i2c functions (default=000 or 0x90 = (0x48 <<1))
* @param  config_reg_add configuration register for DS1631 (size 8b)
* 				configuration register address is 0xAC
* @param	temp pointer to target memory for temperature data
* @retval HAL status
*/
HAL_StatusTypeDef DS1631_read_temp(uint8_t dev_address, uint8_t config_reg_add, float* temp)
{
	HAL_StatusTypeDef ret;

	// DS1631's command to read temperature from register
	static const uint8_t TEMP_READ_COMMAND = 0xAA;
	// Temporary variable to store 2 byte temperature
	uint8_t tempData[2];

	// Read temperature from register
	ret = HAL_I2C_Mem_Read(&hi2c1, (dev_address|0x01), TEMP_READ_COMMAND, 1, tempData, 2, 50);
	if (ret != HAL_OK)
	{
		return ret;
	}

	// Convert bytes to temperature
	// The second byte buffer[1] stores the decimal place, right shift it to get
	// rid of the 4 LSBs, convert it to decimal, each int represents 0.0625C
	float decimal = ((int8_t)tempData[1]>>4)*0.0625;

	float integer = 0.0f;
	// Check if the integer part is positive or negative value
	if ((tempData[0] & 0x80) == 0)
	{
		// The integer part of the temperature is just represented by the T_H byte
		integer = ((int8_t)tempData[0]);
	}else
	{
		// If it is negative in 2's complement form, convert it back to positive
		// by subtracting 1 and flipping all the bits
		integer = (int8_t)(~(tempData[0] - 0x01)) * -1;
	}

	*temp = integer + decimal;

	return HAL_OK;
}


