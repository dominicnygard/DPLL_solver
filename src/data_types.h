#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <vector>
#include <cstdint>

struct Clause {
    std::vector<int> literals;
    int watch1, watch2;
};

struct CNF {
    int num_vars;
    std::vector<Clause> clauses;
};

struct Stats {
    uint64_t decisions = 0;
    uint64_t propagations = 0;
    uint64_t backtracks = 0;
};

#endif