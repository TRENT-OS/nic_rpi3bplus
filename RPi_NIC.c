/**
* Copyright (C) 2020, Hensoldt Cyber GmbH
*/
#include "OS_Error.h"
#include "OS_Dataport.h"
#include "lib_debug/Debug.h"
#include "environment.h"
#include <circleos.h>
#include <circle/bcm54213.h>

#include "network/OS_NetworkStack.h"

#include <string.h>
#include <stdlib.h>

#include <sel4/sel4.h>

#include <camkes.h>

// If we pass the define from a system configuration header. CAmkES generation
// crashes when parsing this file. As a workaround we hardcode the value here
#define NIC_DRIVER_RINGBUFFER_NUMBER_ELEMENTS 16

boolean init_ok = false;

void post_init(void)
{
    Debug_LOG_INFO("GenetDevice_init");
	Bcm54213Device();
    init_ok = true;
    Debug_LOG_INFO("post_init() done");
}

int run(void)
{
    if (!Bcm54213Initialize())
	{
		Debug_LOG_ERROR("Cannot initialize BCM54213");
	}

	Debug_LOG_INFO("[EthDrv '%s'] starting", get_instance_name());

	unsigned nTimeout = 0;
	while (!Bcm54213IsLinkUp())
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
		if(Bcm54213ReceiveFrame(Buffer,(u32 *)&receivedLength))
		{
            OS_NetworkStack_RxBuffer_t* buf_ptr = (OS_NetworkStack_RxBuffer_t*)nic_port_to;

            if (receivedLength > sizeof(buf_ptr->data) )
            {
                Debug_LOG_WARNING(
                    "The max length of the data is %u, but the length of the "
                    "read data is %ld",
                    (unsigned)sizeof(buf_ptr->data),
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

	return OS_SUCCESS;
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
	if (!Bcm54213SendFrame(nic_port_from,*len))
	{
        Debug_LOG_ERROR("Bcm54213SendFrame failed");
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

    GetMACAddress((uint8_t *)nic_port_to);

	return OS_SUCCESS;
}

/* Genet interrupt handler A-----------------------------------------------------------------*/
void genetA_BaseIrq_handle(void) {
    InterruptHandler0();

	int error = genetA_BaseIrq_acknowledge();

    if(error != 0)
	{
		Debug_LOG_ERROR("Failed to acknowledge the interrupt!");
	}
}

/* Genet interrupt handler B-----------------------------------------------------------------*/
void genetB_BaseIrq_handle(void) {
    InterruptHandler1();

	int error = genetB_BaseIrq_acknowledge();

    if(error != 0)
	{
		Debug_LOG_ERROR("Failed to acknowledge the interrupt!");
	}
}
