#include <catch2/catch_all.hpp>
#include <dpll_io.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

TEST_CASE("E2E: solve DIMACS and print solution", "[e2e]") {
    std::filesystem::path temp_file = std::filesystem::temp_directory_path() / "dpll_e2e.cnf";
    {
        std::ofstream out(temp_file);
        out << "p cnf 2 2\n";
        out << "1 0\n";
        out << "2 0\n";
    }

    std::ostringstream capture;
    std::streambuf* old_buf = std::cout.rdbuf(capture.rdbuf());

    io::solve(temp_file);

    std::cout.rdbuf(old_buf);

    std::string output = capture.str();
    REQUIRE(output.find("Solution") != std::string::npos);
    REQUIRE(output.find("1: true") != std::string::npos);
    REQUIRE(output.find("2: true") != std::string::npos);
}
