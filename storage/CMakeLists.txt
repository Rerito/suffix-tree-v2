# Copyright (c) 2021, Rerito
# SPDX-License-Identifier: MIT

add_library(penjing-storage INTERFACE)

target_include_directories(
    penjing-storage
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(
    penjing-storage
    INTERFACE
        penjing-meta)

add_subdirectory(test)
