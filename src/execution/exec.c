#include "../includes/minishell.h"

void verify_path(char *path, t_tree *tree);

void	pipe_end(int *fd, int side, t_tree *tree, t_shell *shell)
{
	if (side == 0) //left
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

void	exec_pipe(t_tree *tree, t_shell *shell)
{
	int	fd[2];
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

int	env_count(t_env *env)
{
	int		count;
	t_env	*tmp;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

char	*env_join(char *key, char *value)
{
	char	*str;
	int		len_k;
	int		len_v;

	len_k = ft_strlen(key);
	len_v = ft_strlen(value);
	str = malloc(len_k + len_v + 2);
	if (!str)
		return (NULL);
	ft_memcpy(str, key, len_k);
	str[len_k] = '=';
	ft_memcpy(str + len_k + 1, value, len_v);
	str[len_k + len_v + 1] = '\0';
	return (str);
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
			free_split(ret, --i);
			return (NULL);
		}
		i++;
		tmp = tmp->next;
	}
	ret[i] = NULL;
	return (ret);
}


void	print_and_exit(char *s1, char *s2, char *s3, int exitcode)
{
	print_error(s1, s2, s3);
	exit(exitcode);
}


void execute_foreign(char **envp, char *path, t_tree *tree)
{
    char *execpath;

    verify_path(path, tree);

    execpath = path;
    if (ft_strchr(tree->argv[0], '/'))
        execpath = tree->argv[0];

    execve(execpath, tree->argv, envp);

    if (errno == EACCES)
        print_and_exit("minishell: ", tree->argv[0], ": Permission denied\n", 126);
    else if (errno == ENOENT)
        print_and_exit("minishell: ", tree->argv[0], ": No such file or directory\n", 127);

    print_error("minishell: ", tree->argv[0], ": ");
    print_error(strerror(errno), "\n", NULL);
    exit(127);
}

static void	check_slash_path(t_tree *tree)
{
	struct stat	st;

	if (stat(tree->argv[0], &st) != 0)
		print_and_exit("minishell: ", tree->argv[0],
			": No such file or directory\n", 127);
	if (S_ISDIR(st.st_mode))
		print_and_exit("minishell: ", tree->argv[0],
			": Is a directory\n", 126);
}

static void	check_normal_path(char *path, t_tree *tree)
{
	struct stat	st;

	if (!path)
		print_and_exit("minishell: ", tree->argv[0],
			": command not found\n", 127);
	if (stat(path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
			print_and_exit("minishell: ", tree->argv[0],
				": Is a directory\n", 126);
	}
}

void	verify_path(char *path, t_tree *tree)
{
	if (ft_strchr(tree->argv[0], '/'))
	{
		check_slash_path(tree);
		return ;
	}
	check_normal_path(path, tree);
}
void	free_exec_resources(char **envp, char *path)
{
	if (envp)
		free_split(envp, 0);
	if (path)
		free(path);
}


static int	run_parent_builtin(t_tree *tree, t_shell *shell)
{
	if (!is_builtin(tree->argv[0]))
		return (0);
	if (shell->in_pipe)
		return (0);
	execute_builtin(tree->argv, shell);
	return (1);
}

static void	child_exec(t_tree *tree, t_shell *shell,
			char **envp, char *path)
{
	if (is_builtin(tree->argv[0]))
	{
		execute_builtin(tree->argv, shell);
		exit(shell->exit_status);
	}
	execute_foreign(envp, path, tree);
	exit(127);
}

static void	update_exit_status(int status, t_shell *shell)
{
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
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
	free_split(envp, 0);
	free(path);
	update_exit_status(status, shell);
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

void exec_with_redir(t_tree *tree, t_shell *shell)
{
	int		outfd;
	int		infd;
	int		res;
	t_redir *redir;

	redir = tree->redirections;
	res = 1;
	outfd = dup(STDOUT_FILENO);
	infd = dup(STDIN_FILENO);
	while(redir && redir->filename && redir->type)
	{
			if (!redir_allocation(redir, shell))
			{
				res = 0;
				shell->exit_status = 1;
				break;
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
