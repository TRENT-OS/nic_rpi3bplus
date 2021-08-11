/*
 * Copyright (C) 2020-2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <platsupport/plat/mailbox_util.h>

#define MAC_ADDR_LEN 6

int mbox_init(ps_io_ops_t *io_ops);
