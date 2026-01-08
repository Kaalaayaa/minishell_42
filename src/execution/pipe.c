/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdangwal <pdangwal@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 16:45:23 by pdangwal          #+#    #+#             */
/*   Updated: 2025/12/19 16:45:28 by pdangwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Handles the left side of the pipe (writes to pipe)
static void	exec_pipe_left_child(t_tree *tree, t_shell *shell, int fd[2])
{
	setup_signals_child();
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	close_fd_in_range(2, 1024);
	exec_tree(tree->left, shell);
	exit(shell->exit_status);
}

// Handles the right side of the pipe (reads from pipe)
static void	exec_pipe_right_child(t_tree *tree, t_shell *shell, int fd[2])
{
	setup_signals_child();
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close_fd_in_range(2, 1024);
	exec_tree(tree->right, shell);
	exit(shell->exit_status);
}

// Forks and executes the left side of the pipe
static pid_t	fork_pipe_left(t_tree *tree, t_shell *shell, int fd[2])
{
	pid_t	left_pid;

	left_pid = fork();
	if (left_pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	if (left_pid == 0)
		exec_pipe_left_child(tree, shell, fd);
	return (left_pid);
}

// Forks and executes the right side of the pipe
static pid_t	fork_pipe_right(t_tree *tree, t_shell *shell,
	int fd[2], pid_t left_pid)
{
	pid_t	right_pid;

	right_pid = fork();
	if (right_pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		waitpid(left_pid, NULL, 0);
		return (-1);
	}
	if (right_pid == 0)
		exec_pipe_right_child(tree, shell, fd);
	return (right_pid);
}

// Main pipe execution function
void	exec_pipe(t_tree *tree, t_shell *shell)
{
	int		fd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	setup_signals_parent();
	shell->in_pipe = true;
	if (pipe(fd) == -1)
	{
		perror("pipe");
		return ;
	}
	left_pid = fork_pipe_left(tree, shell, fd);
	if (left_pid == -1)
		return ;
	right_pid = fork_pipe_right(tree, shell, fd, left_pid);
	if (right_pid == -1)
		return ;
	pipe_cleanup_and_status(shell, fd, left_pid, right_pid);
}
