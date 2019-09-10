/*
 * watchdog.c
 *
 *  Created on: 10 Jan 2016
 *      Author: Tom
 */

#include <msp430.h>
#include "watchdog.h"

void watchdog_stop() {
	WDTCTL = WDTPW | WDTHOLD;
}


void watchdog_service() {
	WDTCTL = WDT_ARST_1000;
}
