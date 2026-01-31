#include <dpll_io.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <inputfile>\n";
        return 1;
    }
    io::solve(std::filesystem::path(argv[1]));

    return 0;
}

