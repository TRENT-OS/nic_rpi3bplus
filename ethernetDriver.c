/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#include <string.h>

#include <uspi.h>
#include <uspios.h>
#include <uspi/macros.h>
#include <uspi/dwhcidevice.h>

#include "LibDebug/Debug.h"

#include <platsupport/plat/spt.h>

#include <camkes.h>

#include "environment.h"

#include "OS_Dataport.h"

/* When sending data to the nw_stack, the data is saved in the dataport in the following way:
	0 - 4080 	=> read data
	4080 - 4096 => length of the read data
*/
#define MAX_READ_DATA_LENGTH		4080
#define POINTER_TO_LEN_IN_DATAPORT ((size_t*)(((uint8_t*)nic_port_to) + MAX_READ_DATA_LENGTH))

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
	// size_t receivedLength = 0;

	static const char fix_packet[1514] = {0x00,0xE0,0x4C,0x06,0x30,0xD4,0xB8,0x27,0xEB,0x52,0x4A,0x44,0x08,0x00,0x45,0x00,0x05,0xDC,0x91,0xC4,0x40,0x00,0x40,0x06,0x8F,0x43,0x0A,0x00,0x00,0x0B,0x0A,0x00,0x00,0x0A,0x15,0xB3,0xA7,0x02,0x21,0x1E,0x29,0xA4,0xB2,0x0C,0x53,0xF1,0x90,0x18,0x3A,0x58,0x56,0x48,0x00,0x00,0x03,0x03,0x00,0x08,0x0A,0x00,0x00,0x02,0x3D,0xCB,0x49,0x14,0x23,0x01,0x01};

	while(true)
	{
		// if(*POINTER_TO_LEN_IN_DATAPORT == 0)
		// {
		// 	if(USPiReceiveFrame (Buffer, &receivedLength))
		// 	{
		// 		if(receivedLength > MAX_READ_DATA_LENGTH)
		// 		{
		// 			Debug_LOG_WARNING("The max length of the data is %u, but the length of the read data is %u",
		// 									MAX_READ_DATA_LENGTH, receivedLength);
		// 		}

		// 		memcpy((((uint8_t*)nic_port_to) + *POINTER_TO_LEN_IN_DATAPORT), Buffer, receivedLength);
		// 		*POINTER_TO_LEN_IN_DATAPORT += receivedLength;

		// 		nic_event_hasData_emit();
		// 	}
		// }
	}

	if (!USPiSendFrame(fix_packet, 1514))
	{
        Debug_LOG_ERROR("USPiSendFrame failed");
		return OS_ERROR_ABORTED;
	}

	dma_free(Buffer, DMA_ALIGNEMENT);

	return 0;
}

/* nic_rpc interface ----------------------------------------------------------------*/
OS_Error_t nic_rpc_tx_data(size_t* len)
{
	if (!USPiSendFrame(nic_port_from, *len))
	{
        Debug_LOG_ERROR("USPiSendFrame failed");
		return OS_ERROR_ABORTED;
	}

	return OS_SUCCESS;
}

OS_Error_t nic_rpc_get_mac(void)
{
	if(!nic_driver_init_done_wait())
	{
		Debug_LOG_DEBUG("Wait failed.");
	}

	USPiGetMACAddress((uint8_t*)nic_port_to);

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
