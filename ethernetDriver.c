/*
 * Copyright (C) 2020-2021, HENSOLDT Cyber GmbH
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>

#include <uspi.h>
#include <uspios.h>
#include <uspi/macros.h>
#include <uspi/dwhcidevice.h>

#include "lib_debug/Debug.h"

#include <camkes.h>

#include "environment.h"

#include "OS_Dataport.h"
#include "network/OS_NetworkStack.h"

#include <sel4/sel4.h>

// If we pass the define from a system configuration header. CAmkES generation
// crashes when parsing this file. As a workaround we hardcode the value here
#define NIC_DRIVER_RINGBUFFER_NUMBER_ELEMENTS 16

/* Private variables ----------------------------------------------------------------*/
unsigned long usb_host_controller_base_paddr;

void
post_init(void)
{
	usb_host_controller_base_paddr = (unsigned long)usbBaseReg;
}

/* Main -----------------------------------------------------------------------------*/
int run()
{
	if (!USPiInitialize())
	{
		Debug_LOG_ERROR("Cannot initialize USPi");
	}

	Debug_LOG_INFO("[EthDrv '%s'] starting", get_instance_name());

	if (!USPiEthernetAvailable ())
	{
		Debug_LOG_ERROR("Ethernet device not found");
	}

	unsigned nTimeout = 0;
	while (!USPiEthernetIsLinkUp ())
	{
		MsDelay (100);

		if (++nTimeout < 40)
		{
			continue;
		}
		nTimeout = 0;

		Debug_LOG_WARNING("Link is down");
	}

	Debug_LOG_DEBUG("Link is up");

	if(!nic_driver_init_done_post())
	{
		Debug_LOG_DEBUG("Post failed.");
	}

	uint8_t* Buffer = (uint8_t*)dma_alloc(DMA_PAGE_SIZE, DMA_ALIGNEMENT);
	size_t receivedLength = 0;

	unsigned int count = 0;
	while(true)
	{
		if(USPiReceiveFrame(Buffer, &receivedLength))
		{
            OS_NetworkStack_RxBuffer_t* buf_ptr = (OS_NetworkStack_RxBuffer_t*)nic_to_port;

            if (receivedLength > sizeof(buf_ptr->data) )
            {
                Debug_LOG_WARNING(
                    "The max length of the data is %u, but the length of the "
                    "read data is %u",
                    sizeof(buf_ptr->data),
                    receivedLength);
                // throw away current frame and read the next one
                continue;
            }

            // if the slot to be used in the ringbuffer isn't empty we
			// wait here in a loop
			// TODO: Implement it in an event driven fashion
			while(buf_ptr[count].len != 0) {
                seL4_Yield();
            }
			memcpy(buf_ptr[count].data, Buffer, receivedLength);
			buf_ptr[count].len = receivedLength;
            count = (count + 1) % NIC_DRIVER_RINGBUFFER_NUMBER_ELEMENTS;
            nic_event_hasData_emit();
		}
	}

	dma_free(Buffer, DMA_ALIGNEMENT);

	return 0;
}

/* nic_rpc interface ----------------------------------------------------------------*/

OS_Error_t
nic_rpc_rx_data(
	size_t* pLen,
	size_t* framesRemaining)
{
    return OS_ERROR_NOT_IMPLEMENTED;
}

OS_Error_t nic_rpc_tx_data(
	size_t* len)
{
	if (!USPiSendFrame(nic_from_port, *len))
	{
        Debug_LOG_ERROR("USPiSendFrame failed");
		return OS_ERROR_ABORTED;
	}

	return OS_SUCCESS;
}

OS_Error_t nic_rpc_get_mac_address(void)
{
	if(!nic_driver_init_done_wait())
	{
		Debug_LOG_DEBUG("Wait failed.");
	}

	USPiGetMACAddress((uint8_t*)nic_to_port);

	return OS_SUCCESS;
}

/* USB interrupt handler -----------------------------------------------------------------*/
void usbBaseIrq_handle(void) {

	DWHCIDeviceInterruptHandler(NULL);

	int error = usbBaseIrq_acknowledge();

    if(error != 0)
	{
		Debug_LOG_ERROR("Failed to acknowledge the interrupt!");
	}
}
