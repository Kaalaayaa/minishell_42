/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 20:00:00 by kchatela          #+#    #+#             */
/*   Updated: 2025/12/05 20:10:51 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
/*
int	pipe_init_and_left_fork(t_tree *tree, t_shell *shell,
	int fd[2], pid_t *left_pid)
{
	if (!tree || !shell || pipe(fd) == -1)
		return (1);
	setup_signals_parent();
	shell->in_pipe = true;
	*left_pid = fork();
	if (*left_pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (1);
	}
	if (*left_pid == 0)
		pipe_end(fd, 0, tree, shell);
	return (0);
}

int	pipe_right_fork(t_shell *shell, int fd[2],
	pid_t left_pid, pid_t *right_pid)
{
	*right_pid = fork();
	if (*right_pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		waitpid(left_pid, NULL, 0);
		return (1);
	}
	if (*right_pid == 0)
		pipe_end(fd, 1, shell->tree, shell);
	return (0);
}
*/

void	pipe_cleanup_and_status(t_shell *shell,
	int fd[2], pid_t left_pid, pid_t right_pid)
{
	int	status;

	close(fd[0]);
	close(fd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	shell->in_pipe = false;
	setup_signals_prompt();
	handle_pipe_status(status, shell);
}

void	print_and_exit(char *s1, char *s2, char *s3, int exitcode)
{
	print_error(2, "%s%s%s",s1, s2, s3);
	exit(exitcode);
}
