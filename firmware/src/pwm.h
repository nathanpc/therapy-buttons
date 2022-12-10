/**
 * pwm.h
 * PWM abstraction layer.
 * 
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef PWM_H
#define	PWM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <inttypes.h>

// RGB color structure.
typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} rgb_t;

// RGBW color structure.
typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t w;
} rgbw_t;
	
// Initialization
void PWM_Initialize(void);

// Channels
void PWM_SetRed(uint8_t value);
void PWM_SetGreen(uint8_t value);
void PWM_SetBlue(uint8_t value);
void PWM_SetWhite(uint8_t value);
void PWM_SetRGB(uint8_t r, uint8_t g, uint8_t b);
void PWM_SetRGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void PWM_SetColor(rgb_t color);
void PWM_SetColorW(rgbw_t color);

#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

