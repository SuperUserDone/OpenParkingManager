cmake_minimum_required(VERSION 3.0.0)

pkg_check_modules(COMMON_LIBS REQUIRED opencv4)

add_library(OpenParkingManagerCommonLib ${CMAKE_SOURCE_DIR}/common/src/ReadLicensePlates.cpp)

include_directories(${COMMON_LIBS_INCLUDE_DIRS})

target_link_libraries(OpenParkingManagerCommonLib ${COMMON_LIBS_LIBRARIES})