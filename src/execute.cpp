#include "../include/execute.hpp"
#include "../include/builtins.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

void executeCommand(const std::vector<std::string>& tokens) {

    if (tokens.empty()) return;

    // Handle built-in commands first
    if (tokens[0] == "cd") {
        builtin_cd(tokens);
        return;
    }
    if (tokens[0] == "exit") {
        builtin_exit();
        return;
    }
    if (tokens[0] == "help") {
        builtin_help();
        return;
    }

    // Handle external commands
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        std::vector<char*> argv;
        for (const auto& token : tokens) {
            argv.push_back(const_cast<char*>(token.c_str()));
        }
        argv.push_back(nullptr);

        execvp(argv[0], argv.data());
        std::cerr << "execvp failed: command not found\n";
        exit(1);
    } else if (pid > 0) {
        waitpid(pid, nullptr, 0);  // Parent waits
    } else {
        std::cerr << "Fork failed!\n";
    }
}
