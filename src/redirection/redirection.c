#include "../includes/minishell.h"




t_redir *add_redir_node(void)
{
	t_redir	*r;

	r = malloc(sizeof(t_redir));
	r->filename = NULL;
	r->next = NULL;
	return (r);
}

void    delete_line(char **argv, int index)
{
    int i;

    if (!argv)
        return ;
    free(argv[index]);
    i = index;
    while(argv[i])
    {
        argv[i] = argv[i + 1];
        i++;
    }
}

int is_redirection(char *argv)
{
    if (ft_strcmp(argv, ">") == 0)
        return (1);
    else if (ft_strcmp(argv, ">>") == 0)
        return (1);
    else if (ft_strcmp(argv, "<") == 0)
        return (1);
    else if (ft_strcmp(argv, "<<") == 0)
        return (1);
    return (0);
}

char	*get_heredoc(char *file, t_shell *shell)
{
	(void)shell;
	char	*line;
	char	*new;
	int		i;
    pid_t   pid;
    int    status;
    int     fd[2];

	if (pipe(fd) == -1)
		return (NULL);
	pid = fork();
	if (pid == -1)
		return (NULL);
	if (pid == 0)
	{
		setup_signals_heredoc();
		close(fd[0]);


		i = 0;
    	new = malloc(sizeof(char) + 1);
   		new[1] = '\0';
		while(1)
		{
			line = readline("> ");
			if (!line)
				break ;
			if (ft_strcmp(line, file) == 0)
			{
				free(line);
				break;
			}
        	new = ft_strjoin(new, ft_strdup("\n"));
        	new = ft_strjoin(new, ft_strdup(line));
        
			i+= ft_strlen(line) + 1;
        	free(line);
		}
    	new = ft_strtrim(new, "\n");
		write(fd[1], new, ft_strlen(new));
		free(new);
		close(fd[1]);
		exit(0);
	}
	else
	{
		close(fd[1]);
		setup_signals_parent();
		waitpid(pid, &status, 0);
		setup_signals_prompt();

		if (WIFEXITED(status))
		{
			shell->exit_status = WEXITSTATUS(status);
			//printf("DEBUG: setting exit_status from heredoc44: %d\n", shell->exit_status);
			if (shell->exit_status == 130)
				g_signal_status = 1;
		} else if (WIFSIGNALED(status)) 
		{
			//printf("DEBUG: setting exit_status from heredoc44: %d\n", shell->exit_status);
			shell->exit_status = 128 + WTERMSIG(status);
		}
	
		char buffer[1024];
		int bytes = read(fd[0], buffer, sizeof(buffer) - 1);
		buffer[bytes] = '\0';
		close(fd[0]);
	
		return (ft_strdup(buffer));
	}
}

void    alot_redirection(t_redir **ret, char **argv, int index, t_shell *shell)
{

    if (ft_strcmp(argv[index], ">") == 0)
        (*ret)->type = REDIR_OUT;
    else if (ft_strcmp(argv[index], ">>") == 0)
        (*ret)->type = REDIR_APPEND;
    else if (ft_strcmp(argv[index], "<") == 0)
        (*ret)->type = REDIR_IN;
    else if (ft_strcmp(argv[index], "<<") == 0)
    {
        (*ret)->type = REDIR_HEREDOC;
        (*ret)->filename = get_heredoc(argv[index + 1], shell);
        delete_line(argv, index);
        delete_line(argv, index);
        return ;
    }
    (*ret)->filename = ft_strdup(argv[index + 1]);
    delete_line(argv, index);
    delete_line(argv, index);
}

t_redir *apply_redirections(char **argv, t_shell *shell)
{
    t_redir *ret;
    t_redir *head;
    int i;

    if (!argv)
        return NULL;
    ret = add_redir_node();
    head = ret;
    i = 0;
    while(argv[i])
    {
        if (is_redirection(argv[i]) && argv[i + 1])
        {
            alot_redirection(&ret, argv, i, shell);
            ret->next = add_redir_node();
            ret = ret->next;
            i -= 2;
        }
        i++;
    }
    return (head);
}