#ifndef SOLVER_H
#define SOLVER_H

#include "data_types.h"

class Solver {
private:
    std::vector<Clause> clauses;
    std::vector<std::vector<int>> watch_list;
    std::vector<int> backtrack;
    std::vector<int> assignment;
    int num_variables = 0;
    size_t propagate_idx = 0;
    Stats runtime_stats;
    
public:
    Solver(CNF clauses);
    bool solve();
    bool propagate();
    void set_watches();
    std::vector<int> get_assignment();
    Stats get_stats();
};

#endif