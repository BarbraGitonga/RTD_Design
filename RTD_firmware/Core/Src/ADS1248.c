/*
 * ADS1248.c
 *
 *  Created on: Oct 30, 2025
 *      Author: Barbra Gitonga
 */

#include "ADS1248.h"
#include <math.h>

/**
 * @brief Initializes the ADC by setting up the PGA,
 *      excitation current and read and write modes.
 * 
 * @param hspi  SPI handle
 * @param GPIO_START  GPIO port for START pin
 * @param PIN_START  GPIO pin for START pin
 * @param GPIO_CS GPIO port for CS pin
 * @param PIN_CS GPIO pin for CS pin
 */
void ADS124X_init(SPI_HandleTypeDef *hspi,
        GPIO_TypeDef *GPIO_START, uint16_t PIN_START,
        GPIO_TypeDef *GPIO_CS, uint16_t PIN_CS) {

	HAL_Delay(16);
	HAL_GPIO_WritePin(GPIO_START, PIN_START, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIO_CS, PIN_START, GPIO_PIN_RESET);
	HAL_Delay(1000);

	uint8_t resetcmd = 0x06;
	HAL_SPI_Transmit(hspi, &resetcmd, 1, HAL_MAX_DELAY);
	HAL_Delay(1);

	uint8_t SDATACcmd = 0x16;
	HAL_SPI_Transmit(hspi, &SDATACcmd, 1, HAL_MAX_DELAY);

	// configuring registers
	uint8_t wregData[] = {0x40, 0x03, 0x01, 0x00, 0x03, 0x42};
	HAL_SPI_Transmit(hspi, wregData, sizeof(wregData), HAL_MAX_DELAY);

	// Setting IDAC1 and IDAC2 to 1mA
	uint8_t wregIDAC[] = {
		0x4A,        // WREG starting at reg 0x0A
		0x01,        // write 2 registers (0x0A and 0x0B)
		0xC0,  		 // IDAC0 = 1 mA (bits 7-5 = 110)
		0x98   		 // IDAC1 = IDAC1→AIN2 (100), IDAC2→AIN3 (011)
	};
	HAL_SPI_Transmit(hspi, wregIDAC, sizeof(wregIDAC), HAL_MAX_DELAY);

	// Optional sanity check — RREG
	uint8_t rregCmd[] = {0x20, 0x03};
	uint8_t rregData[4];
	HAL_SPI_Transmit(hspi, rregCmd, sizeof(rregCmd), HAL_MAX_DELAY);
	HAL_SPI_Receive(hspi, rregData, 4, HAL_MAX_DELAY);

	// Start ADC conversion
	uint8_t synccmd = 0x04;
	HAL_SPI_Transmit(hspi, &synccmd, 1, HAL_MAX_DELAY);

	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIO_CS, PIN_CS, GPIO_PIN_SET);

}

/**
 * @brief Converts the resistance to temperature.
 * 
 * @param voltage value of voltage from resistance
 * @return int32_t the value of the temperature
 */
int32_t RTD_Converter(int32_t resistance){
	int32_t temp;

	// Temperatures above 0 degrees using pt100
	if((resistance >= pt100_R0) && (resistance <= 1000)){
		temp = (-A + sqrt((A * A) -(4 * B) *(1 - (resistance / pt100_R0)))) / (2 * B);
	}

	// Temperatures above 0 degrees using pt1000
	else if(resistance >= pt1000_R0){
		temp = (-A + sqrt((A * A) -(4 * B) *(1 - (resistance / pt1000_R0)))) / (2 * B);
	}

	else {
		temp = -999.0;  // error indicator
	}

	return temp;
}

int32_t Temperature(SPI_HandleTypeDef *hspi,
        GPIO_TypeDef *GPIO_CS, uint16_t PIN_CS){

	int8_t data[3];
	int32_t code;
	float Gain = 16;
	float R_ref = 1000.0f;
	float R_rtd;
	float temperature;

	HAL_GPIO_WritePin(GPIO_CS, PIN_CS, GPIO_PIN_RESET);
	HAL_Delay(1);

	uint8_t rdatacmd = 0x12;
	HAL_SPI_Transmit(hspi, &rdatacmd, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(hspi, &data, 3, HAL_MAX_DELAY);

	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIO_CS, PIN_CS, GPIO_PIN_SET);

	// Convert to integer (MSB)
	code = ((int32_t)data[0] << 16) | ((int32_t)data[1] << 8) | data[2]; // shifting data to occupy 42 bits

	// sign extension. check if MSB is 1, if 1 add -ve sign
	if (code & 0x800000) {
		code |= 0xFF000000; // sets all upper bits of 32 bits to 1. -ve value

	}

	// Calculate resistance from the output of the ADC
	R_rtd = R_ref * (code / (Gain * (pow(2, 23))));

	temperature = RTD_Converter(R_rtd);
	return temperature;
}
