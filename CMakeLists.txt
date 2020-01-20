cmake_minimum_required(VERSION 3.0.0)

project(OpenParkingFinderHub)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_TARGET_HUB true CACHE BOOL "Build the hub")
set(BUILD_TARGET_COMMON true CACHE BOOL "Build the common libs Required for Hub and Node")
set(BUILD_TARGET_GATE_ENTRY false CACHE BOOL "Build the entry system")
set(BUILD_TARGET_NODE false CACHE BOOL "Build the node")
if (NOT EXISTS ${CMAKE_SOURCE_DIR}/deps/include/json.hpp)
    file(DOWNLOAD https://github.com/nlohmann/json/releases/download/v3.7.0/json.hpp ${CMAKE_SOURCE_DIR}/deps/include/json.hpp)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(COMMON_LIBS REQUIRED opencv4 tesseract)

include_directories("${COMMON_LIBS_INCLUDE_DIRS}")
include_directories("${CMAKE_SOURCE_DIR}/common/include/")
include_directories("${CMAKE_SOURCE_DIR}/deps/include/")
include_directories("${PROJECT_SOURCE_DIR}/deps/LouisNet/include/")

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