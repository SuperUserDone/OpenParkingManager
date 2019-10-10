cmake_minimum_required(VERSION 3.0.0)

FILE(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/hub/src/*.cpp)
add_executable(OpenParkingFinderHub ${SRC_FILES})

target_link_libraries(OpenParkingFinderHub ${LIBUV_LIBRARIES})
