include(FetchContent)
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG master
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/ext/tomlplusplus
)
FetchContent_MakeAvailable(tomlplusplus)
