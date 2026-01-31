#include <dpll_io.h>


void io::solve(const std::filesystem::path& filepath) {
    CNF clauses;

    try
    {
        clauses = parser::load_dimacs(filepath);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(EXIT_FAILURE);
    }
    
    Solver solver(clauses);

    bool result = solver.solve();

    print_results(solver.get_assignment(), result);
}

void io::print_results(std::vector<int> assignments, bool result) {
    if (result) {
        std::cout << "------------------------\n";
        std::cout << "         Solution        \n";
        for (int i = 0; i < assignments.size(); i++) {
            std::cout << i + 1 << ": " << (assignments[i] ? "true" : "false") << "\n";
        }
    } else {
        std::cout << "------------------------\n";
        std::cout << "         Results        \n";
        std::cout << "Problem is not solvable.\n";
    }
}
