#include "minishell.h"

bool is_builtin(const char *cmd)
{
    if(ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
    || ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
    || ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
    || ft_strcmp(cmd, "exit") == 0)
        return (1);
    else
        return 0;
}
int execute_builtin(char **cmd, t_shell *shell)
{
    int exit_status;

    exit_status = 0;
    if(ft_strcmp(cmd[0], "echo") == 0)
        exit_status = builtin_echo(cmd);
    else if(ft_strcmp(cmd[0], "cd") == 0)
        exit_status = builtin_cd(cmd, shell);
    else if(ft_strcmp(cmd[0], "pwd") == 0)
        exit_status = builtin_pwd();
    else if(ft_strcmp(cmd[0], "exit") == 0)
        exit_status = builtin_exit(cmd, shell);
    else if(ft_strcmp(cmd[0], "export") == 0)
        exit_status = builtin_export(cmd, shell);
    else if(ft_strcmp(cmd[0], "unset") == 0)
        exit_status = builtin_unset(cmd, shell);
    else if(ft_strcmp(cmd[0], "env") == 0)
        exit_status = builtin_env(shell);
    return (exit_status);
}
