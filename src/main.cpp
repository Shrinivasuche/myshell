#include "../include/parser.hpp"
#include "../include/execute.hpp"
#include <iostream>

int main() {
    while (true) {
        std::cout << "myshell> ";
        std::string input;
        std::getline(std::cin, input);

        auto tokens = parseInput(input);

        // Debug print (optional)
        for (const auto &t : tokens) {
            std::cout << "Token: " << t << std::endl;
        }

        executeCommand(tokens);
    }
}
