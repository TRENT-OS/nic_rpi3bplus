#
# NIC RPi Driver
#
# Copyright (C) 2020, Hensoldt Cyber GmbH
#

cmake_minimum_required(VERSION 3.7.2)


# we cannot use CMAKE_CURRENT_LIST_DIR in a function or macro, because that
# would give us the dir of the file that is invoking the macro. The work around
# is that we create a global (ie cached and flagged as internal) variable here
# and then use it in the macro below. As a convenient side effect we don't#
# require anybody else to provide any helper variables, this CMake file becomes
# fully self-contained and "copy&paste safe".
set(OS_COMPONENT_NIC_DIR "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "")


macro(DeclareCAmkESComponent_NIC_RPi name)

    DeclareCAmkESComponent(
        ${name}
        INCLUDES
            ${OS_COMPONENT_NIC_DIR}/uspi/include
            ${OS_COMPONENT_NIC_DIR}/uspi/lib
        SOURCES
            ${OS_COMPONENT_NIC_DIR}/ethernetDriver.c
            ${OS_COMPONENT_NIC_DIR}/environment.c
            ${OS_COMPONENT_NIC_DIR}/mailboxInterface.c

            # USPi Library
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/uspilibrary.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/dwhcidevice.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/dwhciregister.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/dwhcixferstagedata.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/usbconfigparser.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/usbdevice.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/usbdevicefactory.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/usbendpoint.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/usbrequest.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/usbstandardhub.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/devicenameservice.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/macaddress.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/usbfunction.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/smsc951x.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/lan7800.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/string.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/util.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/usbmassdevice.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/dwhciframeschednper.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/dwhciframeschedper.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/keymap.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/usbkeyboard.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/dwhcirootport.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/usbmouse.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/dwhciframeschednsplit.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/usbgamepad.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/synchronize.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/usbstring.c
            ${OS_COMPONENT_NIC_DIR}/uspi/lib/usbmidi.c

        C_FLAGS
            -Wall
            -Werror
        LIBS
            os_libs
            TimeServer_client

    )

endmacro()
