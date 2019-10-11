cmake_minimum_required(VERSION 3.0.0)

project(OpenParkingFinderHub)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUV REQUIRED libuv)

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/deps/include/json.hpp)
file(DOWNLOAD https://github.com/nlohmann/json/releases/download/v3.7.0/json.hpp ${CMAKE_SOURCE_DIR}/deps/include/json.hpp)
endif()

include_directories(${CMAKE_SOURCE_DIR}/common/include/)
include_directories(${CMAKE_SOURCE_DIR}/deps/include/)

include_directories(${LIBUV_INCLUDE_DIRS})
add_subdirectory(./hub)
