/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/20 15:34:04 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->token);
		free(tmp);
	}
}

void	free_redir(t_redir *redir)
{
	t_redir	*tmp;

	if (!redir)
		return ;
	while (redir)
	{
		if (redir->filename)
			free(redir->filename);
		tmp = redir;
		redir = redir->next;
		free(tmp);
		tmp = NULL;
	}
}

void	free_tree(t_tree *tree)
{
	if (!tree)
		return ;
	free_tree(tree->left);
	tree->left = NULL;
	free_tree(tree->right);
	tree->right = NULL;
	if (tree->argv)
		free_split(tree->argv, 0);
	if (tree->redirections)
		free_redir(tree->redirections);
	if (tree->token)
		free(tree->token);
	free(tree);
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->env_list)
		free_env(shell->env_list);
}

void	cleanup(t_token *tokens, t_tree *tree, t_shell *shell)
{
	if (tokens)
		free_tokens(tokens);
	if (tree)
		free_tree(tree);
	if (shell)
		free_shell(shell);
}
