/*
 * core.c
 *
 *  Created on: 24 Oct 2015
 *      Author: Tom
 */


#include <msp430.h>
#include "core.h"
//#include "util/DCO_Library.h"
#include "hal/watchdog.h"
int _wakeReasons;
int _wakeEventsOccurred;

//static void aclk_init();
//static int calibrate_dco(int freq);


//Using default clock sources, ACLK=REF0 32,768Hz    MCLK and SMCLK =1MHz
int core_init()
{
	//P3DIR |= BIT0;
	//aclk_init();
	//return calibrate_dco(8);
    return 1;
}

/*
static void aclk_init()
{
	BCSCTL1 = (BCSCTL1 & ~DIVA_3) | DIVA_0; // ACLK/1
	BCSCTL3 |= XCAP_3; // 12.5pF cap
}

//Digitally controlled oscillator
static int calibrate_dco(int freq)
{
	volatile unsigned int i;
	for( i = 0; i < 0xFFFF; i++); // Delay for ACLK startup

	// Calibrate the DCO from ACLK.
	int res = TI_SetDCO((freq * 1000000UL) / 4096);

	return res == TI_DCO_NO_ERROR;
}
*/
void core_clear_wakereasons() //TODO never used
{
	_wakeReasons = NONE;
	_wakeEventsOccurred = 0;
}

static WAKE_REASON get_next_event() {
	int flag;
	for (flag = STOPWATCH; flag != NONE; flag >>= 1) {  //TODO add highest number flag here
		if (_wakeEventsOccurred & flag) {
			// Clear the flag to say we've processed it.
			_wakeEventsOccurred &= ~flag;
			return (WAKE_REASON)flag;               //Should be in an else
		}
	}
	return NONE;        //Returns if there are no 1's in wake events occurred
}

//wakeReasons - things that are allowed to wake up the device at any time (stopwatch only after deployment)
//_wakeEventsOccurred - things that have occurred and need to be done once the device has awoken

WAKE_REASON core_sleep(int wakeReasons) {
	// Clear any events that triggered in the past that we don't trigger off anymore.
	int diff = _wakeReasons ^ wakeReasons;
	_wakeEventsOccurred &= ~diff;           //Set to zero first time after startup
	//_wakeEventsOccurred is the events that need to occur NOT have occurred

	_wakeReasons = wakeReasons;
	// Check we don't already have a wake event waiting.
	WAKE_REASON event = get_next_event();
	if (event != NONE) {
		return event;
	}

	// Figure out lowest power mode we can enter.
	int sleepMode;
	if (wakeReasons == NONE) {
		sleepMode = LPM4_bits;      //TODO Shouldn't be sleeping in this mode , enters it never cannot be woken by stopwatch as ACLK stopped & condition can never be satisfied
	} else {
		sleepMode = LPM3_bits;
	}

	// Disable interrupts to avoid race condition where we miss the interrupt.
	// Equivalent to setting GIE to 0
    __disable_interrupt();
	if (_wakeEventsOccurred == 0) {
		// Stop the dog
		watchdog_stop();
		// Go to sleep, reenabling interrupts.
		_BIS_SR(sleepMode | GIE);
		// Reenable the dog
		watchdog_service();
	} else {
		__enable_interrupt();
	}

	// Find event which triggered the wake up.
	return get_next_event();
}
