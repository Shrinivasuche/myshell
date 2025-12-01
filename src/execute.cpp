#include "../include/execute.hpp"
#include "../include/builtins.hpp"

#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;


// ----------------------------------------------------
//  REDIRECTION HANDLER  (<, >, >>)
// ----------------------------------------------------
void handle_redirection(vector<string> &cmd) {
    for (size_t i = 0; i < cmd.size(); i++) {
        if (cmd[i] == ">") {
            int fd = open(cmd[i+1].c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);

            cmd.erase(cmd.begin() + i, cmd.begin() + i + 2);
            i--;
        }
        else if (cmd[i] == ">>") {
            int fd = open(cmd[i+1].c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);

            cmd.erase(cmd.begin() + i, cmd.begin() + i + 2);
            i--;
        }
        else if (cmd[i] == "<") {
            int fd = open(cmd[i+1].c_str(), O_RDONLY);
            if (fd < 0) { perror("open"); return; }

            dup2(fd, STDIN_FILENO);
            close(fd);

            cmd.erase(cmd.begin() + i, cmd.begin() + i + 2);
            i--;
        }
    }
}


// ----------------------------------------------------
//  MAIN EXECUTION FUNCTION
// ----------------------------------------------------
void executeCommand(const vector<string>& tokens) {
    if (tokens.empty()) return;

    // BUILTINS (must run in parent)
    if (tokens[0] == "cd") { builtin_cd(tokens); return; }
    if (tokens[0] == "exit") { builtin_exit(); return; }
    if (tokens[0] == "help") { builtin_help(); return; }


    // ------------------------------------------------
    // SPLIT TOKENS BY PIPES
    // ------------------------------------------------
    vector<vector<string>> cmds;
    vector<string> current;

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


    // ================================================================
    // CASE 1 — NO PIPES (single command with/without redirection)
    // ================================================================
    if (n == 1) {
        pid_t pid = fork();

        if (pid == 0) {
            // CHILD → apply <, >, >> redirection
            vector<string> single = cmds[0];
            handle_redirection(single);

            // Build argv
            vector<char*> argv;
            for (auto &s : single) argv.push_back((char*)s.c_str());
            argv.push_back(nullptr);

            execvp(argv[0], argv.data());
            perror("execvp");
            exit(1);
        }

        wait(NULL);
        return;
    }


    // ================================================================
    // CASE 2 — MULTIPLE PIPES
    // ================================================================
    int pipefds[2 * (n - 1)];

    // Create N−1 pipes
    for (int i = 0; i < n - 1; i++) {
        if (pipe(pipefds + 2*i) < 0) {
            perror("pipe");
            return;
        }
    }


    // --------------------------------------------------------------
    // FORK N PROCESSES
    // --------------------------------------------------------------
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            // ---------------- CHILD ----------------

            // 1. Redirect input (if not first command)
            if (i > 0) {
                dup2(pipefds[(i-1)*2], STDIN_FILENO);
            }

            // 2. Redirect output (if not last command)
            if (i < n - 1) {
                dup2(pipefds[i*2 + 1], STDOUT_FILENO);
            }

            // 3. ALWAYS close all pipe FDs
            for (int j = 0; j < 2*(n-1); j++)
                close(pipefds[j]);

            // 4. APPLY REDIRECTION INSIDE PIPES
            vector<string> cmdCopy = cmds[i];
            handle_redirection(cmdCopy);

            // 5. Build execvp argv
            vector<char*> argv;
            for (auto &s : cmdCopy)
                argv.push_back((char*)s.c_str());
            argv.push_back(nullptr);

            execvp(argv[0], argv.data());
            perror("execvp");
            exit(1);
        }
    }

    // ---------------- PARENT ----------------
    for (int j = 0; j < 2*(n-1); j++)
        close(pipefds[j]);

    for (int i = 0; i < n; i++)
        wait(NULL);
}
