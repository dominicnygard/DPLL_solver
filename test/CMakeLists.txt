file(GLOB TEST_SOURCES *.cpp)

add_executable(DPLL_tests ${TEST_SOURCES})

target_link_libraries(DPLL_tests
    PRIVATE
    Catch2::Catch2WithMain
    DPLL_solver_lib
)

target_compile_options(DPLL_tests PRIVATE --coverage)
target_link_options(DPLL_tests PRIVATE --coverage)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(DPLL_tests)