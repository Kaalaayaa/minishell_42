/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdangwal <pdangwal@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:27:40 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/20 19:27:49 by pdangwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	redir_allocation(t_redir *redirections, t_shell *shell)
{
	if (redirections->type == REDIR_APPEND)
	{
		if (!redir_append(redirections->filename, shell))
			return (0);
	}
	else if (redirections->type == REDIR_OUT)
	{
		if (!redir_output(redirections->filename, shell))
			return (0);
	}
	else if (redirections->type == REDIR_IN)
	{
		if (!redir_input(redirections->filename, shell))
			return (0);
	}
	else if (redirections->type == REDIR_HEREDOC)
		write_lines(redirections->filename);
	return (1);
}

char	**get_envp(t_env *env)
{
	char	**ret;
	t_env	*tmp;
	int		count;
	int		i;

	count = env_count(env);
	ret = malloc(sizeof(char *) * (count + 1));
	if (!ret)
		return (NULL);
	tmp = env;
	i = 0;
	while (tmp)
	{
		ret[i] = env_join(tmp->key, tmp->value);
		if (!ret[i])
		{
			free_split(ret);
			return (NULL);
		}
		i++;
		tmp = tmp->next;
	}
	ret[i] = NULL;
	return (ret);
}

int	run_parent_builtin(t_tree *tree, t_shell *shell)
{
	if (!is_builtin(tree->argv[0]))
		return (0);
	if (shell->in_pipe)
		return (0);
	shell->exit_status = execute_builtin(tree->argv, shell);
	return (1);
}

void	child_exec(t_tree *tree, t_shell *shell, char **envp, char *path)
{
	setup_signals_child();
	if (is_builtin(tree->argv[0]))
	{
		execute_builtin(tree->argv, shell);
		free_exec_resources(envp, path);
		exit(shell->exit_status);
	}
	execute_foreign(envp, path, tree);
	free_exec_resources(envp, path);
	exit(127);
}

void	pipe_end(int *fd, int side, t_tree *tree, t_shell *shell)
{
	if (side == 0)
	{
		setup_signals_child();
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		exec_tree(tree->left, shell);
		exit(shell->exit_status);
	}
	else if (side == 1)
	{
		setup_signals_child();
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		exec_tree(tree->right, shell);
		exit(shell->exit_status);
	}
}
