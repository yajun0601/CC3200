/*
 * UART Channel.c
 *
 *  Created on: 22/07/2014
 *      Author: David
 */

#include "uart_if.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "osi.h"

const signed char 	g_cUartTaskName[] = "Uart Channel";
OsiMsgQ_t			g_sUartQuee;

void UartTask(void *pvParameters) {

	//OsiReturnVal_e	eRetVal;
	char * pcMsg;

	InitTerm();
	ClearTerm();

	/*eRetVal = */osi_MsgQCreate(&g_sUartQuee, "Uart Channel", sizeof(unsigned long), 10);

	//if(eRetVal != OSI_OK)
		//Report("ERROR: Failed to init Uart Quee, error code %d.\n\r", eRetVal);

	Message("Uart Channel Initialized.\n\r Waiting for messages.\n\r");

	while (1) {
		osi_MsgQRead(&g_sUartQuee, &pcMsg, OSI_WAIT_FOREVER);
		Message(pcMsg);

	}
}
