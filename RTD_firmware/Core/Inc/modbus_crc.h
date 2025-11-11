/*
 * modbus_crc.h
 *
 *  Created on: Nov 10, 2025
 *      Author: Barbra Gitonga (barbragitonga@gmail.com)
 */

#ifndef INC_MODBUS_CRC_H_
#define INC_MODBUS_CRC_H_

#include <stdio.h>
#include <stdint.h>

uint16_t Modbus_CRC16(uint8_t *data, uint16_t len);

#endif /* INC_MODBUS_CRC_H_ */
