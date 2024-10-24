set (MOUDLE_DIR ${CMAKE_SOURCE_DIR}/ext/serial)

include(FetchContent)
FetchContent_Declare(
    serial
    GIT_REPOSITORY https://github.com/wjwwood/serial.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    SOURCE_DIR ${MOUDLE_DIR}
    SOURCE_SUBDIR /tmp
)
FetchContent_MakeAvailable(serial)

add_library(serial ${MOUDLE_DIR}/src/serial.cc ${MOUDLE_DIR}/src/impl/unix.cc ${MOUDLE_DIR}/src/impl/list_ports/list_ports_linux.cc)

target_include_directories(serial PUBLIC ${MOUDLE_DIR}/include)
target_link_libraries(serial PRIVATE rt pthread)
