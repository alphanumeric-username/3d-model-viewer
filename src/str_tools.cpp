#include "str_tools.hpp"

namespace app::str_tools {

bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

std::string ltrim(std::string raw) {
    int i;
    for(i = 0; i < raw.length() && is_whitespace(raw.at(i)); i++);

    return std::string{ raw.substr(i) };
}

std::string rtrim(std::string raw) {
    int i;
    for(i = raw.length() - 1; i > -1 && is_whitespace(raw.at(i)); i--);

    return std::string{ raw.substr(0, i+1) };
}

std::string trim(std::string raw) {
    return ltrim(rtrim(raw));
}

}
