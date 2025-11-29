#include "parser.hpp"
#include <sstream>

// Very simple whitespace tokenizer.
// Does NOT handle quotes or escapes yet.
std::vector<std::string> parseInput(const std::string &input) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string tok;
    while (iss >> tok) {
        tokens.push_back(tok);
    }
    return tokens;
}


