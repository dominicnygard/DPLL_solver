#include <filesystem>
#include <iostream>
#include <parser.h>
#include <solver.h>

namespace io {
    void solve(const std::filesystem::path& filepath);
    void print_results(std::vector<int> assignments, bool result);
}