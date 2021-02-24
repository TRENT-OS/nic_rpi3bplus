#
# RasPi3 (BCM2837) RPI SD driver
#
# Copyright (C) 2020, Hensoldt Cyber GmbH
#

cmake_minimum_required(VERSION 3.17)

#-------------------------------------------------------------------------------
# Declare RasPi3 (BCM2837) SPI MSD Driver CAmkES Component
#       TODO
#
# Parameters:
#
#   <name>
#     required, component instance name
#
#   LIBS <lib_1> <lib_2> ...
#     additional libraries. At least a TimeServer client library is required
#
function(NIC_RPi_DeclareCAmkESComponent name)

    cmake_parse_arguments(
        PARSE_ARGV
        1
        RPI_SD
        ""
        ""
        "LIBS"
    )

    DeclareCAmkESComponent(
        ${name}
        INCLUDES
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/lib
        SOURCES
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/RPi_NIC.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/lib/bcm54213.c

            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/lib/synchronize64.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/lib/macaddress.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/lib/netdevice.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/lib/memio.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/environment.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/mailboxInterface.c 
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/lib/spinlock.c
            ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/lib/string.c
        C_FLAGS
            -Wall
            -Werror
        LIBS
            os_core_api
            lib_debug
            ${RPI_SD_LIBS}
    )

endfunction()
