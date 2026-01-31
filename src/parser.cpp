#include "parser.h"
#include <fstream>
#include <iostream>
#include <sstream>

CNF parser::load_dimacs(const std::filesystem::path &filename)
{
    CNF data;

    std::string line;

    std::ifstream f(filename);

    if (!f.is_open()) {
        throw std::invalid_argument("Failed to read file name, make sure the filepath is correct!");
    }

    while(getline(f, line)) {
        if (line.empty() || line[0] == 'c') {
            continue;
        }

        if (line[0] == 'p') {
            std::istringstream iss(line);
            std::string format, p;
            iss >> p >> format >> data.num_vars;
            int num_clauses;
            iss >> num_clauses;
            data.clauses.reserve(num_clauses);
            continue; 
        }

        std::istringstream iss(line);
        Clause clause;
        int lit;

        while (iss >> lit) {
            if (lit == 0)
                break;
            clause.literals.push_back(lit);
        }

        if (!clause.literals.empty()) {
            data.clauses.push_back(std::move(clause));
        }
    }

    return data;
}

