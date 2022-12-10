/**
 * global_pins.h
 * Project-wide pin definitions and names.
 * 
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef GLOBAL_PINS_H
#define	GLOBAL_PINS_H

#ifdef	__cplusplus
extern "C" {
#endif

// PORTA
#define PWM_B _BV(4)
#define PWM_W _BV(5)

// PORTB
#define PWM_R _BV(0)
#define PWM_G _BV(1)
#define TXD   _BV(2)
#define RXD   _BV(3)
#define TX_EN _BV(4)
#define PROG  _BV(5)

// PORTC
#define STATUS_LED _BV(0)

#ifdef	__cplusplus
}
#endif

#endif	/* GLOBAL_PINS_H */

