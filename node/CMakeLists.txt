cmake_minimum_required(VERSION 3.0.0)

add_executable(OpenParkingManagerNode ${PROJECT_SOURCE_DIR}/node/src/main.cpp)

target_link_libraries(OpenParkingManagerNode pthread cppgpio OpenParkingManagerCommonLib)
