/**
 * strutils.c
 * A whole bunch of extremely useful utility functions to deal with strings.
 * 
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "strutils.h"
#include <stdlib.h>

// Private definitions.
#define ASCII_0 '0'
#define ASCII_9 '9'

/**
 * Converts the entirety or part of a string into a uint8_t. This function will
 * stop whenever it reaches the NULL terminator or any character that isn't a
 * number.
 * 
 * @param  n   Pointer to store the parsed number at.
 * @param  buf String to go through and parse a number from.
 * @return     Number of characters read from the string or -1 for error.
 */
int8_t atou8(uint8_t *n, const char *buf) {
	char ch;
	uint8_t count = 0;
	*n = 0;
	
	// Go through the string.
	ch = buf[count];
	while ((ch >= ASCII_0) && (ch <= ASCII_9)) {
		// Check if we are dealing with a greater than we can possibly handle.
		if (count == 3) {
			*n = 0;
			return -1;
		}
		
		// Sum up!
		*n = (*n * 10) + (ch - ASCII_0);
		
		// Go to the next character.
		ch = buf[++count];
	}
	
	return count;
}

/**
 * Converts the entirety or part of a string into a int8_t. This function will
 * stop whenever it reaches the NULL terminator or any character that isn't a
 * number.
 * 
 * @param  n   Pointer to store the parsed number at.
 * @param  buf String to go through and parse a number from.
 * @return     Number of characters read from the string or -1 for error.
 */
int8_t atoi8(int8_t *n, const char *buf) {
	const char *tmp;
	uint8_t count = 0;
	tmp = buf;
	
	// Check if we have a negative number. Ignore it to parse the absolute part.
	if (*buf == '-') {
		count++;
		tmp++;
	}
	
	// Parse the absolute number.
	atou8(n, tmp);
	
	// Should we make it negative?
	if (*buf == '-')
		*n *= -1;
	
	return count;
}

/**
 * Converts a uint8_t to a string.
 * 
 * @param buf String big enough to be filled with the number.
 * @param n   Number to be converted.
 */
void u8toa(char *buf, uint8_t n) {
	char *tmp = buf;
	uint8_t i = 0;
	uint8_t p = 0;
	
	// Short-circuit for good measure.
	if (n == 0) {
		*tmp++ = '0';
		*tmp = '\0';
		return;
	}
	
	// Go through the decades converting each digit into a character.
	for (uint8_t d = 100; d > 0; d /= 10) {
		i = (n - p) / d;
		
		if ((i > 0) || (p > 0)) {
			*tmp++ = i + ASCII_0;
			p += i * d;
		}
	}
	
	// Terminate the string.
	*tmp = '\0';
}

/**
 * Converts a int8_t to a string.
 * 
 * @param buf String big enough to be filled with the number and its sign.
 * @param n   Number to be converted.
 */
void i8toa(char *buf, int8_t n) {
	char *tmp = buf;
	
	// Check if we are dealing with a negative number.
	if (n < 0)
		*tmp++ = '-';
	
	// Pass it over.
	u8toa(tmp, (uint8_t)abs(n));
}

/**
 * Combines two strings into an independent buffer.
 * 
 * @param buf  Buffer large enough to receive the final string.
 * @param str1 First string to be appended to the buffer.
 * @param str2 Second string to be appended to the buffer.
 */
void strcomb(char *buf, const char *str1, const char *str2) {
	char *tmp = buf;
	const char *ctmp;
	
	// Append the first string.
	ctmp = str1;
	while (*ctmp != '\0')
		*tmp++ = *ctmp++;
	
	// Append the second string.
	ctmp = str2;
	while (*ctmp != '\0')
		*tmp++ = *ctmp++;
	
	// Properly terminate the buffer.
	*tmp = '\0';
}
