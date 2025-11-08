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
		exit(0);
	}
	right_pid = fork();
	if (right_pid == 0)
	{
		setup_signals_child();
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		exec_tree(tree->right, shell);
		exit(0);
	}
	close(fd[0]);
	close(fd[1]);
	setup_signals_parent();
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);	
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	setup_signals_prompt();
}

char	*get_path_env(t_shell *shell)
{
	t_env *env;

	env = shell->env_list;
	while(env->key)
	{
		if (ft_strncmp(env->key, "PATH", 4) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*full_path(char *argv)
{
	if (!argv)
		return (NULL);
	if (access(argv, X_OK) == 0)
	{
		return (ft_strdup(argv));
	}
	return (NULL);
}

char	**check_path(char *argv, t_shell *shell)
{
	char *path_env;
	char **paths;

	if (!argv)
		return (NULL);
	
	path_env = get_path_env(shell);
	
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');

	return (paths);
}

char	*get_path(char *argv, t_shell *shell)
{
	char	*tmp;
	char	**paths;
	char 	*fullpath;
	int		i;

	if (ft_strchr(argv, '/'))
		return (full_path(argv));
	
	paths = check_path(argv, shell);
	
	if (!paths)
		return (NULL);
	i = 0;
	while(paths[i])
	{
		ft_trim_end(paths[i], ':');
		tmp = ft_strjoin(paths[i], "/");
		
		fullpath = ft_strjoin(tmp, argv);
		free(tmp);
		if (access(fullpath, X_OK) == 0)
		{
			//might have to free the split here;
			return (fullpath);
		}
		free(fullpath);
		i++;
	}
	//ft_freesplit() here too;
	return (NULL);
	
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
			if (WIFEXITED(status)) // killed normally 
				shell->exit_status = WEXITSTATUS(status); 
			else if (WIFSIGNALED(status)) // killed by signal
			{
				int	sig = WTERMSIG(status); // get signal number
				shell->exit_status = 128 + sig;
				if (sig == SIGQUIT)
					printf("Quit (core dumped)\n");
				else if (sig == SIGINT)
					printf("\n");
			}
		}
	}
}

void	redir_append(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		perror("fd");
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	redir_output(char *file)
{
	int fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		perror("fd");
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	redir_input(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		perror("fd");
	dup2(fd, STDIN_FILENO);
	close(fd);
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

void	free_lines(char **argv)
{
	int	i;

	i = 0;
	while(argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

char *safe_readline(const char *prompt)
{
    char *input = NULL;
    int saved_stdout;

    // Save the current STDOUT file descriptor
    saved_stdout = dup(STDOUT_FILENO);

    // If stdin is a terminal (interactive), show the prompt before reading
    if (isatty(STDIN_FILENO))
    {
        // Restore the original STDOUT (just in case it's been redirected)
        dup2(saved_stdout, STDOUT_FILENO);  // Reset STDOUT to terminal
        close(saved_stdout);  // Close the saved descriptor

        write(STDOUT_FILENO, prompt, strlen(prompt));  // Manually write the prompt
        fflush(stdout);  // Force flush to ensure the prompt is shown immediately
    }
    else
    {
        close(saved_stdout);  // Close saved descriptor if not using terminal
    }

    // Use readline to get the input (it's available in many Unix-like systems)
    input = readline("");  // Read a line from stdin (empty prompt, we already displayed it)

    // If readline failed or returned EOF, handle it
    if (!input)
    {
        return NULL;
    }

    // If the input has a newline character at the end, remove it
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n')
    {
        input[len - 1] = '\0';  // Remove the newline character
    }

    return input;
}


void	redir_heredoc(char *file)
{
	char	*line;
	char	**new;
	int		i;
	
	i = 0;
	new = malloc(sizeof(char *));
	while(1)
	{
		line = safe_readline("> ");
		if (ft_strcmp(line, file) == 0)
		{
			free(line);
			break;
		}
		new = realloc(new, sizeof(char *) * (i + 2));
		new[i] = ft_strdup(line);
		free(line);
		i++;
	}
	new[i] = NULL;
	//write_lines(new);
	free_lines(new);
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
				//	redir_heredoc(tree->redirections->filename);
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
