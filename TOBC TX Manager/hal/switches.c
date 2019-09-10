/*
 * switches.c
 *
 *  Created on: 11 Oct 2015
 *      Author: Tom
 */

//#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "switches.h"
#include "core.h"
#include "stopwatch.h"
//Connections list:
//With MCU: WDT_ACK P1.5, WDT_ERR P1.4
//With TX: GPIO_0 (high from syncword to packet end) P1.3, TX_CS P1.1, TX_RST P1.2
//With AND gate controlling PA: WDT_PA_EN(write) P1.0, PA_PWR(read tx chips PA control) P2.0


//To implement if time available so this can let the tobc know when a packet is sent
//ACK line, TOBC drops low before packet sends, switches to input, if GPIO0 does not go high the err line is pulled high, if success the ack line is pulled high then input and output swap


static volatile uint8_t switchFlags;
void tx_wdt_expired();

void switches_init()        //What P1IN hold when direction is set to out
{
    //Setting switch direction (1 for output or unused)
    //Switches rails off, 2nd top bit is interrupt
    P1DIR = 0xF5;
    P1OUT = 0x31;
    P1IE =  0x8;     //Whether flags are switched on for a pin //interrupt on gpio0
    P1IES = 0;      //Setting flags to rising
	switchFlags = 0;
}


void switches_process()
{
    uint8_t sendingPacket;
    //Checking whether the interrupt was rising or falling
    sendingPacket =  0x8 & P1IES;
    //If it is set to rising the interrupt was caused by a fall
    //if rising
    if (sendingPacket) {
        stopwatch_start(TICKS_PER_SECOND * 1, &tx_wdt_expired);
    } else  {
        //Should switch P1 back high assuming the GPIO_0 pin drops back low when the Tx chip is reset
        P1OUT |= 0b0100;
        stopwatch_disable();
    }
    switchFlags = 0;
}

void tx_wdt_expired()   {
    //Bringing reset pin low
    P1OUT &= ~0b0100;

}


//Connections list:
//With MCU: WDT_ACK P1.5, WDT_ERR P1.4
//With TX: GPIO_0 (high from syncword to packet end) P1.3, TX_CS P1.1, TX_RST P1.2
//With AND gate controlling PA: WDT_PA_EN(write) P1.0, PA_PWR(read tx chips PA control) P2.0

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	switchFlags |= P1IFG;
	// Clear flag
	P1IFG &= ~switchFlags;
    //Changes whether flag is on when a rise or fall is detected rising or falling, enabling the interrupt to catch both rising and falling changes

	P1IES ^= switchFlags & 0x8;

	core_check_wakeup(SWITCH);
}

