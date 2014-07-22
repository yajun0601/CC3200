/*
 * Leds.c
 *
 *  Created on: 20/07/2014
 *      Author: David
 */

#include <stdint.h>

#include "hw_types.h"
#include "hw_memmap.h"

#include "gpio.h"
#include "rom.h"
#include "rom_map.h"

#include "Leds.h"

void Led_Toggle(uint8_t ui8Pin);

void Led_Red(uint8_t ui8State) {

	switch (ui8State) {

	case LED_ON:
		MAP_GPIOPinWrite(GPIOA1_BASE, GPIO_PIN_1, GPIO_PIN_1);
		break;

	case LED_OFF:
		MAP_GPIOPinWrite(GPIOA1_BASE, GPIO_PIN_1, 0);
		break;

	case LED_TOGGLE:
		Led_Toggle(GPIO_PIN_1);
	}
}

void Led_Yellow(uint8_t ui8State) {

	switch (ui8State) {

	case LED_ON:
		MAP_GPIOPinWrite(GPIOA1_BASE, GPIO_PIN_2, GPIO_PIN_2);
		break;

	case LED_OFF:
		MAP_GPIOPinWrite(GPIOA1_BASE, GPIO_PIN_2, 0);
		break;

	case LED_TOGGLE:
		Led_Toggle(GPIO_PIN_2);
	}
}

void Led_Green(uint8_t ui8State) {

	switch (ui8State) {

	case LED_ON:
		MAP_GPIOPinWrite(GPIOA1_BASE, GPIO_PIN_3, GPIO_PIN_3);
		break;

	case LED_OFF:
		MAP_GPIOPinWrite(GPIOA1_BASE, GPIO_PIN_3, 0);
		break;

	case LED_TOGGLE:
		Led_Toggle(GPIO_PIN_3);
	}
}

void Led_Toggle(uint8_t ui8Pin) {
	uint8_t ui8LedStatus;
	ui8LedStatus = MAP_GPIOPinRead(GPIOA1_BASE, ui8Pin);
	if (ui8LedStatus == 0)
		MAP_GPIOPinWrite(GPIOA1_BASE, ui8Pin, ui8Pin);
	else
		MAP_GPIOPinWrite(GPIOA1_BASE, ui8Pin, 0);
}

void Reset_Leds() {
	Led_Red(LED_OFF);
	Led_Yellow(LED_OFF);
	Led_Green(LED_OFF);
}
