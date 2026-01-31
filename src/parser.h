#include "data_types.h"
#include <string>
#include <filesystem>

namespace parser {
    CNF load_dimacs(const std::filesystem::path& filename);
}