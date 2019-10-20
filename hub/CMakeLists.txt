cmake_minimum_required(VERSION 3.0.0)

FILE(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/hub/src/*.cpp)

pkg_check_modules(SQLITE REQUIRED sqlite3)

add_executable(OpenParkingManagerHub ${SRC_FILES})

target_link_libraries(OpenParkingManagerHub ${LIBUV_LIBRARIES} ${SQLITE_LIBRARIES} OpenParkingManagerCommonLib)
