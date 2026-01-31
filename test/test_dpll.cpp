#include <catch2/catch_all.hpp>
#include <solver.h>

TEST_CASE("DPLL solver returns correct answer") {
    CNF cnf;

    cnf.num_vars = 3;
    cnf.clauses = {
        {{1, -2}, 0, 1},
        {{2, 3}, 0, 1},
        {{-1, -3}, 0, 1}
    };

    Solver dpll(cnf);
    bool result = dpll.solve();

    REQUIRE(result);

    std::vector<int> assignment = dpll.get_assignment();

    REQUIRE(assignment[0] == true);
    REQUIRE(assignment[1] == true);
    REQUIRE(assignment[2] == false);
}

TEST_CASE("DPLL solver handles unsatisfiable CNF") {
    CNF cnf;
    cnf.num_vars = 3;
    cnf.clauses = {
        {{1, 2}, 0, 1},
        {{-1}, 0, -1},
        {{-2}, 0, -1},
        {{3}, 0, 1},
        {{-3}, 0, -1}
    };
    Solver dpll(cnf);
    bool result = dpll.solve();
    REQUIRE_FALSE(result);
}

TEST_CASE("DPLL solver handles 4-variable CNF with multiple solutions") {
    CNF cnf;
    cnf.num_vars = 4;
    cnf.clauses = {
        {{1, 2}, 0, 1},
        {{-1, 3}, 0, 1},
        {{-2, 4}, 0, 1},
        {{-3, -4}, 0, 1}
    };
    Solver dpll(cnf);
    bool result = dpll.solve();
    REQUIRE(result);
    std::vector<int> assignment = dpll.get_assignment();
    REQUIRE((assignment[0] == true || assignment[1] == true));
}

/*
TEST_CASE("DPLL solver handles pigeonhole principle (unsat)") {
    CNF cnf;
    cnf.num_vars = 3;
    cnf.clauses = {
        {{1, 2}, 0, 1},  
        {{3, 4}, 0, 1},  
        {{-1, -3}, 0, 1}, 
        {{-2, -4}, 0, 1}  
    };
    Solver dpll(cnf);
    bool result = dpll.solve();
    REQUIRE_FALSE(result);
}*/