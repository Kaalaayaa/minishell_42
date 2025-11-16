#include "../includes/minishell.h"


void	exec_pipe(t_tree *tree, t_shell *shell)
{
	int	fd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		status;

	if (!tree || !shell)
		return ;
	if (pipe(fd) == -1)
		return ;

	left_pid = fork();
	if (left_pid == 0)
	{
		setup_signals_child();
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		exec_tree(tree->left, shell);
		exit(shell->exit_status);
	}
	right_pid = fork();
	if (right_pid == 0)
	{
		setup_signals_child();
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		exec_tree(tree->right, shell);
		exit(shell->exit_status);
	}
	close(fd[0]);
	close(fd[1]);
	setup_signals_parent();
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status)){
		shell->exit_status = WEXITSTATUS(status);	
	}

	else if (WIFSIGNALED(status))
	{
		shell->exit_status = 128 + WTERMSIG(status);
	}
	setup_signals_prompt();
}


void	exec_cmd(t_tree *tree, t_shell *shell)
{
	pid_t	pid;
	char	*path;
	int		status;

	if (!tree)
		return ;
	if (is_builtin(tree->argv[0]))
		execute_builtin(tree->argv, shell);
	else
	{
		pid = fork();
		if (pid == 0) // child process
		{
			setup_signals_child();
			path = get_path(tree->argv[0], shell);
			if (!path)
			{
				perror("command not foudn");
				exit(127);
			}
			execve(path, tree->argv, NULL); // the null here needs to be char**envp
		}
		else if (pid > 0)
		{
			setup_signals_parent();
			waitpid(pid, &status, 0);
			setup_signals_prompt();
			if (WIFEXITED(status))
			{// killed normally 
				shell->exit_status = WEXITSTATUS(status);
				//printf("DEBUG: setting exit_status from exec22: %d\n", shell->exit_status);
			}
			else if (WIFSIGNALED(status)) // killed by signal
			{
				int	sig = WTERMSIG(status);
				//printf("DEBUG: setting exit_status from heredoc33: %d\n", 130);// get signal number
				shell->exit_status = 128 + sig;
				if (sig == SIGQUIT)
					printf("Quit (core dumped)\n");
				else if (sig == SIGINT)
					printf("\n");
			}
		}
	}
}


void	write_lines(char *argv)
{
	int	fd[2];

	pipe(fd);
	if (fork() == 0)
	{
		close(fd[0]);
			write(fd[1], argv, ft_strlen(argv));
			write(fd[1], "\n", 1);
		close(fd[1]);
		exit(0);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
}


void exec_with_redir(t_tree *tree, t_shell *shell)
{
	int		outfd;
	int		infd;

	outfd = dup(STDOUT_FILENO);
	infd = dup(STDIN_FILENO);
	while(tree->redirections && tree->redirections->filename && tree->redirections->type)
	{
				if (tree->redirections->type == REDIR_APPEND)
					redir_append(tree->redirections->filename);
				else if (tree->redirections->type == REDIR_OUT)
					redir_output(tree->redirections->filename);
				else if (tree->redirections->type == REDIR_IN)
					redir_input(tree->redirections->filename);
				else if (tree->redirections->type == REDIR_HEREDOC)
					write_lines(tree->redirections->filename);
			tree->redirections = tree->redirections->next;
	}
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
