/**
 * uart.h
 * Abstraction layer for serial communications.
 * 
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <inttypes.h>
	
// Initialization
void UART_Initialize(uint16_t baud);

// Transmission
void UART_SendByte(uint8_t b);
void UART_SendChar(char c);
void UART_SendString(const char *str);
void UART_SendLine(const char *str);

// Numeric Transmissions
void UART_SendInt8(int8_t n);
void UART_SendUInt8(uint8_t n);
	
#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

