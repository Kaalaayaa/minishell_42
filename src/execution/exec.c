/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/12 15:34:04 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exec_pipe(t_tree *tree, t_shell *shell)
{
	int		fd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		status;

	if (!tree || !shell)
		return ;
	shell->in_pipe = true;
	if (pipe(fd) == -1)
		return ;
	left_pid = fork();
	if (left_pid == 0)
		pipe_end(fd, 0, tree, shell);
	right_pid = fork();
	if (right_pid == 0)
		pipe_end(fd, 1, tree, shell);
	close(fd[0]);
	close(fd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	shell->in_pipe = false;
}

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
		print_and_exit("minishell: ", tree->argv[0], ": Permission denied\n",
			126);
	else if (errno == ENOENT)
		print_and_exit("minishell: ", tree->argv[0],
			": No such file or directory\n", 127);
	print_error("minishell: ", tree->argv[0], ": ");
	print_error(strerror(errno), "\n", NULL);
	exit(127);
}

void	exec_cmd(t_tree *tree, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	**envp;
	char	*path;

	if (!tree || !tree->argv || !tree->argv[0])
		return ;
	if (run_parent_builtin(tree, shell))
		return ;
	envp = get_envp(shell->env_list);
	path = get_path(tree->argv[0], shell);
	pid = fork();
	if (pid == 0)
		child_exec(tree, shell, envp, path);
	waitpid(pid, &status, 0);
	free_exec_resources(envp, path);
	update_exit_status(status, shell);
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
