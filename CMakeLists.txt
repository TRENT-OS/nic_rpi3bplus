#
# NIC RPi3 driver
#
# Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

cmake_minimum_required(VERSION 3.17)


function(NIC_RPi_DeclareCAmkESComponent name)

    DeclareCAmkESComponent(
        ${name}
        INCLUDES
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/include
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib
        SOURCES
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/ethernetDriver.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/environment.c

            # USPi Library
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/uspilibrary.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/dwhcidevice.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/dwhciregister.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/dwhcixferstagedata.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/usbconfigparser.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/usbdevice.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/usbdevicefactory.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/usbendpoint.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/usbrequest.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/usbstandardhub.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/devicenameservice.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/macaddress.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/usbfunction.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/smsc951x.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/lan7800.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/string.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/util.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/usbmassdevice.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/dwhciframeschednper.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/dwhciframeschedper.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/keymap.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/usbkeyboard.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/dwhcirootport.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/usbmouse.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/dwhciframeschednsplit.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/usbgamepad.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/synchronize.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/usbstring.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/3rdParty/uspi/lib/usbmidi.c

        C_FLAGS
            -Wall
            -Werror
        LIBS
            os_core_api
            lib_debug
            TimeServer_client
    )

endfunction()
