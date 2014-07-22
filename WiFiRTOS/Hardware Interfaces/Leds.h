/*
 * Leds.h
 *
 *  Created on: 20/07/2014
 *      Author: David
 */

#ifndef LEDS_H_
#define LEDS_H_

#include <stdint.h>

#define LED_ON		0
#define LED_OFF		1
#define LED_TOGGLE	2

void Led_Red(uint8_t ui8State);
void Led_Yellow(uint8_t ui8State);
void Led_Green(uint8_t ui8State);
void Reset_Leds();

#endif /* LEDS_H_ */
