#include "../include/execute.hpp"
#include "../include/builtins.hpp"

#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <string>

void executeCommand(const std::vector<std::string>& tokens) {
    if (tokens.empty()) return;

    // Handle builtins BEFORE forking
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

    // -------- Split tokens by '|' into commands --------
    std::vector<std::vector<std::string>> cmds;
    std::vector<std::string> current;

    for (const auto &t : tokens) {
        if (t == "|") {
            cmds.push_back(current);
            current.clear();
        } else {
            current.push_back(t);
        }
    }
    cmds.push_back(current);

    int n = cmds.size();
    int pipefds[2 * (n - 1)];

    for (int i = 0; i < n - 1; i++) {
        if (pipe(pipefds + i*2) < 0) {
            perror("pipe");
            return;
        }
    }

    // -------- Spawn N processes --------
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            // CHILD

            // If not first cmd, redirect stdin to previous pipe read end
            if (i > 0) {
                dup2(pipefds[(i-1)*2], STDIN_FILENO);
            }

            // If not last cmd, redirect stdout to next pipe write end
            if (i < n - 1) {
                dup2(pipefds[i*2 + 1], STDOUT_FILENO);
            }

            // Close all pipe fds
            for (int j = 0; j < 2*(n-1); j++)
                close(pipefds[j]);

            // Build argv array for execvp
            std::vector<char*> argv;
            for (auto &s : cmds[i])
                argv.push_back((char*)s.c_str());
            argv.push_back(nullptr);

            // Execute
            if (execvp(argv[0], argv.data()) < 0) {
                perror("execvp");
                exit(1);
            }
        }
    }

    // PARENT closes pipe fds
    for (int j = 0; j < 2*(n-1); j++)
        close(pipefds[j]);

    // Wait for all children
    for (int i = 0; i < n; i++)
        wait(NULL);
}
