/**
 * strutils.h
 * A whole bunch of extremely useful utility functions to deal with strings.
 * 
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef STRUTILS_H
#define	STRUTILS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <inttypes.h>

// Conversions
int8_t atou8(uint8_t *n, const char *buf);
int8_t atoi8(int8_t *n, const char *buf);
void u8toa(char *buf, uint8_t n);
void i8toa(char *buf, int8_t n);

// String Manipulation
void strcomb(char *buf, const char *str1, const char *str2);

#ifdef	__cplusplus
}
#endif

#endif	/* STRUTILS_H */

