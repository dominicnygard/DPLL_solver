set(LIB_SOURCES
    dpll.cpp
)

add_library(DPLL_solver_lib ${LIB_SOURCES})

target_include_directories(DPLL_solver_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(DPLL_solver main.cpp)
target_link_libraries(DPLL_solver PRIVATE DPLL_solver_lib)