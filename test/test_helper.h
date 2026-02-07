#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include <data_types.h>

inline CNF unit_chain(int n) {
    CNF cnf;
    cnf.num_vars = n;

    cnf.clauses.push_back({{1}, 0, 0});
    for (int i = 1; i < n; i++) {
        cnf.clauses.push_back({{-i, i + 1}, 0, 1});
    }

    return cnf;
}

#endif