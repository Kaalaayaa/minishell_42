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
/*
void	exec_pipe(t_tree *tree, t_shell *shell)
{
	int		fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe_init_and_left_fork(tree, shell, fd, &left_pid))
		return ;
	if (pipe_right_fork(shell, fd, left_pid, &right_pid))
		return ;
	pipe_cleanup_and_status(shell, fd, left_pid, right_pid);
}
//*/
/*
void	exec_pipe(t_tree *tree, t_shell *shell)
{
	int fd[2];
	pid_t left_pid;
	pid_t right_pid;

	setup_signals_parent();
	shell->in_pipe = true;
	if (pipe(fd) == -1)
		perror("pipe");
	left_pid = fork();
	if (left_pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return ;
	}
	if (left_pid == 0)
	{
		setup_signals_child();
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		exec_tree(tree->left, shell);
		exit(shell->exit_status);
	}
	right_pid = fork();
	if (right_pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		waitpid(left_pid, NULL, 0);
		return ;
	}
	if (right_pid == 0)
	{
		setup_signals_child();
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		exec_tree(tree->right, shell);
		exit(shell->exit_status);
	}
	pipe_cleanup_and_status(shell, fd, left_pid, right_pid);
}*/


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
	print_error(2, "minishell: %s%s%s%s", tree->argv[0], ": ", strerror(errno), "\n");
	exit(127);
}
/*
int	start_redirections(t_tree *tree, t_shell *shell)
{
	t_redir *redir;
	
	redir = tree->redirections;
	while (redir && redir->filename && redir->type)
	{
		if (!redir_allocation(redir, shell))
		{
			shell->exit_status = 1;
			return (0);
		}
		redir = redir->next;
	}
	return (1);
}
*/
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
