/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_alloc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdangwal <pdangwal@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 17:10:00 by pdangwal          #+#    #+#             */
/*   Updated: 2025/12/19 17:10:00 by pdangwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	alot_redirection(t_redir **ret, char **argv, int index, t_shell *shell)
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
	if (!argv[index + 1])
		(*ret)->filename = ft_strdup("");
	else if (ft_strcmp(argv[index + 1], "$EMPTY") == 0)
		(*ret)->filename = ft_strdup("$EMPTY");
	else
		(*ret)->filename = ft_strdup(argv[index + 1]);
	delete_line(argv, index);
	delete_line(argv, index);
}

static int	create_and_append_redir(t_redir **ret, t_redir **head,
		char **argv, t_shell *shell)
{
	t_redir	*new_node;
	int		index;

	index = 0;
	while (argv[index] && !is_redirection(argv[index]))
		index++;
	new_node = add_redir_node();
	if (!new_node)
		return (free_redir(*head), 0);
	if (!*ret)
		*head = new_node;
	else
		(*ret)->next = new_node;
	*ret = new_node;
	alot_redirection(ret, argv, index, shell);
	return (1);
}

static int	process_redir_arg(char **argv, int i)
{
	if (argv[i + 1] && ft_strcmp(argv[i + 1], "$EMPTY") != 0 
		&& argv[i + 1][0] != '\0')
		return (1);
	else if (argv[i + 1] && ft_strcmp(argv[i + 1], "$EMPTY") == 0)
		return (1);
	else if (argv[i + 1])
	{
		delete_line(argv, i);
		delete_line(argv, i);
	}
	return (1);
}

t_redir	*apply_redirections(char **argv, t_shell *shell)
{
	t_redir	*ret;
	t_redir	*head;
	int		i;

	ret = NULL;
	head = NULL;
	i = 0;
	while (argv[i])
	{
		if (is_redirection(argv[i]))
		{
			if (!process_redir_arg(argv, i))
				return (NULL);
			if (!create_and_append_redir(&ret, &head, argv, shell))
				return (NULL);
		}
		else
			i++;
	}
	return (head);
}
