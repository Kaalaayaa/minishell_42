#!/bin/bash

echo "Test 1: ls \$VAR (undefined variable)"
./minishell <<< "ls \$VAR" 2>&1 | head -5

echo -e "\nTest 2: ls > \$VAR (redirection with undefined)"
./minishell <<< "ls > \$VAR" 2>&1

echo -e "\nTest 3: cat << d with 'd' delimiter"
./minishell <<< "cat << d
d" 2>&1

echo -e "\nTest 4: cat << d with 'e' input and EOF"
./minishell <<< "cat << d
e" 2>&1

