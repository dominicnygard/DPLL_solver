#include <solver.h>
#include <set>
#include <algorithm>
#include <iostream>

Solver::Solver(CNF clauses)
{
    this->clauses = std::move(clauses.clauses);
    num_variables = clauses.num_vars;
    set_watches();
}

bool Solver::solve()
{
    if (!propagate())
        return false;

    int var = -1;
    for (int i = 0; i < assignment.size(); i++) {
        if (assignment[i] == -1) {
            var = i;
            break;
        }
    }

    if (var == -1)
        return true;

    size_t decision_trail_size = backtrack.size();
    size_t decision_prop_idx = propagate_idx;

    backtrack.push_back(var + 1);
    assignment[var] = 1;

    if (solve())
        return true;

    while (backtrack.size() > decision_trail_size) {
        int lit = backtrack.back();
        assignment[abs(lit) - 1] = -1;
        backtrack.pop_back();
    }
    propagate_idx = decision_prop_idx;

    backtrack.push_back(-(var + 1));
    assignment[var] = 0;

    if (solve())
        return true;

    while (backtrack.size() > decision_trail_size) {
        int lit = backtrack.back();
        assignment[abs(lit) - 1] = -1;
        backtrack.pop_back();
    }
    propagate_idx = decision_prop_idx;

    return false;

}

int literal_id(int literal) {
    int variable = abs(literal) - 1;
    int sign = (literal < 0);
    return 2 * variable + sign;
}

void Solver::set_watches() {
    watch_list.resize(2 * num_variables);
    //std::cout << watch_list.size() << '\n';
    assignment.resize(num_variables);
    fill(assignment.begin(), assignment.end(), -1);

    for (int cid = 0; cid < clauses.size(); cid++) {
        Clause& c = clauses[cid];

        if (c.literals.size() >= 2) {
            c.watch1 = 0;
            c.watch2 = 1;

            watch_list[literal_id(c.literals[0])].push_back(cid);
            watch_list[literal_id(c.literals[1])].push_back(cid);
        } else if (c.literals.size() == 1) {
            c.watch1 = 0;
            c.watch2 = -1;

            watch_list[literal_id(c.literals[0])].push_back(cid);

            backtrack.push_back(c.literals[0]);
            assignment[abs(c.literals[0]) - 1] = c.literals[0] > 0 ? 1 : 0;
        }
    }
}

std::vector<int> Solver::get_assignment()
{
    return assignment;
}

bool Solver::propagate()
{
    while (propagate_idx < backtrack.size()) {
        int l = backtrack[propagate_idx++];
        int false_literal = -l;

        int wid = literal_id(false_literal);
    
        for (size_t idx = 0; idx < watch_list[wid].size(); ) {
            int cid = watch_list[wid][idx];

            Clause& clause = clauses[cid];
    
            int other_watch_index = (clause.literals[clause.watch1] == false_literal) ? clause.watch2 : clause.watch1;
            bool watch_moved = false;
    
            for (size_t i = 0; i < clause.literals.size(); i++) {
                if (i == clause.watch1 || i == clause.watch2) continue;
                int literal = clause.literals[i];
                int variable = abs(literal)-1;
    
                if (assignment[variable] == -1 || assignment[variable] == (literal > 0)) {
                    if (clause.literals[clause.watch1] == false_literal) {
                        clause.watch1 = i;
                    } else {
                        clause.watch2 = i;
                    }

                    watch_list[wid][idx] = watch_list[wid].back();
                    watch_list[wid].pop_back();
                    watch_list[literal_id(literal)].push_back(cid);
    
                    watch_moved = true;
                    break;
                }
            }
    
            if (!watch_moved) {
                int other_watch_variable = abs(clause.literals[other_watch_index]) - 1;

                if (assignment[other_watch_variable] == -1) {
                    int unit_literal = clause.literals[other_watch_index];
                    assignment[other_watch_variable] = (unit_literal > 0) ? 1 : 0;
                    backtrack.push_back(unit_literal);
                } else if (assignment[other_watch_variable] != (clause.literals[other_watch_index] > 0)) {
                    return false;
                }
                idx++;
            }
        }
    }
    return true;
}