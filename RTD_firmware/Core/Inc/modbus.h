/*
 * modbus.h
 *
 *  Created on: Nov 10, 2025
 *      Author: Barbra Gitonga (barbragitonga@gmail.com)
 */

#ifndef INC_MODBUS_H_
#define INC_MODBUS_H_

#include "stm32f1xx_hal.h"

#define SLAVE_ADDR			   0x01 // This is the slave ID of the device

#define ILLEGAL_FUNCTION       0x01
#define ILLEGAL_DATA_ADDRESS   0x02
#define ILLEGAL_DATA_VALUE     0x03
#define CRC_ERROR     		   0x04

void RS485_SetTransmit(void);
void RS485_SetReceive(void);
uint8_t ProcessModbusFrame(uint8_t *rx, uint16_t len, uint8_t *response, uint16_t* mb_input_reg);
void Modbus_SendException(uint8_t function, uint8_t *response, uint8_t exceptionCode);

#endif /* INC_MODBUS_H_ */
