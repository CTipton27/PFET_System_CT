# Copyright (C) 2023 - 2024 Advanced Micro Devices, Inc.  All rights reserved.
# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.14.7)

include(${CMAKE_CURRENT_SOURCE_DIR}/Freertos_hello_worldExample.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/UserConfig.cmake)
set(APP_NAME test_freertos_led)
project(${APP_NAME})

find_package(common)
enable_language(C ASM)
collect(PROJECT_LIB_DEPS xilstandalone;xiltimer)






































































































































































































































































































































































































































































































collect(PROJECT_LIB_DEPS freertos)
collect(PROJECT_LIB_DEPS xil)
collect(PROJECT_LIB_DEPS xiltimer)
collect(PROJECT_LIB_DEPS gcc)
collect(PROJECT_LIB_DEPS c)
collector_list (_deps PROJECT_LIB_DEPS)
list (APPEND _deps ${USER_LINK_LIBRARIES})

if (CMAKE_EXPORT_COMPILE_COMMANDS AND
    (NOT ${YOCTO}))
    set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
    set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES})
endif()
list (APPEND _sources ${USER_COMPILE_SOURCES})
linker_gen("${CMAKE_CURRENT_SOURCE_DIR}/linker_files/")
string(APPEND CMAKE_C_FLAGS ${USER_COMPILE_OPTIONS})
string(APPEND CMAKE_CXX_FLAGS ${USER_COMPILE_OPTIONS})
string(APPEND CMAKE_C_LINK_FLAGS ${USER_LINK_OPTIONS})
string(APPEND CMAKE_CXX_LINK_FLAGS ${USER_LINK_OPTIONS})
add_dependency_on_bsp(_sources)
add_executable(${APP_NAME}.elf ${_sources})
set_target_properties(${APP_NAME}.elf PROPERTIES LINK_DEPENDS ${USER_LINKER_SCRIPT})
target_link_libraries(${APP_NAME}.elf -Wl,-T -Wl,\"${USER_LINKER_SCRIPT}\" -L\"${CMAKE_SOURCE_DIR}/\" -L\"${CMAKE_LIBRARY_PATH}/\" -L\"${USER_LINK_DIRECTORIES}/\" -Wl,--start-group,-l${_deps} -Wl,--end-group)
target_compile_definitions(${APP_NAME}.elf PUBLIC ${USER_COMPILE_DEFINITIONS})
target_include_directories(${APP_NAME}.elf PUBLIC ${USER_INCLUDE_DIRECTORIES})
print_elf_size(CMAKE_SIZE ${APP_NAME})
