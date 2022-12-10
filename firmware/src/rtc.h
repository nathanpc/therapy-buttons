/**
 * rtc.h
 * Helps us keep track of time.
 * 
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef RTC_H
#define	RTC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <inttypes.h>
	
// Timer structure.
typedef struct {
	bool enabled;
	bool continuous;
	uint8_t period;
	uint8_t counter;
} rtc_timer_t;
	
// Initialization
void RTC_Initialize(uint16_t cmp_ms, uint16_t per_ms);

// Timer
void RTC_Timer_Setup(rtc_timer_t *timer, bool continuous, uint8_t period);
void RTC_Timer_Start(rtc_timer_t *timer);
void RTC_Timer_Restart(rtc_timer_t *timer);
void RTC_Timer_Pause(rtc_timer_t *timer);
void RTC_Timer_Stop(rtc_timer_t *timer);
bool RTC_Timer_Tick(rtc_timer_t *timer);

#ifdef	__cplusplus
}
#endif

#endif	/* RTC_H */

