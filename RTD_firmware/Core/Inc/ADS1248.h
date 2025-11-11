/*
 * ADS1248.h
 *
 *  Created on: Oct 30, 2025
 *      Author: Barbra Gitonga (barbragitonga@gmail.com)
 */

#ifndef INC_ADS1248_H_
#define INC_ADS1248_H_

#include "stm32f1xx_hal.h"

#define pt100_R0 100.0
#define pt1000_R0 1000.0
#define A  3.908e-3
#define B -5.775e-7


void ADS124X_init(SPI_HandleTypeDef *hspi,
        GPIO_TypeDef *GPIO_START, uint16_t PIN_START,
        GPIO_TypeDef *GPIO_CS, uint16_t PIN_CS);

int32_t Temperature(SPI_HandleTypeDef *hspi,
        GPIO_TypeDef *GPIO_CS, uint16_t PIN_CS);

int32_t RTD_Converter(int32_t resistance);

#endif /* INC_ADS1248_H_ */
