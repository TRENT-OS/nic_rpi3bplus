#
# NIC RPi3 driver
#
# Copyright (C) 2020, Hensoldt Cyber GmbH
#

cmake_minimum_required(VERSION 3.17)


function(NIC_RPi_DeclareCAmkESComponent name)

    DeclareCAmkESComponent(
        ${name}
        INCLUDES
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/include
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib
        SOURCES
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/ethernetDriver.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/environment.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/mailboxInterface.c

            # USPi Library
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/uspilibrary.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/dwhcidevice.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/dwhciregister.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/dwhcixferstagedata.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/usbconfigparser.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/usbdevice.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/usbdevicefactory.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/usbendpoint.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/usbrequest.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/usbstandardhub.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/devicenameservice.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/macaddress.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/usbfunction.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/smsc951x.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/lan7800.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/string.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/util.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/usbmassdevice.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/dwhciframeschednper.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/dwhciframeschedper.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/keymap.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/usbkeyboard.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/dwhcirootport.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/usbmouse.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/dwhciframeschednsplit.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/usbgamepad.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/synchronize.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/usbstring.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/uspi/lib/usbmidi.c

        C_FLAGS
            -Wall
            -Werror
        LIBS
            os_libs
            TimeServer_client
    )

endfunction()
