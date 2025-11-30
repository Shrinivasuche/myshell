#include "../include/parser.hpp"
#include <vector>
#include <string>
#include <cctype>

std::vector<std::string> parseInput(const std::string &input) {
    std::vector<std::string> tokens;
    std::string current;
    bool in_single = false;
    bool in_double = false;

    for (size_t i = 0; i < input.size(); i++) {
        char c = input[i];

        // Toggle single quote mode
        if (c == '\'' && !in_double) {
            in_single = !in_single;
            current += c;   // keep the quote for awk
            continue;
        }

        // Toggle double quote mode
        if (c == '"' && !in_single) {
            in_double = !in_double;
            current += c;
            continue;
        }

        // Inside quotes => take everything literally
        if (in_single || in_double) {
            current += c;
            continue;
        }

        // Pipe
        if (c == '|') {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            tokens.push_back("|");
            continue;
        }

        // Redirection ( <, >, >> )
        if (c == '<' || c == '>') {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            if (c == '>' && i+1 < input.size() && input[i+1] == '>') {
                tokens.push_back(">>");
                i++;
            } else {
                tokens.push_back(std::string(1, c));
            }
            continue;
        }

        // Whitespace ends a token (only outside quotes)
        if (isspace(c)) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            continue;
        }

        // Normal character
        current += c;
    }

    if (!current.empty()) {
        tokens.push_back(current);
    }

    return tokens;
}
