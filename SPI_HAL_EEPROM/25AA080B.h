/*
 * 25AA080B.h
 *
 *  Created on: Jan 30, 2022
 *      Author: Vincent
 */

#ifndef INC_25AA080B_H_
#define INC_25AA080B_H_

#include "main.h"

// 25AA080B's instruction set
static const uint8_t EEPROM_READ = 0b00000011;
static const uint8_t EEPROM_WRITE = 0b00000010;
static const uint8_t EEPROM_WRDI = 0b00000100;
static const uint8_t EEPROM_WREN = 0b00000110;
static const uint8_t EEPROM_RDSR = 0b00000101;
static const uint8_t EEPROM_WRSR = 0b00000001;

// Function Declarations
HAL_StatusTypeDef eeprom_write(GPIO_TypeDef* GPIOx, uint16_t CS_Pin, uint16_t addr, char* spi_buf, uint16_t size, SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef eeprom_read(GPIO_TypeDef* GPIOx, uint16_t CS_Pin, uint16_t addr, char* spi_buf, uint16_t size, SPI_HandleTypeDef *hspi);
#endif /* INC_25AA080B_H_ */
