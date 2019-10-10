cmake_minimum_required(VERSION 3.0.0)

project(OpenParkingFinderHub)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_SOURCE_DIR}/common/include/)

find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBUV REQUIRED libuv)

include_directories(${LIBUV_INCLUDE_DIRS})
add_subdirectory(./hub)
