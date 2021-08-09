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
#include <camkes/io.h>

#include "environment.h"

#include "OS_Error.h"
#include "OS_Dataport.h"
#include "network/OS_NetworkStack.h"

#include <sel4/sel4.h>

#include <platsupport/plat/mailbox_util.h>

// If we pass the define from a system configuration header. CAmkES generation
// crashes when parsing this file. As a workaround we hardcode the value here
#define NIC_DRIVER_RINGBUFFER_NUMBER_ELEMENTS 16

/* Private variables ----------------------------------------------------------------*/
unsigned long usb_host_controller_base_paddr;

// The marker stating wether the initialization was successful must be flagged
// volatile when strictly following the C rules. A hypothetical highly advanced
// optimizer could turn global variable accesses into constants, if it concludes
// the global state is always well known. Also, there is no rule in C that
// global variables must be synced with memory content on function entry and
// exit - it is just something that happen due to practical reasons. There is
// not even a rule that functions must be preserved and can't be inlined, which
// would eventually allow caching global variables easily. Furthermore, C also
// does not know threads nor concurrent execution of functions, but both have a
// string impact on global variables.
// Using volatile here guarantees at least, that accesses to global variables
// are accessing the actual memory in the given order stated in the program and
// there is no caching or constant folding that removes memory accesses. That is
// the best we can do to avoid surprises at higher optimization levels.
static volatile bool init_ok = false;

void
post_init(void)
{
	ps_io_ops_t io_ops;
	int ret = camkes_io_ops(&io_ops);
    if (0 != ret)
    {
        Debug_LOG_ERROR("camkes_io_ops() failed - error code: %d", ret);
        return;
    }

    //mailbox initialization
    ret = mbox_init(&io_ops);
    if (0 != ret)
    {
		Debug_LOG_ERROR("Mailbox initialization failed!");
        return;
    }

	usb_host_controller_base_paddr = (unsigned long)usbBaseReg;

	init_ok = true;
}

/* Main -----------------------------------------------------------------------------*/
int run()
{
	if (!init_ok)
	{
		Debug_LOG_ERROR("Initialization failed!");
		return OS_ERROR_INVALID_STATE;
	}

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
