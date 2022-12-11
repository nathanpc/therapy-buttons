/*
 * File:   main.c
 * Author: nathanpc
 *
 * Created on June 13, 2022, 10:11 PM
 */

#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/xmega.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>
#include "pins.h"
#include "rtc.h"
#include "nvmconfig.h"
#include "uart.h"
#include "buscomm.h"
#include "pwm.h"
#include "strutils.h"

// Private macros.
#define IF_COMMAND(cmdstr)    if (strcmp(frame->command, (cmdstr)) == 0)
#define ELSIF_COMMAND(cmdstr) else IF_COMMAND(cmdstr)

// Private variables.
volatile rgb_t idle_color;
volatile rgb_t act_color;
volatile bool armed;
volatile bool announce_press;

// Private methods.
void Clock_Initialize(void);
void GPIO_Initialize(void);

/**
 * Program's main entry point.
 * 
 * @return Return code only used by the debugger.
 */
int main(void) {
	// Preamble.
	armed = 0;
    announce_press = 0;
	
	// Set things up.
	Clock_Initialize();
	GPIO_Initialize();
	RTC_Initialize(250, 500);
	Config_Initialize();
	PWM_Initialize();
	UART_Initialize(BUS_BAUD_RATE+300);
	Comms_Initialize(Config_GetOurAddress());
	sei();
	
	// Main application loop.
	while (1) {
		Comms_ParseFrame();
	}
	
	return 0;
}

/**
 * Handle a command issued to us.
 * 
 * @param frame Received frame to handle.
 */
void Comms_HandleCommand(const comms_frame_t *frame) {
	uint8_t tmp = 0;
	/*
	// Handle programming commands.
	if (!(PORTB.IN & PROG)) {
		IF_COMMAND("SETADDR") {
			// Sets our bus address.
			atou8(&tmp, frame->args[0]);
			Comms_SetOurAddress(tmp, true);
			Comms_AddrReply(Config_GetOurAddress(), "ADDRSET OK");
		
			return;
		}
	}
	*/
	IF_COMMAND("CLKCAL?") {
		// Gets the clock calibration factor.
		Comms_ReplyStart();
		UART_SendString("CLKCAL ");
		UART_SendInt8(Config_GetClockCalFactor());
		Comms_ReplyEnd();

		return;
	} ELSIF_COMMAND("SETCLKCAL") {
		// Sets the clock calibration factor.
		int8_t itmp = 0;
		atoi8(&itmp, frame->args[0]);
		Config_SetClockCalFactor(itmp);
		UART_Initialize(BUS_BAUD_RATE);

		Comms_ReplyStart();
		UART_SendString("CLKCAL ");
		UART_SendInt8(Config_GetClockCalFactor());
		Comms_ReplyEnd();

		return;
	} ELSIF_COMMAND("CLKCAL+") {
		// Increases our clock calibration factor.
		Config_SetClockCalFactor(Config_GetClockCalFactor() + 1);
		UART_Initialize(BUS_BAUD_RATE);

		Comms_ReplyStart();
		UART_SendString("CLKCAL ");
		UART_SendInt8(Config_GetClockCalFactor());
		Comms_ReplyEnd();

		return;
	} ELSIF_COMMAND("CLKCAL-") {
		// Increases our clock calibration factor.
		Config_SetClockCalFactor(Config_GetClockCalFactor() - 1);
		UART_Initialize(BUS_BAUD_RATE);

		Comms_ReplyStart();
		UART_SendString("CLKCAL ");
		UART_SendInt8(Config_GetClockCalFactor());
		Comms_ReplyEnd();

		return;
	} ELSIF_COMMAND("WBIDLCOLOR") {
		// Sets the idle color.
		atou8(&tmp, frame->args[0]);
		idle_color.r = tmp;
		atou8(&tmp, frame->args[1]);
		idle_color.g = tmp;
		atou8(&tmp, frame->args[2]);
		idle_color.b = tmp;
		
		if (!armed)
			PWM_SetColor(idle_color);
		
		if (frame->addr > 0)
			Comms_Reply("OK");
		
		return;
	} ELSIF_COMMAND("WBIDLCOLOR?") {
		// Gets the idle color.
		Comms_ReplyStart();
		UART_SendString("WBIDLCOLOR ");
		UART_SendUInt8(idle_color.r);
		UART_SendChar(' ');
		UART_SendUInt8(idle_color.g);
		UART_SendChar(' ');
		UART_SendUInt8(idle_color.b);
		Comms_ReplyEnd();
		
		return;
	} ELSIF_COMMAND("WBACTCOLOR") {
		// Sets the actuated color.
		atou8(&tmp, frame->args[0]);
		act_color.r = tmp;
		atou8(&tmp, frame->args[1]);
		act_color.g = tmp;
		atou8(&tmp, frame->args[2]);
		act_color.b = tmp;

		if (frame->addr > 0)
			Comms_Reply("OK");
		
		return;
	} ELSIF_COMMAND("WBACTCOLOR?") {
		// Gets the actuated color.
		Comms_ReplyStart();
		UART_SendString("WBACTCOLOR ");
		UART_SendUInt8(act_color.r);
		UART_SendChar(' ');
		UART_SendUInt8(act_color.g);
		UART_SendChar(' ');
		UART_SendUInt8(act_color.b);
		Comms_ReplyEnd();
		
		return;
	} ELSIF_COMMAND("WBARM") {
		// Arms the bomb.
		PWM_SetColor(act_color);
		armed = true;

		if (frame->addr > 0)
			Comms_Reply("OK");
		
		return;
	} ELSIF_COMMAND("WBARM?") {
		// Checks if the button is armed.
		Comms_ReplyStart();
		UART_SendString("WBARM ");
		UART_SendChar((armed) ? '1' : '0');
		Comms_ReplyEnd();
		
		return;
	} ELSIF_COMMAND("ANNCPRESS") {
		// Sets the announce pressed flag.
		atou8(&tmp, frame->args[0]);
		announce_press = tmp;
		
		if (frame->addr > 0)
			Comms_Reply("OK");
		
		return;
	} ELSIF_COMMAND("ANNCPRESS?") {
		// Gets the announce pressed flag.
		Comms_ReplyStart();
		UART_SendString("ANNCPRESS ");
		UART_SendUInt8(announce_press);
		Comms_ReplyEnd();
		
		return;
	} ELSIF_COMMAND("PRESSED?") {
		// Checks if the button is pressed.
		Comms_ReplyStart();
		UART_SendString("PRESSED ");
		UART_SendChar((!(PORTC.IN & WALL_SW)) ? '1' : '0');
		Comms_ReplyEnd();
		
		return;
	} ELSIF_COMMAND("WHAT?") {
		// What are we?
		Comms_AddrReply(Config_GetOurAddress(), "WALLBUTTON");
		return;
	} else {
		// Not a valid command for this module.
		if (frame->addr > 0) {
			Comms_ReplyStart();
			UART_SendString("INVCMD \"");
			UART_SendString(frame->command);
			UART_SendChar('"');
			Comms_ReplyEnd();
		}
		
		return;
	}
	
	// Ooops.
	if (frame->addr > 0) {
		Comms_ReplyStart();
		UART_SendString("GENERR \"");
		UART_SendString(frame->command);
		UART_SendChar('"');
		Comms_ReplyEnd();
	}
}

