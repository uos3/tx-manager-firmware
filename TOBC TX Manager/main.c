#include <msp430.h>
#include "core.h"
#include "hal/watchdog.h"
#include "hal/stopwatch.h"

//This code is heavily based upon the EPS firmware code available at https://github.com/uos3/eps-firmware
//by Giles Sohi

//Connections list:
//With MCU: WDT_ACK P1.5, WDT_ERR P1.4
//With TX: GPIO_0 (high from syncword to packet end) P1.3, TX_CS P1.1, TX_RST P1.2
//With AND gate controlling PA: WDT_PA_EN(write) P1.0, PA_PWR(read tx chips PA control) P2.0

/*Functionality
 * Own watchdog using built in module
 * Watchdog for tx chip, started on packet sync word (GPIO0 high) stopped on packet end (GPIO0 low)
 * Pulse WDT_ACK low on packet start, pulse WDT_ERR low on WDT timeout
 */
int main(void)
{
	watchdog_stop();    //TODO try changing to watchdog service
	core_init();
	switches_init();

    for (;;)    {

    	switch (core_sleep(STOPWATCH | SWITCH))
    	{
    	case STOPWATCH:
    		stopwatch_process();
    		break;

    	}
	}
}
