/**
 * nvmconfig.c
 * Non-volatile memory configuration abstraction.
 * 
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef NVMCONFIG_H
#define	NVMCONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <inttypes.h>
	
// Initialization
void Config_Initialize(void);

// Getters and Setters
uint8_t Config_GetOurAddress(void);
void Config_SetOurAddress(uint8_t addr);
int8_t Config_GetClockCalFactor(void);
void Config_SetClockCalFactor(int8_t factor);

#ifdef	__cplusplus
}
#endif

#endif	/* NVMCONFIG_H */