/**
 * Interrupt service routine that's called when an RTC PIT period has completed.
 */
ISR(RTC_PIT_vect) {
	// Clear the interrupt flag.
	RTC.PITINTFLAGS = RTC_PI_bm;
}

/**
 * Interrupt service routine that's called when an RTC compare or period has
 * completed.
 */
ISR(RTC_CNT_vect) {
	// Das blinkenlights.
    if (RTC.INTFLAGS & RTC_CMP_bm)
		PORTC.OUTTGL = STATUS_LED;
	
	// Clear the interrupt flag.
    RTC.INTFLAGS = (RTC_OVF_bm | RTC_CMP_bm);
}

/**
 * Interrupt service routine that's called when an UART transmission has ended.
 */
ISR(USART0_TXC_vect) {
	if (USART0.STATUS & USART_DREIF_bm)
		PORTB.OUTCLR = TX_EN;         // Put the RS-485 transceiver in RX mode.

	USART0.STATUS |= USART_TXCIF_bm;  // Clear the interrupt flag.
}

/**
 * Interrupt service routine that's called when we receive something via UART.
 */
ISR(USART0_RXC_vect) {
	while (USART0.RXDATAH & USART_RXCIF_bm) {
		// Check for errors.
		if (USART0.RXDATAH & (USART_BUFOVF_bm | USART_FERR_bm | USART_PERR_bm)) {
			// Read the registers to clear the BUFOVF flag.
			USART0.RXDATAH;
			USART0.RXDATAL;
			
			// Discard the frame.
			Comms_ResetRXBuffer();
			continue;
		}

		// Handle the received character.
		Comms_ReceiveChar((char)USART0.RXDATAL);
	}
}

/**
 * Interrupt service routine that's called when a pin in PORTC changes.
 */
ISR(PORTC_PORT_vect) {
    // Announce that the button was pressed.
    if (announce_press) {
        char reply[12];

        // Send back our reply.
        strcomb(reply, "TRIGD ", Comms_GetAddrStr());
        Comms_AddrReply(0, reply);
    }

	// Check if we are armed.
	if (armed) {
		// Reset the button state.
		PWM_SetColor(idle_color);
		armed = false;
	}
	
	// Clear the interrupt flag.
	PORTC.INTFLAGS |= WALL_SW;
}

/**
 * Initializes the clock and things related to it.
 */
void Clock_Initialize(void) {
	// No prescaler. Give us the full 20MHz.
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0);
	
	// Run the RTC clock in all modes.
	_PROTECTED_WRITE(CLKCTRL.OSC32KCTRLA, CLKCTRL_RUNSTDBY_bm);
}

/**
 * Initializes the port directions and pin's standard level.
 */
void GPIO_Initialize(void) {
	// Setup the outputs.
	PORTA.DIRSET = (PWM_B | PWM_W);
	PORTB.DIRSET = (PWM_R | PWM_G | TXD | TX_EN);
	PORTC.DIRSET = (STATUS_LED);
	
	// Setup the input interrupt for the button.
	PORTC.PIN2CTRL = PORT_ISC_FALLING_gc;  // Trigger on the falling edge.
}