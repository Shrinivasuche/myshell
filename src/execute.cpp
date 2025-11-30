#include "../include/execute.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

using namespace std;

void runSingleCommand(const vector<string>& cmd) {
    if (cmd.empty()) return;

    // Build char* array for execvp
    vector<char*> argv;
    for (const string& s : cmd) {
        argv.push_back(const_cast<char*>(s.c_str()));
    }
    argv.push_back(NULL);

    execvp(argv[0], argv.data());
    perror("execvp");
    exit(1);
}

void executeCommand(const vector<string>& tokens) {
    if (tokens.empty()) return;

    // ----------- CHECK FOR SINGLE PIPE -----------
    int pipeIndex = -1;
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "|") {
            pipeIndex = i;
            break;
        }
    }

    if (pipeIndex != -1) {
        // Split into two commands:
        vector<string> left(tokens.begin(), tokens.begin() + pipeIndex);
        vector<string> right(tokens.begin() + pipeIndex + 1, tokens.end());

        int fd[2];
        pipe(fd);

        pid_t pid1 = fork();
        if (pid1 == 0) {
            // Child 1 → write end of pipe
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            runSingleCommand(left);
        }

        pid_t pid2 = fork();
        if (pid2 == 0) {
            // Child 2 → read end of pipe
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
            runSingleCommand(right);
        }

        // Parent closes both ends
        close(fd[0]);
        close(fd[1]);

        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
        return;
    }

    // ----------- NO PIPE: run normally -----------
    runSingleCommand(tokens);
}
