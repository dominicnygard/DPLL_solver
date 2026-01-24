#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <optional>
#include <algorithm>

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


DPLL::DPLL(std::vector<Clause> clauses)
{
    this->clauses = std::move(clauses);
    num_clauses = this->clauses.size();
}

std::unordered_map<int, bool> DPLL::solve()
{
    bool result = dpll_search();

    if (result) {
        return assignment;
    }

    return std::unordered_map<int, bool>();
}

bool DPLL::dpll_search()
{
    while (true) {
        std::optional<int> u = find_unit_clause();
        if (!u.has_value())
            break;

        int unit = *u;

        int var = abs(unit);
        bool value = unit > 0;

        if (assignment.contains(var)) {
            if (assignment[var] != value) {
                return false;
            }
            continue;
        }

        if (!assign(var, value))
            return false;
    }

    std::optional<int> p = find_pure_literal();
    if (p.has_value()) {
        int pure = *p;

        int var = abs(pure);
        int value = pure > 0;

        if (!assign(var, value))
            return false;

        if (dpll_search())
            return true;

        unassign(var);
        return false;
    }

    if (std::all_of(clauses.begin(), clauses.end(), [](Clause c) { return c.satisfied; }))
        return true;

    std::optional<int> v = choose_variable();
    if (!v.has_value())
        return true;
        
    int var = *v;

    if (assign(var, true)) {
        if (dpll_search()) {
            return true;
        }
    }

    unassign(var);

    if (assign(var, false)) {
        if (dpll_search()) {
            return true;
        }
    }

    unassign(var);
    return false;
}

std::optional<int> DPLL::find_unit_clause()
{
    for (size_t i = 0; i < clauses.size(); i++) {
        if (!clauses[i].satisfied && clauses[i].unassigned_count == 1) {
            for (int literal : clauses[i].literals) {
                if (!assignment.contains(literal)) {
                    return literal;
                }
            }
        }
    }

    return std::nullopt;
}

std::optional<int> DPLL::find_pure_literal()
{
    std::unordered_map<int, std::set<int>> literal_polarities;

    for (size_t i = 0; i < clauses.size(); i++) {
        if (clauses[i].satisfied)
            continue;

        for (int literal : clauses[i].literals) {
            int var = abs(literal);
            if (assignment.contains(var))
                continue;

            bool polarity = literal > 0;
            literal_polarities[var].insert(polarity ? 1 : 0);
        }
    }

    for (auto i : literal_polarities) {
        if (i.second.size() == 1) {
            bool polarity = (*i.second.begin() == 1);
            return polarity ? i.first : -i.first;
        }
    }

    return std::nullopt;
}

bool DPLL::assign(int var, bool value)
{
    assignment[var] = value;
    trail_markers[var] = trail.size();

    for (size_t i = 0; i < clauses.size(); i++) {
        Clause &clause = clauses[i];

        if (clause.satisfied)
            continue;

        bool clause_changed = false;

        for (int literal : clause.literals) {
            if (abs(literal) != var)
                continue;

            trail.emplace_back(TrailEntry{
                static_cast<int>(i),
                clause.satisfied,
                clause.unassigned_count
            });

            clause_changed = true;

            if ((literal > 0 && value) || (literal < 0 && !value)) {
                clause.satisfied = true;
            } else {
                clause.unassigned_count--;
            }

            break;
        }

        if (!clause.satisfied && clause.unassigned_count == 0) {
            undo_to_marker(var);
            assignment.erase(var);
            trail_markers.erase(var);
            return false;
        }
    }

    return true;
}

void DPLL::unassign(int var)
{
    if (!assignment.contains(var))
        return;

    undo_to_marker(var);

    assignment.erase(var);
    trail_markers.erase(var);
}

void DPLL::undo_to_marker(int var)
{
    if (!trail_markers.contains(var))
        return;
    
    int marker = trail_markers[var];

    while (trail.size() > marker) {
        TrailEntry entry = trail.back();
        trail.pop_back();

        clauses[entry.clause_id].satisfied = entry.prev_satisfied;
        clauses[entry.clause_id].unassigned_count = entry.prev_unassigned;
    }
}

std::optional<int> DPLL::choose_variable()
{
    std::unordered_map<int, int> var_counts;

    for (size_t i = 0; i < clauses.size(); i++) {
        if (clauses[i].satisfied)
            continue;

        for (int literal : clauses[i].literals) {
            int var = abs(literal);
            if (!assignment.contains(var))
                var_counts[var] = var_counts.find(var) != var_counts.end() ? var_counts[var] + 1 : 1;
        }
    }

    if (var_counts.empty())
        return std::nullopt;

    auto max_it = std::max_element(
        var_counts.begin(), var_counts.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; }
    );

    if (max_it != var_counts.end()) {
        return max_it->first;
    }

    return std::nullopt;
}

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

