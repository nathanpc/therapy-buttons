/**
 * buscomm.c
 * Bus communication abstraction layer.
 * 
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "buscomm.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "uart.h"
#include "strutils.h"
#include "nvmconfig.h"

// Private variables.
static volatile char comms_rcv_buf[FRAME_MAX_LEN];
static volatile comms_frame_t comms_rcv_frame;
static volatile uint8_t comms_ch_rcvd;
static volatile bool comms_frame_parse_rdy;
static volatile char comms_our_addr_str[4];

/**
 * Initializes the bus communication stuff.
 * 
 * @param addr Our own address.
 */
void Comms_Initialize(uint8_t addr) {
	Comms_SetOurAddress(addr, false);
}

/**
 * Parses the received frame and takes action on it if needed.
 */
void Comms_ParseFrame(void) {
	const char *buf;
	char *tmp;
	int8_t skip = 0;
	
	// Do nothing if we don't have a frame to parse.
	if (!comms_frame_parse_rdy) {
		// Check if we ran into an exception while receiving data.
		if (comms_ch_rcvd >= (FRAME_MAX_LEN - 1))
			goto finish;
		
		return;
	}
	
	// Set some things beforehand.
	comms_rcv_frame.num_args = 0;
	buf = comms_rcv_buf;

	// Check if we've received a frame from the master.
	if (*buf++ != ':')
		goto finish;
	
	// Parse the address.
	skip = atou8(&comms_rcv_frame.addr, buf);
	buf += skip;
	if (*buf++ != ' ')
		goto finish;
	
	// Parse the command.
	tmp = &comms_rcv_frame.command;
	while ((*buf != ' ') && (*buf != '\r')) {
		*tmp++ = *buf;
		buf++;
	}
	*tmp = '\0';
	buf++;

	// Parse the arguments.
	while ((*buf != '\r') && (*buf != '\n')) {
		// Check if we've overflowed the number of arguments allowed.
		if (comms_rcv_frame.num_args == ARGS_MAX)
			goto finish;
		
		// Parse a new argument.
		tmp = &comms_rcv_frame.args[comms_rcv_frame.num_args];
		while ((*buf != ' ') && (*buf != '\r')) {
			*tmp++ = *buf;
			buf++;
		}
		*tmp = '\0';
		buf++;
		
		// Increase the argument counter.
		comms_rcv_frame.num_args++;
	}
	
	// Is this message for us? Should we handle it?
	if ((comms_rcv_frame.addr == 0) ||
			(comms_rcv_frame.addr == Config_GetOurAddress())) {
		Comms_HandleCommand(&comms_rcv_frame);
	}

finish:
	// Get ready to receive more shit.
	Comms_ResetRXBuffer();
}

/**
 * Send a reply with an address.
 * 
 * @param addr  Address to be used on the reply.
 * @param reply Reply message.
 */
void Comms_AddrReply(uint8_t addr, const char *reply) {
	Comms_AddrReplyStart(addr);
	UART_SendString(reply);
	Comms_ReplyEnd();
}

/**
 * Starts a reply with an address.
 * 
 * @param addr Address to be used on the reply.
 */
void Comms_AddrReplyStart(uint8_t addr) {
	char nch[4];
	// TODO: Make sure to check if we are not receiving anything.
	
	// Send reply marker.
	UART_SendChar(';');
	
	// Our address and the separator.
	u8toa(nch, addr);
	UART_SendString(nch);
	UART_SendChar(' ');
}

/**
 * Send back a reply to the master.
 * 
 * @param reply Reply message.
 */
void Comms_Reply(const char *reply) {
	Comms_AddrReply(comms_rcv_frame.addr, reply);
}

/**
 * Starts a reply to the master.
 */
void Comms_ReplyStart(void) {
	Comms_AddrReplyStart(comms_rcv_frame.addr);
}

/**
 * Ends a reply to the master.
 */
void Comms_ReplyEnd(void) {
	UART_SendString("\r\n");
}

/**
 * Handles the event of a character on the bus being received.
 * 
 * @param c Received character.
 */
void Comms_ReceiveChar(char c) {
	// Wait for the start of a frame.
	if ((comms_ch_rcvd == 0) && (c != ':'))
		return;
	
	// Append the received character to our buffer.
	comms_rcv_buf[comms_ch_rcvd++] = c;
	
	// Looks like a frame is ready for parsing.
	if (c == '\n') {
		comms_rcv_buf[comms_ch_rcvd] = '\0';
		comms_frame_parse_rdy = true;
	}
}

/**
 * Something bad happened, so let's just completely discard any stored frames.
 */
void Comms_DiscardFrame(void) {
	// Discard the parsed frame.
	comms_rcv_frame.addr = 0;
	comms_rcv_frame.command[0] = '\0';
	comms_rcv_frame.num_args = 0;
	
	// Reset the character counter and frame string buffer.
	Comms_ResetRXBuffer();
}

/**
 * Resets the receive buffer stuff.
 */
void Comms_ResetRXBuffer(void) {
	comms_rcv_buf[0] = '\0';
	comms_ch_rcvd = 0;
	comms_frame_parse_rdy = false;
}

/**
 * Sets our bus address immediately and also saves the new address to the
 * EEPROM.
 * 
 * @param addr    Our new bus address.
 * @param persist Save the new address to the EEPROM?
 */
void Comms_SetOurAddress(uint8_t addr, bool persist) {
	// Persist this change?
	if (persist)
		Config_SetOurAddress(addr);
	
	// Set the string version of the address and clean up any received frames.
	u8toa(comms_our_addr_str, addr);
	Comms_DiscardFrame();
}

/**
 * Gets our bus address as a string.
 * 
 * @return Our bus address as a string.
 */
const char* Comms_GetAddrStr(void) {
	return comms_our_addr_str;
}

/**
 * Prints back a debug representation of the received frame.
 */
void Comms_DebugPrintFrame(void) {
	char nch[4];
	
	// Address
	UART_SendString("Address: ");
	u8toa(nch, comms_rcv_frame.addr);
	UART_SendString(nch);
	UART_SendString("\r\n");
	
	// Command
	UART_SendString("Command: '");
	UART_SendString(comms_rcv_frame.command);
	UART_SendLine("'");
	
	// Arguments
	UART_SendString("Arguments [");
	u8toa(nch, comms_rcv_frame.num_args);
	UART_SendString(nch);
	UART_SendLine("]:");
	for (uint8_t i = 0; i < comms_rcv_frame.num_args; i++) {
		UART_SendString("  [");
		u8toa(nch, i);
		UART_SendString(nch);
		UART_SendString("] '");
		UART_SendString(comms_rcv_frame.args[i]);
		UART_SendLine("'");
	}
	
	UART_SendString("\r\n");
}
