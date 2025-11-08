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

char	*get_heredoc(char *file)
{
	char	*line;
	char	*new;
	int		i;
	
	i = 0;
    new = malloc(sizeof(char) + 1);
    new[1] = '\0';
	while(1)
	{
		line = readline("> ");
		if (ft_strcmp(line, file) == 0)
		{
            ft_strtrim(new, "\n");
			free(line);
			break;
		}
    
        new = ft_strjoin(new, ft_strdup("\n"));
        new = ft_strjoin(new, ft_strdup(line));
		i+= ft_strlen(line) + 1;
        free(line);
		
	}
    //printf("my new: %s\n", new);
    return (new);
}

void    alot_redirection(t_redir **ret, char **argv, int index)
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
        (*ret)->filename = get_heredoc(argv[index + 1]);
        delete_line(argv, index);
        delete_line(argv, index);
        return ;
    }
    (*ret)->filename = ft_strdup(argv[index + 1]);
    delete_line(argv, index);
    delete_line(argv, index);
}

t_redir *apply_redirections(char **argv)
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
            alot_redirection(&ret, argv, i);
            ret->next = add_redir_node();
            ret = ret->next;
            i -= 2;
        }
        i++;
    }
    return (head);
}