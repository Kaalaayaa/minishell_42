/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/20 15:34:04 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_redir	*add_redir_node(void)
{
	t_redir	*r;

	r = malloc(sizeof(t_redir));
	if (!r)
		return (NULL);
	r->filename = NULL;
	r->next = NULL;
	return (r);
}

int	is_redirection(char *argv)
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

int	start_redirections(t_tree *tree, t_shell *shell)
{
	t_redir	*redirections;

	if (!tree || !tree->argv)
		return (1);
	redirections = apply_redirections(tree->argv, shell);
	if (!redirections)
		return (0);
	while (redirections)
	{
		if (!redir_allocation(redirections, shell))
		{
			free_redir(redirections);
			return (0);
		}
		redirections = redirections->next;
	}
	free_redir(redirections);
	return (1);
}
