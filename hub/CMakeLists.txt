cmake_minimum_required(VERSION 3.0.0)

FILE(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/hub/src/*.cpp)

pkg_check_modules(SQLITE REQUIRED sqlite3)

include_directories(${PROJECT_SOURCE_DIR}/hub/src/)

add_executable(OpenParkingManagerHub ${SRC_FILES})

target_link_libraries(OpenParkingManagerHub ${SQLITE_LIBRARIES} OpenParkingManagerCommonLib pthread ${COMMON_LIBS})
