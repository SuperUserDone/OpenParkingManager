cmake_minimum_required(VERSION 3.0.0)

add_executable(OpenParkingManagerEntry ${PROJECT_SOURCE_DIR}/entry/src/main.cpp)

target_link_libraries(OpenParkingManagerEntry OpenParkingManagerCommonLib pthread)