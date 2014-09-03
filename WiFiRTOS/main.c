#include <stdlib.h>
#include <stdint.h>

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "hw_common_reg.h"

#include "gpio.h"
#include "prcm.h"
#include "interrupt.h"
#include "rom.h"
#include "rom_map.h"

#include "uart_if.h"
#include "udma_if.h"
#include "pin_mux_config.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "osi.h"

#include "Leds.h"
#include "SimpleLink_conf.h"

extern void (* const g_pfnVectors[])(void);

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,
		signed portCHAR *pcTaskName);
extern void vApplicationTickHook(void);
extern void vAssertCalled(const char *pcFile, unsigned long ulLine);
extern void vApplicationIdleHook(void);

extern const signed char g_cLedTask_Name[];
extern void LedTask(void *pvParameters);

extern const signed char g_cUartTaskName[];
extern void UartTask(void *pvParameters);

extern const signed char g_cControlServer[];
extern SimpleLinkConnection g_sSLCon;
extern void ControlServer(void *pvParameters);

int main(void) {

	MAP_IntVTableBaseSet((unsigned long) &g_pfnVectors[0]);
	//MAP_IntMasterEnable();
	MAP_IntEnable(FAULT_SYSTICK);

	PRCMCC3200MCUInit();
	PinMuxConfig();
	UDMAInit();



	osi_TaskCreate(UartTask, g_cUartTaskName, 2000, NULL, 5, NULL);
	osi_TaskCreate(LedTask, g_cLedTask_Name, 200, NULL, 2, NULL);
	VStartSimpleLinkSpawnTask(4);
	osi_TaskCreate(ControlServer, g_cControlServer, 4096, NULL, 1, NULL);

	osi_start();
}

