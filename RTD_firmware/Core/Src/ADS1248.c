/*
 * ADS1248.c
 *
 *  Created on: Oct 30, 2025
 *      Author: Barbra Gitonga
 */

#include "ADS1248.h"
#include <math.h>

void ADS124X_init(ADS Pins){
	HAL_Delay(16);
	HAL_GPIO_WritePin(Pins->GPIO_START,Pins->START_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(Pins->GPIO_CS, Pins->CS_Pin, GPIO_PIN_RESET);
	HAL_Delay(1000);

	uint8_t resetcmd = 0x06;
	HAL_SPI_Transmit(Pins->hspi, &resetcmd, 1, HAL_MAX_DELAY);
	HAL_Delay(1);

	uint8_t SDATACcmd = 0x16;
	HAL_SPI_Transmit(Pins->hspi, &resetcmd, 1, HAL_MAX_DELAY);

	// configuring registers
	uint8_t wregData[] = {0x40, 0x03, 0x01, 0x00, 0x03, 0x42};
	HAL_SPI_Transmit(Pins->hspi, wregData, sizeof(wregData), HAL_MAX_DELAY);

	// Optional sanity check — RREG
	uint8_t rregCmd[] = {0x20, 0x03};
	uint8_t rregData[4];
	HAL_SPI_Transmit(&hspi1, rregCmd, sizeof(rregCmd), HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, rregData, 4, HAL_MAX_DELAY);

	// Start ADC conversion
	uint8_t synccmd = 0x04;
	HAL_SPI_Transmit(Pins->hspi, &synccmd, 1, HAL_MAX_DELAY);

	HAL_Delay(1);
	HAL_GPIO_WritePin(Pins->GPIO_CS, Pins->CS_Pin, GPIO_PIN_SET);

}

float RTD_Converter(int32_t resistance){
	float temp;

	// Temperatures above 0 degrees using pt100
	if((resistance >= pt100_R0) && (resistance <= 1000)){
		temp = (-A + sqrt((A * A) -(4 * B) *(1 - (resistance / pt100_R0)))) / (2 * B);
	}

	// Temperatures above 0 degrees using pt100
	else if(resistance >= pt1000_R0){
		temp = (-A + sqrt((A * A) -(4 * B) *(1 - (resistance / pt1000_R0)))) / (2 * B);
	}

	else {
		temp = -999.0;  // error indicator
	}

	return temp;
}

int32_t Temperature(ADS Pins){
	int8_t data[3];
	int32_t resistance;
	float temperature;

	HAL_GPIO_WritePin(Pins->GPIO_CS, Pins->CS_Pin, GPIO_PIN_RESET);
	HAL_Delay();

	uint8_t rdatacmd = 0x12;
	HAL_SPI_Transmit(Pins->hspi, &rdatacmd, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(Pins->hspi, data, 3, HAL_MAX_DELAY);

	HAL_Delay(1);
	HAL_GPIO_WritePin(Pins->GPIO_CS, Pins->CS_Pin, GPIO_PIN_SET);

	// Convert to integer (MSB)
	resistance = ((int32_t)data[0] << 16) | ((int32_t)data[1] << 8) | data[2];
	if (resistance & 0x800000) {
		resistance |= 0xFF000000;

	}
	temperature = RTD_Converter(resistance);
	return temperature;
}
