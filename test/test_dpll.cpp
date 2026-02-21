#include <catch2/catch_all.hpp>
#include <solver.h>
#include "test_helper.h"

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

TEST_CASE("DPLL solver handles 4 variable CNF with multiple solutions") {
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


TEST_CASE("DPLL solver handles pigeonhole principle") {
    CNF cnf;
    cnf.num_vars = 6;
    cnf.clauses = {
        {{1, 2}, 0, 1},
        {{3, 4}, 0, 1},
        {{5, 6}, 0, 1},

        {{-1, -3}, 0, 1},
        {{-1, -5}, 0, 1},
        {{-3, -5}, 0, 1},

        {{-2, -4}, 0, 1},
        {{-2, -6}, 0, 1},
        {{-4, -6}, 0, 1}
    };
    Solver dpll(cnf);
    bool result = dpll.solve();
    REQUIRE_FALSE(result);
}

TEST_CASE("DPLL solver handles empty CNF") {
    CNF cnf;
    cnf.num_vars = 0;
    cnf.clauses = {};
    Solver dpll(cnf);
    bool result = dpll.solve();
    REQUIRE(result);
}

TEST_CASE("Unit propagation chain") {
    CNF cnf = unit_chain(5);
    Solver dpll(cnf);
    bool result = dpll.solve();
    REQUIRE(result);
}

TEST_CASE("Unit propagation detects conflict") {
    CNF cnf;
    cnf.num_vars = 2;
    cnf.clauses = {
        {{1}, 0},
        {{-1, 2}, 0, 1},
        {{-2}, 0},
    };
    Solver dpll(cnf);
    bool result = dpll.solve();
    REQUIRE_FALSE(result);
}

TEST_CASE("Pure literal elimination - single positive pure literal") {
    CNF cnf;
    cnf.num_vars = 3;
    cnf.clauses = {
        {{1, 2}, 0, 1},
        {{1, -2}, 0, 1},
        {{2, 3}, 0, 1},
        {{-2, 3}, 0, 1}
    };
    
    Solver solver(cnf);
    
    std::vector<int> assignment = solver.get_assignment();
    
    REQUIRE(assignment[0] == 1);
    REQUIRE(assignment[2] == 1);
    REQUIRE(solver.get_num_clauses() == 0);
}

TEST_CASE("Pure literal elimination - single negative pure literal") {
    CNF cnf;
    cnf.num_vars = 3;
    cnf.clauses = {
        {{-1, 2}, 0, 1},
        {{-1, -2}, 0, 1},
        {{2, -3}, 0, 1},
        {{-2, 3}, 0, 1}
    };
    
    Solver solver(cnf);
    
    std::vector<int> assignment = solver.get_assignment();
    REQUIRE(assignment[0] == 0);
    REQUIRE(solver.get_num_clauses() == 2);
}

TEST_CASE("Pure literal elimination - mixed polarities not eliminated") {
    CNF cnf;
    cnf.num_vars = 3;
    cnf.clauses = {
        {{1, -2}, 0, 1},
        {{-1, 2}, 0, 1},
        {{3}, 0, -1},
        {{-2, 3}, 0, 1}
    };
    
    Solver solver(cnf);
    
    std::vector<int> assignment = solver.get_assignment();
    
    REQUIRE(assignment[0] == -1);
    REQUIRE(assignment[1] == -1);
    REQUIRE(assignment[2] == 1);
    REQUIRE(solver.get_num_clauses() == 2);
}

TEST_CASE("Pure literal elimination - all variables pure") {
    CNF cnf;
    cnf.num_vars = 3;
    cnf.clauses = {
        {{1, 2}, 0, 1},
        {{1, 3}, 0, 1},
        {{2, 3}, 0, 1}
    };
    
    Solver solver(cnf);
    
    std::vector<int> assignment = solver.get_assignment();
    
    REQUIRE(assignment[0] == 1);
    REQUIRE(assignment[1] == 1);
    REQUIRE(assignment[2] == 1);
    REQUIRE(solver.get_num_clauses() == 0);
}

TEST_CASE("Pure literal elimination - simplifies problem") {
    CNF cnf;
    cnf.num_vars = 4;
    cnf.clauses = {
        {{1, 2, 3}, 0, 2},
        {{1, -2, 4}, 0, 2},
        {{-2, 3}, 0, 1},
        {{2, -3}, 0, 1}
    };
    
    Solver solver(cnf);
    
    std::vector<int> assignment = solver.get_assignment();
    
    REQUIRE(assignment[0] == 1);
    REQUIRE(assignment[3] == 1);
    REQUIRE(assignment[1] == -1);
    REQUIRE(assignment[2] == -1);
    REQUIRE(solver.get_num_clauses() == 2);
}

TEST_CASE("Pure literal elimination - no pure literals") {
    CNF cnf;
    cnf.num_vars = 2;
    cnf.clauses = {
        {{1, 2}, 0, 1},
        {{-1, -2}, 0, 1},
        {{1, -2}, 0, 1},
        {{-1, 2}, 0, 1}
    };
    
    Solver solver(cnf);
    
    std::vector<int> assignment = solver.get_assignment();
    
    REQUIRE(assignment[0] == -1);
    REQUIRE(assignment[1] == -1);
    REQUIRE(solver.get_num_clauses() == 4);
}