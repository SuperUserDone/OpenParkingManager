cmake_minimum_required(VERSION 3.0.0)

project(OpenParkingFinderHub)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_TARGET_HUB true CACHE BOOL "Build the hub")
set(BUILD_TARGET_COMMON true CACHE BOOL "Build the common libs Required for Hub and Node")
set(BUILD_TARGET_GATE_ENTRY false CACHE BOOL "Build the entry system")
set(BUILD_TARGET_NODE false CACHE BOOL "Build the node")

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/deps/include/sigs.h)
    file(DOWNLOAD https://raw.githubusercontent.com/netromdk/sigs/master/sigs.h ${CMAKE_SOURCE_DIR}/deps/include/sigs.h)
endif()

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/deps/include/json.hpp)
    file(DOWNLOAD https://github.com/nlohmann/json/releases/download/v3.7.0/json.hpp ${CMAKE_SOURCE_DIR}/deps/include/json.hpp)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUV REQUIRED libuv)

include_directories("${CMAKE_SOURCE_DIR}/common/include/")
include_directories(${CMAKE_SOURCE_DIR}/deps/include/)
include_directories(${LIBUV_INCLUDE_DIRS})

if(BUILD_TARGET_COMMON)
    add_subdirectory(./common)

    if(BUILD_TARGET_HUB)
        add_subdirectory(./hub)
    endif()

    if(BUILD_TARGET_NODE)
        add_subdirectory(./node)
    endif()
endif()

if(BUILD_TARGET_GATE_ENTRY)
    add_subdirectory(./entry)
endif()