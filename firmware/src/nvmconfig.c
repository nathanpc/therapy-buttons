/**
 * nvmconfig.c
 * Non-volatile memory configuration abstraction.
 * 
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "nvmconfig.h"
#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdbool.h>

// Configuration variables in EEPROM.
uint8_t EEMEM config_eeprom_our_addr = 1;
int8_t EEMEM config_eeprom_clock_cal = 0;

// Configuration variables in SRAM.
static volatile uint8_t config_our_addr;
static volatile int8_t config_clock_cal;

/**
 * Initializes the configuration EEPROM stuff and reads all of the values that
 * we have in it.
 */
void Config_Initialize(void) {
	config_our_addr = eeprom_read_byte(&config_eeprom_our_addr);
	config_clock_cal = (int8_t)eeprom_read_byte(&config_eeprom_clock_cal);
}

/**
 * Gets our assigned bus address.
 * 
 * @return Bus address.
 */
uint8_t Config_GetOurAddress(void) {
	return config_our_addr;
}

/**
 * Sets our assigned bus address to the EEPROM.
 * 
 * @param addr Our new bus address.
 */
void Config_SetOurAddress(uint8_t addr) {
	eeprom_update_byte(&config_eeprom_our_addr, addr);
	config_our_addr = addr;
}

/**
 * Gets the clock calibration factor constant.
 * 
 * @return Clock calibration factor constant.
 */
int8_t Config_GetClockCalFactor(void) {
	return config_clock_cal;
}

/**
 * Sets the clock calibration factor constant.
 * 
 * @param factor Clock calibration factor constant.
 */
void Config_SetClockCalFactor(int8_t factor) {
	eeprom_update_byte(&config_eeprom_clock_cal, factor);
	config_clock_cal = factor;
}
