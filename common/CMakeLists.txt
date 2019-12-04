cmake_minimum_required(VERSION 3.0.0)

pkg_check_modules(COMMON_LIBS REQUIRED opencv4 tesseract)

add_library(OpenParkingManagerCommonLib ${CMAKE_SOURCE_DIR}/common/src/ReadLicensePlates.cpp)

target_link_libraries(OpenParkingManagerCommonLib ${COMMON_LIBS_LIBRARIES})