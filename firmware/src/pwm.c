/**
 * pwm.c
 * PWM abstraction layer.
 * 
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "pwm.h"
#include "global_pins.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/**
 * Initializes the PWM peripheral and gets everything ready for das blinken
 * lights.
 */
void PWM_Initialize(void) {
	TCA0.SINGLE.CTRLD = TCA_SINGLE_SPLITM_bm;        // Enable split mode.
	TCA0.SPLIT.HPER   = 255;                         // HTOP set to 255.
	TCA0.SPLIT.LPER   = 255;                         // LTOP set to 255.
	TCA0.SPLIT.HCNT   = 0;                           // Make sure everything is in phase.
	TCA0.SPLIT.LCNT   = 0;                           // Make sure everything is in phase.
	TCA0.SPLIT.CTRLB  = TCA_SPLIT_LCMP0EN_bm | TCA_SPLIT_LCMP1EN_bm |
			TCA_SPLIT_HCMP1EN_bm | TCA_SPLIT_HCMP2EN_bm;  // Enable the desired PWM channels.
	TCA0.SPLIT.CTRLA  = (TCA_SPLIT_CLKSEL_DIV256_gc |     // fT = fCLK_PER / 256
			TCA_SPLIT_ENABLE_bm);                         // Enable the peripheral.
}

/**
 * Sets the duty cycle of the red PWM channel.
 * 
 * @param value Duty cycle from 0 to 255.
 */
void PWM_SetRed(uint8_t value) {
	TCA0.SPLIT.LCMP0 = value;
}

/**
 * Sets the duty cycle of the green PWM channel.
 * 
 * @param value Duty cycle from 0 to 255.
 */
void PWM_SetGreen(uint8_t value) {
	TCA0.SPLIT.LCMP1 = value;
}

/**
 * Sets the duty cycle of the blue PWM channel.
 * 
 * @param value Duty cycle from 0 to 255.
 */
void PWM_SetBlue(uint8_t value) {
	TCA0.SPLIT.HCMP1 = value;
}

/**
 * Sets the duty cycle of the white PWM channel.
 * 
 * @param value Duty cycle from 0 to 255.
 */
void PWM_SetWhite(uint8_t value) {
	TCA0.SPLIT.HCMP2 = value;
}

/**
 * Sets the duty cycle of all our color PWM channels.
 * 
 * @param r Red duty cycle from 0 to 255.
 * @param g Green duty cycle from 0 to 255.
 * @param b Blue duty cycle from 0 to 255.
 */
void PWM_SetRGB(uint8_t r, uint8_t g, uint8_t b) {
	PWM_SetRed(r);
	PWM_SetGreen(g);
	PWM_SetBlue(b);
}

/**
 * Sets the duty cycle of all our color PWM channels including white.
 * 
 * @param r Red duty cycle from 0 to 255.
 * @param g Green duty cycle from 0 to 255.
 * @param b Blue duty cycle from 0 to 255.
 * @param w White duty cycle from 0 to 255.
 */
void PWM_SetRGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	PWM_SetRed(r);
	PWM_SetGreen(g);
	PWM_SetBlue(b);
	PWM_SetWhite(w);
}

/**
 * Sets the duty cycle of all our color PWM channels.
 * 
 * @param color RGB color structure.
 */
void PWM_SetColor(rgb_t color) {
	PWM_SetRed(color.r);
	PWM_SetGreen(color.g);
	PWM_SetBlue(color.b);
}

/**
 * Sets the duty cycle of all our color PWM channels including white.
 * 
 * @param color RGBW color structure.
 */
void PWM_SetColorW(rgbw_t color) {
	PWM_SetRed(color.r);
	PWM_SetGreen(color.g);
	PWM_SetBlue(color.b);
	PWM_SetWhite(color.w);
}
