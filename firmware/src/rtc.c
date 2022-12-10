/**
 * rtc.c
 * Helps us keep track of time.
 * 
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "rtc.h"
#include "global_pins.h"
#include "nvmconfig.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/**
 * Initializes the RTC peripheral.
 * 
 * @param cmp_ms Period of time in milliseconds between each CMP interrupt.
 * @param per_ms Period of time in milliseconds between each OVF interrupt.
 */
void RTC_Initialize(uint16_t cmp_ms, uint16_t per_ms) {
	// Wait for all register to be synchronized.
	while (RTC.STATUS > 0)
		;
	
	// RTC
	RTC.CLKSEL  = RTC_CLKSEL_INT32K_gc;             // 32.768kHz internal oscillator.
	RTC.CNT = 0;                                    // Reset the counter.
	RTC.CMP = 32768 / (1000 / cmp_ms);              // Compare value.
	RTC.PER = 32768 / (1000 / per_ms);              // Period value.
	RTC.INTCTRL = (RTC_CMP_bm | RTC_OVF_bm);        // CMP and OVF interrupts enabled
	RTC.CTRLA  = (RTC_RUNSTDBY_bm | RTC_RTCEN_bm);  // Run the RTC even in standby.
	
	// Wait for all register to be synchronized.
	while (RTC.PITSTATUS > 0)
		;
	
	// PIT
	RTC.PITINTCTRL = RTC_PI_bm;                 // Enable the PIT interrupt.
	RTC.PITCTRLA   = (RTC_PERIOD_CYC32768_gc |  // Interrupt every second.
			RTC_PITEN_bm);                      // Enable the PIT timer.
}

/**
 * Sets up a timer.
 * 
 * @param timer      Timer object to be set up.
 * @param continuous Should the timer reset and continue counting after it's up?
 * @param period     Period of the timer.
 */
void RTC_Timer_Setup(rtc_timer_t *timer, bool continuous, uint8_t period) {
	timer->enabled = false;
	timer->continuous = continuous;
	timer->counter = 0;
	timer->period = period;
}

/**
 * Starts a timer.
 * 
 * @param timer Timer object to be started.
 */
void RTC_Timer_Start(rtc_timer_t *timer) {
	timer->enabled = true;
}

/**
 * Restarts a timer.
 * 
 * @param timer Timer object to be restarted.
 */
void RTC_Timer_Restart(rtc_timer_t *timer) {
	timer->enabled = false;
	timer->counter = 0;
	timer->enabled = true;
}

/**
 * Pauses a timer.
 * 
 * @param timer Timer object to be paused.
 */
void RTC_Timer_Pause(rtc_timer_t *timer) {
	timer->enabled = false;
}

/**
 * Stops a timer.
 * 
 * @param timer Timer object to be stopped.
 */
void RTC_Timer_Stop(rtc_timer_t *timer) {
	timer->enabled = false;
	timer->counter = 0;
}

/**
 * Ticks a timer and checks if its time is up.
 * 
 * @param  timer Timer object to be ticked and checked.
 * @return       TRUE when the timer has reached its period. (Time is up!)
 */
bool RTC_Timer_Tick(rtc_timer_t *timer) {
	bool finished;
	
	// Check if we actually have to tick.
	if (!timer->enabled)
		return false;
	
	// Tick the timer.
	timer->counter++;
	finished = timer->counter >= timer->period;
	
	// Are we done?
	if (finished) {
		timer->counter = 0;
		timer->enabled = timer->continuous;
	}
	
	return finished;
}
