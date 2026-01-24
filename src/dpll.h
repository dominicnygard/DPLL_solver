#ifndef DPLL_H
#define DPLL_H

#include <vector>
#include <unordered_map>
#include <optional>

struct Clause {
    std::vector<int> literals;
    int unassigned_count;
    bool satisfied;
};

struct TrailEntry {
    int clause_id;
    bool prev_satisfied;
    int prev_unassigned;
};

class DPLL {
private:
    std::vector<Clause> clauses;
    int num_clauses = 0;
    std::unordered_map<int, bool> assignment;
    std::vector<TrailEntry> trail;
    std::unordered_map<int, int> trail_markers;

public:
    DPLL(std::vector<Clause> clauses);
    std::unordered_map<int, bool> solve();
    bool dpll_search();
    std::optional<int> find_unit_clause();
    std::optional<int> find_pure_literal();
    bool assign(int var, bool value);
    void unassign(int var);
    void undo_to_marker(int var);

    std::optional<int> choose_variable();
};

#endif