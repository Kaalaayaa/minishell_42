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

static int	create_and_append_redir(t_redir **ret, t_redir **head,
				char **argv, int index, t_shell *shell)
{
	t_redir *new_node;

	new_node = add_redir_node();
	if (!new_node)
	{
		free_redir(*head);
		return (0);
	}
	if (!*ret)
	{
		*head = new_node;
		*ret = new_node;
	}
	else
	{
		(*ret)->next = new_node;
		*ret = (*ret)->next;
	}
	alot_redirection(ret, argv, index, shell);
	return (1);
}

t_redir *apply_redirections(char **argv, t_shell *shell)
{
	t_redir *ret;
	t_redir *head;
	int i;

	ret = NULL;
	head = NULL;
	i = 0;
	while (argv[i])
	{
		if (is_redirection(argv[i]) && argv[i + 1])
		{
			if (!create_and_append_redir(&ret, &head, argv, i, shell))
				return (NULL);
			i -= 2; // Adjust index after deleting tokens
		}
		i++;
	}
	return (head);
}
