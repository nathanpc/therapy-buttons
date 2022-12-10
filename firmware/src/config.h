/**
 * config.h
 * Configuration options for this project.
 * 
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <avr/io.h>

FUSES = {
	.WDTCFG = 0x00, // WDTCFG {PERIOD=OFF, WINDOW=OFF}
	.BODCFG = 0x00, // BODCFG {SLEEP=DIS, ACTIVE=DIS, SAMPFREQ=1KHZ, LVL=BODLEVEL0}+
	.OSCCFG = 0x7E, // OSCCFG {FREQSEL=20MHZ, OSCLOCK=CLEAR}
	.SYSCFG0 = 0xF6, // SYSCFG0 {EESAVE=CLEAR, RSTPINCFG=UPDI, CRCSRC=NOCRC}
	.SYSCFG1 = 0xFF, // SYSCFG1 {SUT=64MS}
	.APPEND = 0x00, // APPEND {APPEND=User range:  0x0 - 0xFF}
	.BOOTEND = 0x00, // BOOTEND {BOOTEND=User range:  0x0 - 0xFF}
};

LOCKBITS = 0xC5; // {LB=NOLOCK}

// Just making sure.
#ifndef F_CPU
#error "F_CPU not defined"
#endif

// Bus default baud rate.
//#define BUS_BAUD_RATE 38400
#define BUS_BAUD_RATE 9600

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

