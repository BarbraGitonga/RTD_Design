/*
 * modbus.c
 *
 *  Created on: Nov 10, 2025
 *      Author: Barbra Gitonga (barbragitonga@gmail.com)
 */

#include "modbus.h"
#include "modbus_crc.h"

uint8_t ProcessModbusFrame(uint8_t *rx, uint16_t len, uint8_t *response, uint16_t* mb_input_reg){
	if(len < 8){
		return ILLEGAL_DATA_ADDRESS; // too short
	}

	if (rx[1] != 0x04){
		return ILLEGAL_FUNCTION;
	}

	if(((rx[2] << 8) | rx[3]) != 0){
		return ILLEGAL_DATA_ADDRESS;
	}
	if(((rx[4] << 8) | rx[5]) != 1){
		return ILLEGAL_DATA_VALUE;
	}
	// check CRC
	uint16_t crcCalc = Modbus_CRC16(rx, len-2);
	uint16_t crcRecv = (rx[len - 1] << 8) | rx[len - 2];

	if (crcCalc != crcRecv){
		return ILLEGAL_DATA_VALUE;
	}
	// Handle "Read Input registers" (0x04)
	if((rx[0] == SLAVE_ADDR) && (rx[1] == 0x04)) {
		uint16_t temp_val = mb_input_reg[0];

		response[0] = SLAVE_ADDR;
		response[1] = 0x04; // function code
		response[2] = 0x02; // 2 bytes follow (number of bytes)
		response[3] = (temp_val >> 8) & 0xFF; // High byte first
		response[4] = temp_val & 0xFF; // low byte follows
		uint16_t crc = Modbus_CRC16(response, 5);
		response[5] = crc & 0xFF; // low byte
		response[6] = crc >> 8; // high byte
	}
	return 0;
}

void Modbus_SendException(uint8_t function, uint8_t* response, uint8_t exceptionCode){
	response[0] = SLAVE_ADDR;
	response[1] = function | 0x08; // Function code
	response[2] = exceptionCode; // exception code

	uint16_t crc = Modbus_CRC16(response, 3);
	response[3] = crc & 0xFF; // crc low
	response[4] = crc >> 8; // crc high
}
