#include <iostream>
#include <string>
#include "../include/parser.hpp"

int main() {
    std::string input;

    while (true) {
        std::cout << "myshell> ";
        std::getline(std::cin, input);

        if (input == "exit") break;

        auto tokens = parseInput(input);
        for (auto &t : tokens) {
            std::cout << "Token: " << t << std::endl;
        }
    }

    return 0;
}
