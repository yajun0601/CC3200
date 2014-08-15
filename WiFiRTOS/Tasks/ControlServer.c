/*
 * ControlServer.c
 *
 *  Created on: 22/07/2014
 *      Author: David
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "osi.h"

#include "hw_types.h"
#include "hw_memmap.h"
#include "gpio.h"
#include "rom.h"
#include "rom_map.h"

#include "simpleLink.h"
#include "wlan.h"

#include "SimpleLink_conf.h"
#include "Leds.h"

extern OsiMsgQ_t g_sUartQuee;

void InitVariables();
int32_t ResetSimpleLink();
uint8_t WlanConnect();

const signed char g_cControlServer[] = "Control Server";
SimpleLinkConnection g_sSLCon;

void ControlServer(void *pvParameters) {

	char ServerBuffer[CONFIG_SERVER_BUFFER];
	char MsgBuffer[100];
	char *pMsgBuffer;

	SlSockAddrIn_t sAddr;
	SlSockAddrIn_t sLocalAddr;
	int32_t i32SockID;
	int32_t i32NewSockID;
	int32_t i32DataSize;
	int32_t i32NonBlocking = 1;
	SlSocklen_t i32AddrSize;
	int32_t retval;

	pMsgBuffer = &MsgBuffer[0];

	InitVariables();
	retval = ResetSimpleLink();
	if (retval < 0)
		while (1)
			;

	WlanConnect();

	sprintf(MsgBuffer, "Connectado a rede: %s\n\r"
			"IP: %d.%d.%d.%d\n\r"
			"Gateway: %d.%d.%d.%d\n\r", SL_IPV4_BYTE(g_sSLCon.DeviceIP, 3),
			SL_IPV4_BYTE(g_sSLCon.DeviceIP, 2),
			SL_IPV4_BYTE(g_sSLCon.DeviceIP, 1),
			SL_IPV4_BYTE(g_sSLCon.DeviceIP, 0),
			SL_IPV4_BYTE(g_sSLCon.GatewayIP, 3),
			SL_IPV4_BYTE(g_sSLCon.GatewayIP, 2),
			SL_IPV4_BYTE(g_sSLCon.GatewayIP, 1),
			SL_IPV4_BYTE(g_sSLCon.GatewayIP, 0));

	osi_MsgQWrite(&g_sUartQuee, &pMsgBuffer, OSI_NO_WAIT);

	sLocalAddr.sin_family = SL_AF_INET;
	sLocalAddr.sin_port = sl_Htons((unsigned short) g_sSLCon.PortNumber);
	sLocalAddr.sin_addr.s_addr = 0;

	i32SockID = sl_Socket(SL_AF_INET, SL_SOCK_STREAM, 0);
	sl_Bind(i32SockID, (SlSockAddr_t *) &sLocalAddr, sizeof(SlSockAddrIn_t));
	sl_Listen(i32SockID, 0);
	sl_SetSockOpt(i32SockID, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &i32NonBlocking,
			sizeof(i32NonBlocking));

	while (1) {
		i32NewSockID = SL_EAGAIN;

		while (i32NewSockID < 0) {
			i32NewSockID = sl_Accept(i32SockID, (struct SlSockAddr_t *) &sAddr,
					(SlSocklen_t*) &i32AddrSize);
			if (i32NewSockID == SL_EAGAIN) {
				osi_Sleep(100);
			} else if (i32NewSockID < 0) {
				while (1) {
				}
			}
		}

		i32DataSize = sl_Recv(i32NewSockID, ServerBuffer, CONFIG_SERVER_BUFFER,
				0);

		if (strcmp(ServerBuffer, "Led On") == 0) {
			Led_Green(LED_ON);
			strcpy(ServerBuffer, "OK");
			i32DataSize = 3;
		} else if (strcmp(ServerBuffer, "Led Off") == 0) {
			Led_Green(LED_OFF);
			strcpy(ServerBuffer, "OK");
			i32DataSize = 3;
		} else if (strcmp(ServerBuffer, "Lamp On") == 0) {
			MAP_GPIOPinWrite(GPIOA0_BASE, GPIO_PIN_6, GPIO_PIN_6);
			strcpy(ServerBuffer, "OK");
			i32DataSize = 3;
		} else if (strcmp(ServerBuffer, "Lamp Off") == 0) {
			MAP_GPIOPinWrite(GPIOA0_BASE, GPIO_PIN_6, 0);
			strcpy(ServerBuffer, "OK");
			i32DataSize = 3;
		}

		sl_Send(i32NewSockID, ServerBuffer, i32DataSize, 0);
		sl_Close(i32NewSockID);
	}

}

void InitVariables() {
	g_sSLCon.PortNumber = CONFIG_SERVER_PORT;
	g_sSLCon.DeviceIP = 0;
	g_sSLCon.GatewayIP = 0;
	memcpy(g_sSLCon.PW, CONFIG_HOST_PW, sizeof(CONFIG_HOST_PW));
	memcpy(g_sSLCon.SSID, CONFIG_HOST_SSID, sizeof(CONFIG_HOST_SSID));
}

int32_t ResetSimpleLink() {
	int32_t i32Mode = -1, i32RetVal = -1;
	uint8_t ui8Val = 1;

	i32Mode = sl_Start(0, 0, 0);

	i32RetVal = sl_WlanPolicySet(SL_POLICY_CONNECTION,
			SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);
	if (i32RetVal != 0) {
		return -1;
	}

	i32RetVal = sl_WlanProfileDel(0xFF);
	if (i32RetVal != 0) {
		return -1;
	}

	if (ROLE_STA != i32Mode) {
		if (ROLE_AP == i32Mode) {
			while (!STATUS_GET(g_sSLCon.Status, STATUS_BIT_IP_ACQUIRED)) {

			}
		}

		sl_WlanSetMode(ROLE_STA);
		sl_Stop(0);

		g_sSLCon.Status = 0;

		i32RetVal = sl_Start(0, 0, 0);

		if (ROLE_STA != i32RetVal) {
			return -1;
		}
	}

	i32RetVal = sl_WlanDisconnect();
	if (i32RetVal == 0) {
		while (STATUS_GET(g_sSLCon.Status, STATUS_BIT_CONNECTED)) {

		}
	}

	sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE, 1, 1, &ui8Val);
	sl_WlanPolicySet(SL_POLICY_SCAN, SL_SCAN_POLICY(0), NULL, NULL);

	ui8Val = 0;
	sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID,
	WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, 1, (unsigned char *) &ui8Val);

	sl_WlanPolicySet(SL_POLICY_PM, SL_NORMAL_POLICY, NULL, 0);
	sl_NetAppMDNSUnRegisterService(0, 0);
	sl_Stop(0);

	InitVariables();

	return 0;
}

uint8_t WlanConnect() {

	SlSecParams_t secParams = { 0 };

	secParams.Key = g_sSLCon.PW;
	secParams.KeyLen = strlen(g_sSLCon.PW);
	secParams.Type = CONFIG_HOST_SEC_TYPE;

	sl_Start(0, 0, 0);
	sl_WlanConnect(g_sSLCon.SSID, strlen(g_sSLCon.SSID), 0, &secParams, 0);

	while ((!STATUS_GET(g_sSLCon.Status, STATUS_BIT_CONNECTED))
			|| (!STATUS_GET(g_sSLCon.Status, STATUS_BIT_IP_ACQUIRED))) {
	}

	return 0;
}

