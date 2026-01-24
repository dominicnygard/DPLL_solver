#include <iostream>
#include "dpll.h"

int main(int argc, char *argv[]) {
    std::vector<Clause> formulas;

    formulas.emplace_back(Clause{std::vector<int>{1, 2}, 2, false});
    formulas.emplace_back(Clause{std::vector<int>{-1, 3}, 2, false});
    formulas.emplace_back(Clause{std::vector<int>{-2, -3}, 2, false});

    DPLL dpll(formulas);
    std::unordered_map<int, bool> result = dpll.solve();

    for (auto it : result) {
        std::cout << "key: " << it.first << " value: " << it.second << std::endl;
    }

    return 0;
}

