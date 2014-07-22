/*
 * SimpleLink_Hooks.c
 *
 *  Created on: 22/07/2014
 *      Author: David
 */

#include <stdint.h>

#include "simpleLink.h"
#include "wlan.h"
#include "SimpleLink_conf.h"

extern SimpleLinkConnection g_sSLCon;

void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent) {

	switch (pWlanEvent->Event) {

	case SL_WLAN_CONNECT_EVENT:
		STATUS_SET(g_sSLCon.Status, STATUS_BIT_CONNECTED);

		//Report("[WLAN EVENT] STA Connected to the AP: %s\n\r",
		//		g_sConnection.ui8SSID);
		break;

	case SL_WLAN_DISCONNECT_EVENT:
		STATUS_CLR(g_sSLCon.Status, STATUS_BIT_CONNECTED);
		STATUS_CLR(g_sSLCon.Status, STATUS_BIT_IP_ACQUIRED);
//		Report("[WLAN EVENT] STA Disconnected from the AP: %s\n\r",
//				g_sConnection.ui8SSID);
		break;

	default:
//		Report("[WLAN EVENT] Unexpected event [0x%x]\n\r", pWlanEvent->Event);
		break;
	}
}

void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent) {

	SlIpV4AcquiredAsync_t *pEventData = NULL;

	switch (pNetAppEvent->Event) {
	case SL_NETAPP_IPV4_ACQUIRED:
		STATUS_SET(g_sSLCon.Status, STATUS_BIT_IP_ACQUIRED);

		pEventData = &pNetAppEvent->EventData.ipAcquiredV4;
		g_sSLCon.DeviceIP = pEventData->ip;
		g_sSLCon.GatewayIP = pEventData->gateway;

//		Report("[NETAPP EVENT] IP Acquired: IP=%d.%d.%d.%d , "
//				"Gateway=%d.%d.%d.%d\n\r",
//				SL_IPV4_BYTE(g_sConnection.ui32DeviceIP, 3),
//				SL_IPV4_BYTE(g_sConnection.ui32DeviceIP, 2),
//				SL_IPV4_BYTE(g_sConnection.ui32DeviceIP, 1),
//				SL_IPV4_BYTE(g_sConnection.ui32DeviceIP, 0),
//				SL_IPV4_BYTE(g_sConnection.ui32GatewayIP, 3),
//				SL_IPV4_BYTE(g_sConnection.ui32GatewayIP, 2),
//				SL_IPV4_BYTE(g_sConnection.ui32GatewayIP, 1),
//				SL_IPV4_BYTE(g_sConnection.ui32GatewayIP, 0));
		break;

	default:
//		Report("[NETAPP EVENT] Unexpected event [0x%x] \n\r",
//				pNetAppEvent->Event);
		break;
	}
}

void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent,
		SlHttpServerResponse_t *pHttpResponse) {
}

void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent) {
//	Report("[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n",
//			pDevEvent->EventData.deviceEvent.status,
//			pDevEvent->EventData.deviceEvent.sender);
}

void SimpleLinkSockEventHandler(SlSockEvent_t *pSock) {
}
