/**
 * Copyright (C) 2019-2021, HENSOLDT Cyber GmbH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <uspios.h>

#include "lib_debug/Debug.h"

#include "environment.h"
#include "mailboxInterface.h"

#include "TimeServer.h"

#include <camkes.h>
#include <camkes/dma.h>

static const if_OS_Timer_t timer =
    IF_OS_TIMER_ASSIGN(
        timeServer_rpc,
        timeServer_notify);

/* Environment functions -------------------------------------------------------------*/
void* dma_alloc (unsigned nSize, unsigned alignement)
{
    // we are setting cached to false to allocate non-cached DMA memory for the
    // NIC driver
    return camkes_dma_alloc(nSize, alignement, false);
}

void dma_free (void* pBlock, unsigned alignement)
{
    camkes_dma_free(pBlock, alignement);
}

uintptr_t dma_getPhysicalAddr(void* ptr)
{
    return camkes_dma_get_paddr(ptr);
}

void LogWrite (const char* pSource, unsigned Severity, const char* pMessage,
               ...)
{
    va_list args;
    va_start (args, pMessage);

    switch (Severity)
    {
    case USPI_LOG_ERROR:
        printf("\n ERROR %s: - ", pSource);
        break;
    case USPI_LOG_WARNING:
        printf("\n WARNING %s: - ", pSource);
        break;
    case USPI_LOG_NOTICE:
        printf("\n NOTICE %s: - ", pSource);
        break;
    case USPI_LOG_DEBUG:
        printf("\n DEBUG %s: - ", pSource);
        break;
    }

    vprintf(pMessage, args);
    va_end(args);
}

void MsDelay (unsigned nMilliSeconds)
{
    usDelay(1000 * nMilliSeconds);
}

void usDelay (unsigned nMicroSeconds)
{
    OS_Error_t err;

    if ((err = TimeServer_sleep(&timer, TimeServer_PRECISION_USEC,
                                nMicroSeconds)) != OS_SUCCESS)
    {
        Debug_LOG_ERROR("TimeServer_sleep() failed with %d", err);
    }
}

unsigned StartKernelTimer (unsigned nHzDelay, TKernelTimerHandler* pHandler,
                           void* pParam, void* pContext)
{
    Debug_LOG_WARNING("Not implemented!");
    return 0;
}

void CancelKernelTimer (unsigned hTimer)
{
    Debug_LOG_WARNING("Not implemented!");
}

/*
	Not necessary => used for configuring the usb interrupt
	which is in our environment done by camkes
*/
void ConnectInterrupt (unsigned nIRQ, TInterruptHandler* pHandler, void* pParam)
{
    Debug_LOG_WARNING("Not implemented!");
}

int SetPowerStateOn (unsigned nDeviceId)
{
    PropertyTagPowerState PowerState;
    PowerState.nDeviceId = nDeviceId;
    PowerState.nState = POWER_STATE_ON | POWER_STATE_WAIT;

    if (!MailboxInterface_getTag (PROPTAG_SET_POWER_STATE, &PowerState,
                                  sizeof PowerState, 8)
        || (PowerState.nState & POWER_STATE_NO_DEVICE)
        || !(PowerState.nState & POWER_STATE_ON))
    {
        Debug_LOG_ERROR("Failed to set power state on!");
        return 0;
    }

    return 1;
}

int GetMACAddress (unsigned char Buffer[6])
{
    PropertyTagMACAddress MACAddress;

    if (!MailboxInterface_getTag(PROPTAG_GET_MAC_ADDRESS, &MACAddress,
                                 sizeof(MACAddress), 0))
    {
        Debug_LOG_ERROR("Failed to retreive the MAC address!");
        return 0;
    }

    memcpy(Buffer, MACAddress.Address, 6);

    return 1;
}
