#include <catch2/catch_all.hpp>
#include "dpll.h"

TEST_CASE("DPLL solver returns correct answer") {
    std::vector<Clause> formulas;

    formulas.emplace_back(Clause{std::vector<int>{1, -2}, 2, false});
    formulas.emplace_back(Clause{std::vector<int>{2, 3}, 2, false});
    formulas.emplace_back(Clause{std::vector<int>{-1, -3}, 2, false});

    DPLL dpll(formulas);
    std::unordered_map<int, bool> result = dpll.solve();

    REQUIRE(!result.empty());
    REQUIRE(result[1] == false);
    REQUIRE(result[2] == false);
    REQUIRE(result[3] == true);
}