include(FetchContent)
FetchContent_Declare(
    easyloggingpp
    GIT_REPOSITORY https://github.com/abumq/easyloggingpp.git
    GIT_TAG master
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/ext/easyloggingpp
)
FetchContent_MakeAvailable(easyloggingpp)

add_library(easyloggingpp ${easyloggingpp_SOURCE_DIR}/src/easylogging++.cc)
target_compile_definitions(
    easyloggingpp 
    PUBLIC ELPP_STL_LOGGING
           ELPP_NO_DEFAULT_LOG_FILE)
