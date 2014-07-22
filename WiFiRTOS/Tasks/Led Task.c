/*
 * Led Task.c
 *
 *  Created on: 22/07/2014
 *      Author: David
 */

#include "Leds.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "osi.h"

extern OsiMsgQ_t	g_sUartQuee;

const signed char 	g_cLedTask_Name[] = "Led Task";

void LedTask(void *pvParameters) {

	char *msg = "Led task initialized.\n\r";
	Reset_Leds();

	osi_MsgQWrite(&g_sUartQuee, &msg, OSI_NO_WAIT);

	while (1) {
		Led_Red(LED_TOGGLE);
		osi_Sleep(1000);
	}
}
