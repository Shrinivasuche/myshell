# Mini Shell in C++

A lightweight UNIX-like shell implemented in C++, built as a systems programming project to learn process management, piping, redirection, and low-level OS concepts.  

This shell supports execution of external commands, built-in commands, multi-pipe operations, and input/output redirection using UNIX system calls such as `fork()`, `execvp()`, `pipe()`, `dup2()`, and `waitpid()`.

---

## 🚀 Features

### ✅ **1. Basic Command Execution**
- Executes external commands using `execvp()`
- Forks a child process for each command
- Parent waits for child using `waitpid()`
- Supports arguments, multiple tokens, and simple commands  
  Example:
    * ls -l
    * pwd
    * echo hello world

### ✅ **2. Built-In Commands**

These are executed inside the shell without spawning a new process:

| Command    | Description             |
|------------|--------------------------|
| `cd <dir>` | Change directory         |
| `exit`     | Exit the shell           |
| `help`     | List supported commands  |

### ✅ **3. Multi-Pipe Support (|)**

Implements chaining of multiple commands using pipes.

Examples:

* ls -l | grep cpp | wc -l
* echo "hello" | tr a-z A-Z | rev
* cat input.txt | sort | uniq


Pipeline handling includes:

* Creating multiple pipes dynamically
* Redirecting stdin/stdout using dup2()
* Closing unused pipe ends
* Waiting for all processes in the pipeline

### ✅ **4. Input/Output Redirection (<, >, >>)**

Supports input redirection (`<`), output redirection (`>`), and append redirection (`>>`).

Examples:

* ls -l > output.txt
* cat input.txt < output.txt
* sort < input.txt > sorted.txt 

Implemented using:
* open()
* O_CREAT, O_TRUNC, O_WRONLY, O_APPEND
* dup2() to remap file descriptors


## 🧠 Concepts Learned / Used

* UNIX process creation (fork)
* Program execution (execvp)
* Inter-process communication (pipes)
* File descriptor redirection (dup2)
* Wait management (waitpid)
* Basic command parsing and tokenization
* Handling built-ins vs external commands
* Error handling with perror()

This project gives practical exposure to Operating Systems, Linux internals, and system-level C++ programming.


## 📁 Project Structure
```text
/myshell
│
├── src
│   ├── main.cpp         # Shell loop, prompt, command dispatch
│   ├── parser.cpp       # Tokenizer and input parsing
│   ├── execute.cpp      # Execution engine, pipes, redirection
│   ├── builtins.cpp     # cd, exit, help implementations
│
├── include              # Header files
├── Makefile             # Build configuration
└── README.md            # Project documentation
```

## 🛠 Build & Run Instructions

* Compile - `make`

* Run - `./myshell`


## 🧩 Future Enhancements (Optional)

* Stage 5: Background job execution (&)

* Stage 6: Command history + arrow keys (readline)

* Auto-completion (tab to list files)

* Better tokenizer supporting quotes and escapes

* Advanced shell scripting features

* Job management (fg, bg, jobs)


## 👤 Author

Shrinivas Uche
* Systems Programming & Fullstack Developer