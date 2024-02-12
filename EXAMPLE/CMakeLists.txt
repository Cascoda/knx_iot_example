cmake_minimum_required(VERSION 3.18)
project(knx_iot_example  C CXX ASM)

# Copyright (c) 2022-2023 Cascoda Ltd
# All rights reserved.
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice, this list
#    of conditions and the following disclaimer.
#
# 2. Redistributions in binary form, except as embedded into a Cascoda Limited.
#    integrated circuit in a product or a software update for such product, must
#    reproduce the above copyright notice, this list of  conditions and the following
#    disclaimer in the documentation and/or other materials provided with the distribution.
# 
# 3. Neither the name of Cascoda Limited nor the names of its contributors may be used to
#    endorse or promote products derived from this software without specific prior written
#    permission.
#
# 4. This software, whether provided in binary or any other form must not be decompiled,
#    disassembled, reverse engineered or otherwise modified.
#
#  5. This software, in whole or in part, must only be used with a Cascoda Limited circuit.
#
# THIS SOFTWARE IS PROVIDED BY CASCODA LIMITED "AS IS" AND ANY EXPRESS
# OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL CASCODA LIMITED OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
# GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 

include(FetchContent)

option(CASCODA_USE_PRIVATE_SDK "Whether to use proprietary Cascoda code. Necessary for certain functionality." OFF)
set(CASCODA_SDK_REF master CACHE STRING "The branch of the Cascoda SDK to use in this build")
set(KNX_IOT_STACK_REF master CACHE STRING "The branch of the KNX IoT Stack to use in this build")
set(USE_GITLAB OFF CACHE BOOL "use gitlab as source for KNX IoT Stack")

