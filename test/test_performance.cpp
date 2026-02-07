#include <catch2/catch_all.hpp>
#include "test_helper.h"
#include <solver.h>
#include <parser.h>
#include <chrono>
#include <iostream>
#include <set>
#include <filesystem>

TEST_CASE("Solver performance for unit propagation") {
    CNF cnf = unit_chain(100000);
    Solver solver(cnf);
    bool result = solver.solve();
    REQUIRE(result);
    Stats stats = solver.get_stats();
    REQUIRE(stats.decisions == 0);
    REQUIRE(stats.backtracks == 0);
}

TEST_CASE("Solver performance for single decision with two conflicts") {
    CNF cnf;
    cnf.num_vars = 2;
    cnf.clauses = {
        {{1, 2}, 0, 1},
        {{1, -2}, 0, 1},
        {{-1, 2}, 0, 1},
        {{-1, -2}, 0, 1}
    };

    Solver solver(cnf);
    bool result = solver.solve();
    REQUIRE_FALSE(result);

    Stats stats = solver.get_stats();
    REQUIRE(stats.decisions == 1);
    REQUIRE(stats.backtracks == 2);
    REQUIRE(stats.propagations == 3);
}

TEST_CASE("Benchmark", "[.][benchmark]") {
    std::filesystem::path cnf_file = "cnf/prime4294967297.cnf";
    
    if (!std::filesystem::exists(cnf_file)) {
        SKIP("Benchmark file not found: " << cnf_file);
    }
    
    CNF cnf = parser::load_dimacs(cnf_file);
    
    auto start = std::chrono::high_resolution_clock::now();
    Solver solver(cnf);
    bool result = solver.solve();
    auto end = std::chrono::high_resolution_clock::now();
    
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    Stats stats = solver.get_stats();
    
    std::cout << "\n=== Benchmark Results ===\n";
    std::cout << "File: " << cnf_file << "\n";
    std::cout << "Problem: " << cnf.num_vars << " variables, " << cnf.clauses.size() << " clauses\n";
    std::cout << "Result: " << (result ? "SAT" : "UNSAT") << "\n";
    std::cout << "Time: " << ms << " ms\n";
    std::cout << "Decisions: " << stats.decisions << "\n";
    std::cout << "Propagations: " << stats.propagations << "\n";
    std::cout << "Backtracks: " << stats.backtracks << "\n";
    std::cout << "=========================\n";
}