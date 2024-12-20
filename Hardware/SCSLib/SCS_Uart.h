/*
 * SCS_Uart.h
 *
 *  Created on: Nov 1, 2024
 *      Author: Taolin
 */

#ifndef SCSLIB_SCS_UART_H_
#define SCSLIB_SCS_UART_H_

#include <stdint.h>
extern void Uart_Init(uint32_t baudRate);
extern int16_t Uart_Read(uint8_t *buf, uint16_t len, uint32_t timeout);
extern void Uart_Send(uint8_t *buf, uint16_t len);

#endif /* SCSLIB_SCS_UART_H_ */
