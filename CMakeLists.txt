# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(shift_register C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(shift_register main.c)

# pull in common dependencies
target_link_libraries(shift_register 
        pico_stdlib 
        hardware_i2c
        pico_cyw43_arch_none
        hardware_adc
        )

# enable usb output, disable uart output
pico_enable_stdio_usb(shift_register 1)
pico_enable_stdio_uart(shift_register 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(shift_register)

# add url via pico_set_program_url
#example_auto_set_url(barometer)