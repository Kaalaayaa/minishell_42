/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/12/05 20:03:49 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_foreign(char **envp, char *path, t_tree *tree)
{
	char	*execpath;

	verify_path(envp, path, tree);
	execpath = path;
	if (ft_strchr(tree->argv[0], '/'))
		execpath = tree->argv[0];
	execve(execpath, tree->argv, envp);
	free_exec_resources(envp, path);
	if (errno == EACCES)
	{
		print_and_exit("minishell: ", tree->argv[0],
			": Permission denied\n", 126);
	}
	else if (errno == ENOENT)
	{
		print_and_exit("minishell: ", tree->argv[0],
			": No such file or directory\n", 127);
	}
	print_error(2, "minishell: %s%s%s%s", tree->argv[0],
		": ", strerror(errno), "\n");
	exit(127);
}

void	exec_cmd(t_tree *tree, t_shell *shell)
{
	char	**envp;
	char	*path;

	if (exec_cmd_prechecks(tree, shell))
		return ;
	if (exec_cmd_setup(tree, shell, &envp, &path))
		return ;
	exec_cmd_fork_exec(tree, shell, envp, path);
}

void	exec_with_redir(t_tree *tree, t_shell *shell)
{
	int		outfd;
	int		infd;
	int		res;
	t_redir	*redir;

	redir = tree->redirections;
	res = 1;
	outfd = dup(STDOUT_FILENO);
	infd = dup(STDIN_FILENO);
	while (redir && redir->filename && redir->type)
	{
		if (!redir_allocation(redir, shell))
		{
			res = 0;
			shell->exit_status = 1;
			break ;
		}
		redir = redir->next;
	}
	if (res)
		exec_cmd(tree, shell);
	dup2(outfd, STDOUT_FILENO);
	dup2(infd, STDIN_FILENO);
	close(outfd);
	close(infd);
}

void	exec_tree(t_tree *tree, t_shell *shell)
{
	if (!tree)
		return ;
	if (tree->type == PIPE)
	{
		exec_pipe(tree, shell);
		return ;
	}
	else if (tree->type == WORD)
	{
		exec_with_redir(tree, shell);
		return ;
	}
}