if(CASCODA_USE_PRIVATE_SDK)
  set(CASCODA_SDK_REPO https://github.com/Cascoda/cascoda-sdk-priv.git CACHE STRING "")
else()
  set(CASCODA_SDK_REPO https://github.com/Cascoda/cascoda-sdk.git CACHE STRING "")
endif()

set(CASCODA_REPO cascoda-sdk)

if(USE_GITLAB)
  set(KNX_REPO knx-iot-stack-gitlab)
  set(KNX_IOT_STACK_REPO https://gitlab.knx.org/shared-projects/knx-iot-point-api-public-stack/ CACHE STRING "")
else()
  set(KNX_REPO knx-iot-stack)
  set(KNX_IOT_STACK_REPO https://github.com/KNX-IOT/KNX-IOT-STACK.git CACHE STRING "")
endif()

# KNX IoT Stack
FetchContent_Declare(
    ${KNX_REPO}
    GIT_REPOSITORY ${KNX_IOT_STACK_REPO}
    GIT_TAG ${KNX_IOT_STACK_REF}
)

# STACK + Port Cascoda
FetchContent_Declare(
    ${CASCODA_REPO}
    GIT_REPOSITORY ${CASCODA_SDK_REPO}
    GIT_TAG ${CASCODA_SDK_REF}
)

if(NOT CMAKE_SYSTEM_PROCESSOR STREQUAL M2351)

    set(USE_CONSOLE OFF CACHE BOOL "use console (for output logging)")

    if(MSVC)
      add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    endif()

    message(STATUS "This project is build for Linux/Windows")

    option(wxBUILD_SHARED "" OFF)
    # compile in the knx iot Router code from the stack
    #set(OC_IOT_ROUTER_ENABLED ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(${KNX_REPO})

    add_executable(knx_iot_example
      ${PROJECT_SOURCE_DIR}/knx_iot_example.c
      ${PROJECT_SOURCE_DIR}/knx_iot_example_virtual.c)
    target_link_libraries(knx_iot_example kisClientServer)

    if(WIN32)
      FetchContent_Declare(
          wxWidgets
          URL https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.5/wxWidgets-3.1.5.7z
      )
      option(wxBUILD_SHARED "" OFF)
      message(STATUS "Fetching wxWidgets, kindly wait...")
      FetchContent_MakeAvailable(wxWidgets)
    
      add_executable(knx_iot_example_gui WIN32
        ${PROJECT_SOURCE_DIR}/knx_iot_example_gui.cpp
        # ${PROJECT_SOURCE_DIR}/knx_iot_example_logic.c
        ${PROJECT_SOURCE_DIR}/knx_iot_example.c
        ${PROJECT_SOURCE_DIR}/knx_iot_example_virtual.c)
      target_link_libraries(knx_iot_example_gui wx::net wx::core wx::base kisClientServer)
      # enable flag to compile the console in, so that the printf of the stack are shown.
      target_compile_definitions(knx_iot_example_gui PUBLIC KNX_GUI)
      if(USE_CONSOLE)
        set_target_properties(knx_iot_example_gui PROPERTIES
          LINK_FLAGS /SUBSYSTEM:CONSOLE )
      endif()
      add_definitions(-DOC_SIMULATION)
    endif()




else()
    set(CASCODA_CHILI2_CONFIG_STRING "DEV_BOARD" CACHE STRING "KNX only supported on Devboard")
    set(CASCODA_CHILI2_REV "1" CACHE STRING "KNX only supported for Chili2 Rev 1")
    set(CASCODA_OTA_UPGRADE_ENABLED 0 CACHE BOOL "Enables OTA upgrade")
    set(USE_DEMO_MODE OFF CACHE BOOL "demo mode (no reset)")
    option(CASCODA_BUILD_KNX "Enable the KNX-IoT Port" ON)
    option(CASCODA_BUILD_BINARIES "Build Binaries" OFF)

    message(STATUS "Downloading KNX-IOT & Cascoda SDK source code...")
    FetchContent_MakeAvailable(${CASCODA_REPO})

    add_executable(knx_iot_example_ed
      # KNX-IoT code, common across all platforms
      ${PROJECT_SOURCE_DIR}/knx_iot_example.c
      # KNX-IoT code, common across all platforms
      ${PROJECT_SOURCE_DIR}/knx_iot_example_dev.c
      ${PROJECT_SOURCE_DIR}/knx_iot_example_logic.c
      # Embedded specific main loop, which initialises the MCU, communications & then runs the KNX application
      ${PROJECT_SOURCE_DIR}/knx_iot_wakeful_main.c
    )
    target_link_libraries(knx_iot_example_ed 
      kisClientServer
      kis-port
      # Openthread Minimal Thread Device
      ca821x-openthread-bm-mtd
      # Library for interfacing with the devboard
      cascoda-bm-devboard
      )

    add_executable(knx_iot_example_reed
      # KNX-IoT code, common across all platforms
      ${PROJECT_SOURCE_DIR}/knx_iot_example.c
      # KNX-IoT code, common across all platforms
      ${PROJECT_SOURCE_DIR}/knx_iot_example_dev.c
      ${PROJECT_SOURCE_DIR}/knx_iot_example_logic.c
      # Embedded specific main loop, which initialises the MCU, communications & then runs the KNX application
      ${PROJECT_SOURCE_DIR}/knx_iot_wakeful_main.c
    )
    target_link_libraries(knx_iot_example_reed
      kisClientServer
      kis-port
      # Openthread Full Thread Device
      ca821x-openthread-bm-ftd
      # Library for interfacing with the devboard
      cascoda-bm-devboard
      )

    add_executable(knx_iot_example_sleepy
      # KNX-IoT code, common across all platforms
      ${PROJECT_SOURCE_DIR}/knx_iot_example.c
      # KNX-IoT code, common across all platforms
      ${PROJECT_SOURCE_DIR}/knx_iot_example_dev.c
      ${PROJECT_SOURCE_DIR}/knx_iot_example_logic.c
      # Embedded specific main loop, which initialises the MCU, communications & then runs the KNX application
      ${PROJECT_SOURCE_DIR}/knx_iot_sleepy_main.c
    )
    target_link_libraries(knx_iot_example_sleepy 
      kisClientServer
      kis-port
      # Openthread Minimal Thread Device
      ca821x-openthread-bm-mtd
      # Library for interfacing with the devboard
      cascoda-bm-devboard
      )

# include device cmake files, after all the targets have been created so that they may be altered within


    if(USE_DEMO_MODE)
      target_compile_definitions(knx_iot_example_ed PUBLIC DEMO_MODE)
      target_compile_definitions(knx_iot_example_reed PUBLIC DEMO_MODE)
      target_compile_definitions(knx_iot_example_sleepy PUBLIC DEMO_MODE)
    endif()

    target_compile_definitions(knx_iot_example_ed PRIVATE NO_MAIN)
    target_compile_definitions(knx_iot_example_reed PRIVATE NO_MAIN)
    
    target_compile_definitions(knx_iot_example_sleepy PRIVATE NO_MAIN SLEEPY)

    if(NOT DEFINED STACK_SIZE_KB)
      set(STACK_SIZE_KB "5" CACHE STRING "Number of KB to alloc stack")
    endif()
    if(NOT DEFINED HEAP_SIZE_BASE_KB)
      set(HEAP_SIZE_BASE_KB "20" CACHE STRING "Number of KB to alloc heap")
    endif()
    math(EXPR STACK_SIZE "1024 * ${STACK_SIZE_KB}")
    math(EXPR HEAP_SIZE_ED "1024 * (${HEAP_SIZE_BASE_KB})")
    math(EXPR HEAP_SIZE_REED "1024 * (${HEAP_SIZE_BASE_KB}-4)")
    math(EXPR HEAP_SIZE_SLEEPY "1024 * (${HEAP_SIZE_BASE_KB})")
    
    # Set the size of the stack and the heap, respectively
    cascoda_configure_memory(knx_iot_example_ed ${STACK_SIZE} ${HEAP_SIZE_ED})
    cascoda_configure_memory(knx_iot_example_reed ${STACK_SIZE} ${HEAP_SIZE_REED})
    cascoda_configure_memory(knx_iot_example_sleepy ${STACK_SIZE} ${HEAP_SIZE_SLEEPY})

    # Generate the .bin file to be flashed onto the Chili
    cascoda_make_binary(knx_iot_example_ed CASCODA_BUILD_KNX)
    cascoda_make_binary(knx_iot_example_reed CASCODA_BUILD_KNX)
    cascoda_make_binary(knx_iot_example_sleepy CASCODA_BUILD_KNX)
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

    add_custom_command(TARGET knx_iot_example_ed
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy knx_iot_example_ed ${PROJECT_BINARY_DIR}/bin
      COMMAND ${CMAKE_COMMAND} -E copy knx_iot_example_ed.bin ${PROJECT_BINARY_DIR}/bin
      COMMAND ${CMAKE_COMMAND} -E remove knx_iot_example_ed
    )
    add_custom_command(TARGET knx_iot_example_reed
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy knx_iot_example_reed ${PROJECT_BINARY_DIR}/bin
      COMMAND ${CMAKE_COMMAND} -E copy knx_iot_example_reed.bin ${PROJECT_BINARY_DIR}/bin
      COMMAND ${CMAKE_COMMAND} -E remove knx_iot_example_reed
    )
    
    add_custom_command(TARGET knx_iot_example_sleepy
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy knx_iot_example_sleepy ${PROJECT_BINARY_DIR}/bin
      COMMAND ${CMAKE_COMMAND} -E copy knx_iot_example_sleepy.bin ${PROJECT_BINARY_DIR}/bin
      COMMAND ${CMAKE_COMMAND} -E remove knx_iot_example_sleepy
    )


    if (NOT TARGET copy-ldrom)
        if (CASCODA_BM_INTERFACE STREQUAL "USB")
        add_custom_target(copy-ldrom ALL DEPENDS ldrom_hid)
        add_custom_command(TARGET copy-ldrom
          POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/_deps/${CASCODA_REPO}-build/bin/ldrom_hid ${PROJECT_BINARY_DIR}/bin
          COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/_deps/${CASCODA_REPO}-build/bin/ldrom_hid.bin ${PROJECT_BINARY_DIR}/bin
        )
        elseif (CASCODA_BM_INTERFACE STREQUAL "UART")
        add_custom_target(copy-ldrom ALL DEPENDS ldrom_uart)
        add_custom_command(TARGET copy-ldrom
          POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/_deps/${CASCODA_REPO}-build/bin/ldrom_uart ${PROJECT_BINARY_DIR}/bin
          COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/_deps/${CASCODA_REPO}-build/bin/ldrom_uart.bin ${PROJECT_BINARY_DIR}/bin
        )
        endif()
    endif()

    if (CASCODA_OTA_UPGRADE_ENABLED EQUAL 1)
        if (NOT TARGET copy-ota-bootloader)
          add_custom_target(copy-ota-bootloader ALL DEPENDS ota-bootloader)
          add_custom_command(TARGET copy-ota-bootloader
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/_deps/${CASCODA_REPO}-build/bin/ota-bootloader ${PROJECT_BINARY_DIR}/bin
            COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/_deps/${CASCODA_REPO}-build/bin/ota-bootloader.bin ${PROJECT_BINARY_DIR}/bin
          )
        endif()
    endif()

endif()
