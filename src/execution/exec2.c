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

static int	check_redir_empty(t_redir *redirections, t_shell *shell)
{
	if (ft_strcmp(redirections->filename, "$EMPTY") == 0 
		|| (redirections->type != REDIR_HEREDOC 
			&& (!redirections->filename || !redirections->filename[0])))
	{
		print_error(2, "minishell: %s\n", "ambiguous redirect");
		shell->exit_status = 1;
		return (0);
	}
	return (1);
}

static int	apply_redir_type(t_redir *redirections, t_shell *shell)
{
	if (redirections->type == REDIR_APPEND)
		return (redir_append(redirections->filename, shell));
	else if (redirections->type == REDIR_OUT)
		return (redir_output(redirections->filename, shell));
	else if (redirections->type == REDIR_IN)
		return (redir_input(redirections->filename, shell));
	else if (redirections->type == REDIR_HEREDOC)
		return (write_lines(redirections->filename));
	return (1);
}

int	redir_allocation(t_redir *redirections, t_shell *shell)
{
	if (!check_redir_empty(redirections, shell))
		return (0);
	if (!apply_redir_type(redirections, shell))
		return (0);
	return (1);
}

int	run_parent_builtin(t_tree *tree, t_shell *shell)
{
	if (!is_builtin(tree->argv[0]))
		return (0);
	if (shell->in_pipe)
		return (0);
	filter_empty_args(tree->argv);
	shell->exit_status = execute_builtin(tree->argv, shell);
	return (1);
}

void	child_exec(t_tree *tree, t_shell *shell, char **envp, char *path)
{
	setup_signals_child();
	filter_empty_args(tree->argv);
	if (is_builtin(tree->argv[0]))
	{
		execute_builtin(tree->argv, shell);
		free_exec_resources(envp, path);
		exit(shell->exit_status);
	}
	execute_foreign(envp, path, tree);
	close_fd_in_range(2, 1024);
	free_exec_resources(envp, path);
	exit(127);
}
