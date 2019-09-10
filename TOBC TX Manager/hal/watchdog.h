/*
 * watchdog.h
 *
 *  Created on: 10 Jan 2016
 *      Author: Tom
 */

#ifndef HAL_WATCHDOG_H_
#define HAL_WATCHDOG_H_

#include <stdint.h>

//#define WDTSSEL                (0x0004)     //Sets clocks source to ACLK
//#define WDTCNTCL               (0x0008)     //Resets the watchdog timer

//#define WDTPW                  (0x5A00)     //Password needed to write to the watchdog

//#define WDT_RST_1000       (WDTPW+WDTCNTCL+WDTSSEL)                          /* 1000ms  " */

void watchdog_stop();
void watchdog_service();

#endif /* HAL_WATCHDOG_H_ */
