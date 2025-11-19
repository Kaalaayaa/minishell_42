#include "../includes/minishell.h"




t_redir *add_redir_node(void)
{
	t_redir	*r;

	r = malloc(sizeof(t_redir));
	if (!r)
		return (NULL);
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
	if (!argv || !argv[0])
		return (0);	
	
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

char *ft_strtrim_free(char *src, const char *set)
{
	char *dest;

	if (!src)
		return (NULL);
	if (!set)
		return (src);
	dest = ft_strtrim(src, set);
	free(src);	
	return (dest);
}

char	*get_heredoc(char *file, t_shell *shell)
{
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
    	new = malloc(sizeof(char) * 1);
   		new[0] = '\0';
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
			i+= ft_strlen(line) + 1;
        	new = ft_strjoin_free(new, "\n", 1);
        	new = ft_strjoin_free(new, line, 3);
		}
		new = ft_strtrim_free(new, "\n");
		write(fd[1], new, ft_strlen(new));
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
    t_redir *ret = NULL;
    t_redir *head = NULL;
    t_redir *new_node;
    int i = 0;

    while (argv[i])
    {
        if (is_redirection(argv[i]) && argv[i + 1])
        {
            new_node = add_redir_node();
            if (!new_node)
            {
                free_redir(head);
                return (NULL);
            }

            if (!ret)
            {
                head = new_node;
                ret = new_node;
            }
            else
            {
                ret->next = new_node;
                ret = ret->next;
            }

            // If alot_redirection fails, free the node
            alot_redirection(&ret, argv, i, shell);

            i -= 2; // Adjust index after deleting tokens
        }
        i++;
    }

    return (head);
}