cmake_minimum_required(VERSION 3.10.0)
project(DPLL_solver VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

option(ENABLE_COVERAGE "Enable test coverage reporting" OFF)
if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "Enabling code coverage flags")
    add_compile_options(-O0 -g --coverage)
    add_link_options(--coverage)
endif()

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.11.0
)
FetchContent_MakeAvailable(Catch2)

add_subdirectory(src)
add_subdirectory(test)

