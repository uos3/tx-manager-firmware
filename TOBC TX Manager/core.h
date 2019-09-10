/*
 * core.h
 *
 *  Created on: 24 Oct 2015
 *      Author: Tom
 */

#ifndef CORE_H_
#define CORE_H_

#include <msp430.h>

#define F_CPU 8000000UL

typedef enum {      //Higher number - higher priority
	NONE = 0,
	SWITCH = 1,
	STOPWATCH = 2,
} WAKE_REASON;

// Initialises the core of the system, including
// DCO.
int core_init();

// Puts the microcontroller to the lowest power mode
// with the specified modules allowed to wake.
WAKE_REASON core_sleep(int wakeReasons);

void core_clear_wakereasons();

extern int _wakeReasons;
extern int _wakeEventsOccurred;

#define core_check_wakeup(reason) do { \
	if (_wakeReasons & reason) \
	{ \
		_BIC_SR_IRQ(LPM0_bits); \
	} \
	_wakeEventsOccurred |= reason; \
} while (0)

#endif /* CORE_H_ */
