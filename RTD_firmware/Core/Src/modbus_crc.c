/*
 * modbus_crc.c
 *
 *  Created on: Nov 10, 2025
 *      Author: Barbra Gitonga(barbragitonga@gmail.com)
 */

#include"modbus_crc.h"

uint16_t Modbus_CRC16(uint8_t *data, uint16_t length)
{
    uint16_t crc = 0xFFFF;

    for (uint16_t pos = 0; pos < length; pos++) {
        crc ^= (uint16_t)data[pos]; // XOR first 8-bit binary data into crc

        for (uint8_t i = 0; i < 8; i++) {
            if (crc & 0x0001) {
                crc >>= 1; // shifting to the right by 1 bit
                crc ^= 0xA001; // Polynomial
            } else {
                crc >>= 1;
            }
        }
    }

    return crc;
}
