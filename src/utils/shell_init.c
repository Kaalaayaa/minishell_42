#include "minishell.h"

void	shell_init(t_shell *shell, char **envp)
{
    if (!shell)
        return ;
    shell->env_list = NULL;
    shell->in_pipe = false;
    if (envp)
        shell->env_list = env_list_init(envp);
    shell->exit_status = 0;
    update_shlvl(shell);
    setup_signals_prompt();  /* Set up signal handlers for the shell prompt */
}
