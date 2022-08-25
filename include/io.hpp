#pragma once

#include <fstream>
#include <filesystem>
#include <vector>

namespace app {

namespace fs = std::filesystem;

std::string read_file(fs::path file_name);
std::vector<std::string> read_lines(fs::path file_name);

}