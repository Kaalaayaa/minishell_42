# 🐚 Minishell

A minimal Unix shell implementation written in **C**, built from scratch as part of a systems programming project.

This project recreates core behaviors of `bash`, including command parsing, process execution, pipes, redirections, environment variable expansion, and signal handling.

> The objective was to deeply understand how Unix shells work internally — from parsing user input to managing processes and file descriptors.

---

## Features

### Command Execution
- Execute external binaries via `$PATH`
- Absolute and relative path execution
- Proper exit status propagation (`$?`)

### Built-in Commands
- `echo`
- `cd`
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

### Pipes
- Support for multiple pipes (`|`)
- Correct process chaining
- Safe file descriptor handling

### Redirections
- Output redirection: `>`
- Append redirection: `>>`
- Input redirection: `<`
- Heredoc: `<<`

### Environment Variable Expansion
- `$VAR`
- `$?`
- Correct handling of single and double quotes

### Signal Handling
- `Ctrl + C`
- `Ctrl + D`
- `Ctrl + \`
- Interactive behavior similar to bash

### Memory Management
- No memory leaks
- Clean Valgrind report
- Proper resource cleanup on exit

---

## Technical Stack

- **Language:** C  

### System Calls Used
- `fork`
- `execve`
- `pipe`
- `dup2`
- `waitpid`
- `signal`
- `open`
- `close`

### Libraries
- `readline`

### Debugging Tools
- `valgrind`
- `gdb`

---

## Architecture Overview

The shell is structured into modular components:

### 1 - Lexer
Tokenizes user input into:
- Words
- Pipes
- Redirections
- Environment variables
- Quoted strings

### 2 - Parser
Transforms tokens into structured command nodes:
- Command tables
- Argument arrays
- Redirection lists

### 3 - Expander
Handles:
- Environment variable expansion
- Exit code expansion (`$?`)
- Quote interpretation rules

### 4 - Executor
Manages:
- Process creation (`fork`)
- Piping and file descriptor duplication (`dup2`)
- Built-in execution (without forking when required)
- External binary execution (`execve`)
- Exit code handling

---

## Execution Flow

while (1):
    readline()           # read user input
        ↓
    lexer                # tokenize input
        ↓
    parser               # parse tokens into command structures
        ↓
    expander             # expand env variables and handle quotes
        ↓
    executor (fork → pipe → dup2 → execve)  # execute commands
        ↓
    waitpid_all()        # wait for all child processes
    free_all()           # cleanup memory

---

## Installation

Clone the repository and build:

```
git clone https://github.com/YOUR_USERNAME/minishell.git
cd minishell
make
./minishell
```

---

## Example Usage

Run a simple command
```
$ echo Hello Berlin
Hello Berlin
```

Using pipes and redirection
```
$ ls -l | grep ".c" > sources.txt
```

Setting and using environment variables
```
$ export CITY=Berlin
$ echo $CITY
Berlin
```

Input redirection and counting lines
```
$ cat < infile.txt | wc -l >> outfile.txt
```

