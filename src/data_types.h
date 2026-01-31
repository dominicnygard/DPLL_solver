#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <vector>

struct Clause {
    std::vector<int> literals;
    int watch1, watch2;
};

struct CNF {
    int num_vars;
    std::vector<Clause> clauses;
};

#endif