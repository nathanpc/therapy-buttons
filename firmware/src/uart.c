/**
 * uart.c
 * Abstraction layer for serial communications.
 * 
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "uart.h"
#include "global_pins.h"
#include "strutils.h"
#include "nvmconfig.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Private definitions.
#define BAUDRATER(BAUD_RATE) ((float)((float)F_CPU * 64 /(16 *(float)BAUD_RATE))+ 0.5)

/**
 * Sets up the UART peripheral for communication.
 * 
 * @param baud Desired baud rate.
 */
void UART_Initialize(uint16_t baud) {
	// Disable interrupts while we set things up.
	cli();
	
	PORTB.OUTCLR = TX_EN;                            // Make sure RS-485 bus is set to RX.
	USART0.BAUD  = (uint16_t)((int32_t)BAUDRATER(baud) *
			(1024 + (int8_t)SIGROW.OSC20ERR5V) / 1024) + Config_GetClockCalFactor();
	USART0.CTRLA = USART_RXCIE_bm | USART_TXCIE_bm;  // Enable the TX and RX interrupt.
	USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;    // Enable transmitter and receiver.
	
	// Enable interrupts again.
	sei();
}

/**
 * Sends a byte via UART.
 * 
 * @param b Byte to be sent.
 */
void UART_SendByte(uint8_t b) {
	// Wait for the TX buffer to be ready for more data.
	while (!(USART0.STATUS & USART_DREIF_bm))
		;
	
	// Send byte.
	PORTB.OUTSET = TX_EN;
	USART0.TXDATAL = b;
}

/**
 * Sends a character via UART.
 * 
 * @param c Character to be sent.
 */
void UART_SendChar(char c) {
	// Wait for the TX buffer to be ready for more data.
	while (!(USART0.STATUS & USART_DREIF_bm))
		;
	
	// Send byte.
	PORTB.OUTSET = TX_EN;
	USART0.TXDATAL = (uint8_t)c;
}

/**
 * Sends a whole string via UART.
 * 
 * @param str String to be sent.
 */
void UART_SendString(const char *str) {
	const char *tmp = str;
	
	while (*tmp) {
		// Wait for the TX buffer to be ready for more data.
		while (!(USART0.STATUS & USART_DREIF_bm))
			;

		// Send data.
		PORTB.OUTSET = TX_EN;
		USART0.TXDATAL = (uint8_t)*tmp++;
	}
}

/**
 * Sends an int8_t as s string via UART.
 * 
 * @param n Number to be set.
 */
void UART_SendInt8(int8_t n) {
	char buf[5];
	
	i8toa(buf, n);
	UART_SendString(buf);
}

/**
 * Sends an uint8_t as s string via UART.
 * 
 * @param n Number to be set.
 */
void UART_SendUInt8(uint8_t n) {
	char buf[4];
	
	u8toa(buf, n);
	UART_SendString(buf);
}

/**
 * Sends a whole string with a CRLF at the end via UART.
 * 
 * @param str String to be sent.
 */
void UART_SendLine(const char *str) {
	const char *tmp = str;
	
	// Send string.
	while (*tmp) {
		// Wait for the TX buffer to be ready for more data.
		while (!(USART0.STATUS & USART_DREIF_bm))
			;

		// Send data.
		PORTB.OUTSET = TX_EN;
		USART0.TXDATAL = (uint8_t)*tmp++;
	}
	
	// Send CRLF.
	while (!(USART0.STATUS & USART_DREIF_bm))
		;
	PORTB.OUTSET = TX_EN;
	USART0.TXDATAL = (uint8_t)'\r';
	while (!(USART0.STATUS & USART_DREIF_bm))
		;
	PORTB.OUTSET = TX_EN;
	USART0.TXDATAL = (uint8_t)'\n';
}
