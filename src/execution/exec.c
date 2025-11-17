#include "../includes/minishell.h"


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
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);	
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	shell->in_pipe = false;
}


char **get_envp(t_env *env) // change this
{
    int count = 0;
    t_env *tmp = env;
    char **ret;

    // Count entries
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }

    ret = malloc(sizeof(char *) * (count + 1));
    if (!ret)
        return NULL;

    tmp = env;
    count = 0;

    while (tmp)
    {
        size_t len_key = strlen(tmp->key);
        size_t len_val = strlen(tmp->value);

        ret[count] = malloc(len_key + len_val + 2);  // + '=' + '\0'
        if (!ret[count])
            return NULL;

        memcpy(ret[count], tmp->key, len_key);
        ret[count][len_key] = '=';
        memcpy(ret[count] + len_key + 1, tmp->value, len_val);
        ret[count][len_key + len_val + 1] = '\0';

        tmp = tmp->next;
        count++;
    }

    ret[count] = NULL;
    return ret;
}

void exec_cmd(t_tree *tree, t_shell *shell)
{
    pid_t   pid;
    char    *path;
    int     status;
    char    **envp;

    if (!tree || !tree->argv || !tree->argv[0])
        return;
    if (tree->argv[0][0] == '\0')  // e.g. "", $EMPTY, "$EMPTY"
    {
        shell->exit_status = 0;
        return;
    }
    if (is_builtin(tree->argv[0]))
    {
        if (shell->in_pipe)
        {
            pid = fork();
            if (pid == 0)
            {
                setup_signals_child();
                execute_builtin(tree->argv, shell);
                exit(shell->exit_status);
            }
            waitpid(pid, &status, 0);
            shell->exit_status = WEXITSTATUS(status);
        }
        else
            execute_builtin(tree->argv, shell);
        return;
    }
    envp = get_envp(shell->env_list);
    pid = fork();

    if (pid == 0)
    {
        setup_signals_child();

        path = get_path(tree->argv[0], shell);
        if (!path)
        {
            if (ft_strchr(tree->argv[0], '/'))
				path = tree->argv[0];
            else if (!path)
            {
                print_error("minishell: ", tree->argv[0], ": command not found\n");
                exit(127);
            }
        }
        struct stat st;
        if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
        {
            print_error("minishell: ", tree->argv[0], ": Is a directory\n");
            exit(126);
        }
        execve(path, tree->argv, envp);
		if (errno == EACCES)
		{
    print_error("minishell: ", tree->argv[0], ": Permission denied\n");
    exit(126);
		}
	else if (errno == ENOENT)
		{
		print_error("minishell: ", tree->argv[0], ": No such file or directory\n");
	 	exit(127);
	}
	else
		{
		    print_error("minishell: ", tree->argv[0], ": ");
 		   print_error(strerror(errno), "\n", NULL);
 		   exit(127);
	}

        exit(0); // should never happen
    }
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        shell->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        shell->exit_status = 128 + WTERMSIG(status);
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

int	redir_allocation(t_tree *tree, t_shell *shell)
{
	if (tree->redirections->type == REDIR_APPEND)
	{
		if (!redir_append(tree->redirections->filename, shell))
			return (0);
	}	
	else if (tree->redirections->type == REDIR_OUT)
	{
		if (!redir_output(tree->redirections->filename, shell))
			return (0);
	}
	else if (tree->redirections->type == REDIR_IN)
	{
		if (!redir_input(tree->redirections->filename, shell))
			return (0);
	}
	else if (tree->redirections->type == REDIR_HEREDOC)
		write_lines(tree->redirections->filename);
	return (1);
}

void exec_with_redir(t_tree *tree, t_shell *shell)
{
	int		outfd;
	int		infd;
	int		res;

	res = 1;
	outfd = dup(STDOUT_FILENO);
	infd = dup(STDIN_FILENO);
	while(tree->redirections && tree->redirections->filename && tree->redirections->type)
	{
			if (!redir_allocation(tree, shell))
			{
				res = 0;
				shell->exit_status = 1;
				break;
			}
			tree->redirections = tree->redirections->next;
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
