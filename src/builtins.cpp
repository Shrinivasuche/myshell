#include "../include/builtins.hpp"
#include <iostream>
#include <unistd.h>

void builtin_cd(const std::vector<std::string>& tokens) {
    if (tokens.size() < 2) {
        std::cerr << "cd: missing argument\n";
        return;
    }
    if (chdir(tokens[1].c_str()) != 0) {
        perror("cd");
    }
}

void builtin_exit() {
    std::cout << "Exiting shell...\n";
    exit(0);
}

void builtin_help() {
    std::cout << "Supported commands:\n"
              << "  cd <dir>      - change directory\n"
              << "  exit          - exit the shell\n"
              << "  help          - show this help\n"
              << "  External commands (ls, pwd, echo, etc.)\n";
}
