/*
 * SimpleLink_conf.h
 *
 *  Created on: 22/07/2014
 *      Author: David
 */

#ifndef SIMPLELINK_CONF_H_
#define SIMPLELINK_CONF_H_

#define CONFIG_SERVER_PORT		5005
#define CONFIG_SERVER_BUFFER	1400
#define CONFIG_HOST_SSID		"Akenge"
#define CONFIG_HOST_PW			"4pplco3d1"
#define CONFIG_HOST_SEC_TYPE	SL_SEC_TYPE_WPA
#define CONFIG_WLAN_SSID_MAX	32
#define CONFIG_WLAN_PW_MAX		32
#define CONFIG_WLAN_MAX_ATT		5

#define STATUS_BIT_IP_ACQUIRED	0
#define STATUS_BIT_CONNECTED	1

#define STATUS_GET(Status, Bit)		(Status & (1<<Bit))
#define STATUS_SET(Status, Bit)		(Status |= (1<<Bit))
#define STATUS_CLR(Status, Bit)		(Status &= ~(1<<Bit))

typedef struct {
	uint8_t Status;
	uint32_t DeviceIP;
	uint32_t GatewayIP;
	uint32_t PortNumber;
	char SSID[CONFIG_WLAN_SSID_MAX];
	char PW[CONFIG_WLAN_PW_MAX];
} SimpleLinkConnection;

#endif /* SIMPLELINK_CONF_H_ */
