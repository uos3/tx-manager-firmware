/*
 * stopwatch.h
 *
 *  Created on: 8 Jan 2016
 *      Author: Tom
 */

#ifndef HAL_STOPWATCH_H_
#define HAL_STOPWATCH_H_

#include <stdint.h>

#define TICKS_PER_SECOND 32768

void stopwatch_init();
void stopwatch_disable();
void stopwatch_process();
void stopwatch_start(uint32_t stopValue, void (*stopwatch_callback)());

#endif /* HAL_STOPWATCH_H_ */
