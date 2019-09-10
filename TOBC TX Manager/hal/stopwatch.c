/*
 * stopwatch.c
 *
 *  Created on: 8 Jan 2016
 *      Author: Tom
 */

#include <msp430.h>
#include <stdint.h>
#include "core.h"
#include "stopwatch.h"

static uint32_t timeLeft = 0;
static void (*stopwatch_callback_stored)();

void stopwatch_init()
{
}

void stopwatch_start(uint32_t stopValue, void (*stopwatch_callback)())
{
	TB0CCTL0 = 0; // Disable interrupt
	TB0CTL = 0;   // Disable timer.
	TB0R = 0;      // Reset timer.

	stopwatch_callback_stored = stopwatch_callback;
	timeLeft = stopValue;
	if (timeLeft > 0xFFFF)
		TB0CCR0 = 0xFFFF;
	else if (timeLeft > 0)
		TB0CCR0 = timeLeft & 0xFFFF;

	if (timeLeft == 0)
		stopwatch_process();
	else
	{
		TB0CCTL0 = CCIE; // Enable interrupt.
		TB0CTL = TBSSEL_1 // Source from ACLK.
				| ID_0    // Divider /1.
				| MC_1;   // Up mode: the stopwatch counts up to TBCCR0
	}

}

void stopwatch_process()
{
	if (timeLeft == 0 && stopwatch_callback_stored)
	{
		stopwatch_callback_stored();
		stopwatch_disable();
	}
}

void stopwatch_disable()
{
	TB0CCTL0 = 0; // Disable interrupt
	stopwatch_callback_stored = 0;
	TB0CTL = 0;
	timeLeft = 0;
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer0_B0 (void)
{
	timeLeft -= TB0CCR0;

	if (timeLeft > 0xFFFF)
		TB0CCR0 = 0xFFFF;
	else if (timeLeft > 0)
		TB0CCR0 = timeLeft & 0xFFFF;
	else
	{
		TB0CTL = 0; // Disable stopwatch.
		core_check_wakeup(STOPWATCH);
	}
}
