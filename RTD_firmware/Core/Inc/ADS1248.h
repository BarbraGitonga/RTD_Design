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
#define A  3.9083e-3
#define B -5.775e-7
#define C -4.183e-12

typedef struct {
	SPI_HandleTypeDef* hspi;
	GPIO_TypeDef* GPIO_START;
	uint16_t START_Pin;
	GPIO_TypeDef* GPIO_CS;
	uint16_t CS_Pin;
} ADS;

void ADS124X_init(ADS Pins);
int32_t Temperature(ADS Pins);
float RTD_Converter(int32_t result);

#endif /* INC_ADS1248_H_ */
