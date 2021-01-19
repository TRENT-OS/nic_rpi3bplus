/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

/* Includes -------------------------------------------------------------*/
#include "mailboxInterface.h"

/* Defines -------------------------------------------------------------*/
#define POWER_STATE_OFF		(0 << 0)
#define POWER_STATE_ON		(1 << 0)
#define POWER_STATE_WAIT	(1 << 1)
#define POWER_STATE_NO_DEVICE	(1 << 1)	// in response
#define DEVICE_ID_SD_CARD	0
#define DEVICE_ID_USB_HCD	3

/* Type declarations -------------------------------------------------------------*/
typedef struct PropertyTagPowerState
{
	MailboxInterface_PropertyTag	Tag;
	uint32_t		nDeviceId;
	uint32_t		nState;
}
PropertyTagPowerState;

typedef struct PropertyTagMACAddress
{
	MailboxInterface_PropertyTag	Tag;
	uint8_t		Address[6];
	uint8_t		Padding[2];
}
PropertyTagMACAddress;