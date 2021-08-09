/*
 * Copyright (C) 2020-2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <platsupport/plat/mailbox_util.h>

#define MAC_ADDR_LEN 6

typedef struct {
    MailboxInterface_PropertyTag_t tag;
}
PropertyTag_GetMACAddress_Request_t;

typedef struct {
    MailboxInterface_PropertyTag_t tag;
    uint8_t mac_address[6];
}
PropertyTag_GetMACAddress_Response_t;

#define TAG_GET_MAC_ADDRESS 0x00010003

int mbox_init(ps_io_ops_t *io_ops);
