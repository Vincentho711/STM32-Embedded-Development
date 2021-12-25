/*
 * ds1631.h
 *
 *  Created on: Dec 24, 2021
 *      Author: Vincent
 */

#ifndef INC_DS1631_H_
#define INC_DS1631_H_

#include "main.h"

extern I2C_HandleTypeDef hi2c1;

// Addresses for DS1631
// DS1631's address
static const uint8_t DS1631_ADDR = 0x90;//Use 8-bit address
// DS1631's config reg address
static const uint8_t DS1631_CONFIG_ADDR = 0xAC;

// Function Declarations
HAL_StatusTypeDef DS1631_init(uint8_t dev_address, uint8_t config_reg_add);
HAL_StatusTypeDef DS1631_read_temp(uint8_t dev_address, uint8_t config_reg_add, float* temp);


#endif /* INC_DS1631_H_ */
