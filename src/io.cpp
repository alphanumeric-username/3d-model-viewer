#include "io.hpp"

#include <iostream>

namespace app {

std::string read_file(fs::path file_name) {
    std::ifstream file{};
    file.open(file_name);

    if(!file.is_open()) {
        std::cerr << "[IO]: Could not read file '" << file_name << "'\n";
        return "";
    }

    std::string file_content{""};

    for(int c = file.get(); c != std::char_traits<char>::eof(); c = file.get()) {
        file_content.push_back(c);
    }
    
    file.close();

    return file_content;
}

std::vector<std::string> read_lines(fs::path file_name) {
    std::vector<std::string> lines{};
    std::ifstream file{};
    file.open(file_name);

    if(!file.is_open()) {
        std::cerr << "[IO]: Could not read file '" << file_name << "'\n";
        return lines;
    }

    std::string line{""};

    for(int c = file.get(); c != std::char_traits<char>::eof(); c = file.get()) {
        if(c == '\n') {
            lines.emplace_back(std::move(line));
            line = std::string{""};
        } else {
            line.push_back(c);
        }
    }

    lines.emplace_back(std::move(line));

    file.close();

    return lines;
}

}