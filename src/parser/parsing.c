/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:21 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/12 16:08:07 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// the kleene star (*) means 0 or more repeitions

/* <<grammer tree for minishell>>
 * <command_line> 	-> <command> ('|' <command>)*
 * <command> 		-> <simple_command> (<redirection_list>)*
 * <simple_command> 	-> <WORD> (<ARG>)*;
 * <redirection_list>	-> <redirection> | <redirection> <redirection_list>
 * <redirection> 	-> ('>' | '<' | '>>' | '<<') <WORD>
 */

t_tree	*add_tree_node(char *dst, t_type s)
{
	t_tree	*tree;

	tree = malloc(sizeof(t_tree));
	tree->token = ft_strdup(dst);
	tree->type = s;
	tree->left = NULL;
	tree->right = NULL;
	tree->argv = NULL;
	tree->redirections = NULL;
	return (tree);
}

char	**parse_simple_command(t_token **tokens)
{
	t_token	*tmp;
	char	**argv;
	int		i;

	i = 0;
	tmp = *tokens;
	while (tmp && tmp->type == WORD)
	{
		if (ft_strcmp(tmp->token, "$EMPTY") == 0)
		{
			tmp = tmp->next;
			if (tmp == NULL)
				break ;
		}
		tmp = tmp->next;
		i++;
	}
	argv = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (*tokens && (*tokens)->type == WORD)
	{
		if (ft_strcmp((*tokens)->token, "$EMPTY") == 0)
		{
			*tokens = (*tokens)->next;
			if (*tokens == NULL)
				break ;
		}
		argv[i] = ft_strdup((*tokens)->token);
		i++;
		if (*tokens != NULL)
			*tokens = (*tokens)->next;
	}
	argv[i] = NULL;
	return (argv);
}

t_tree	*init_tree(t_type s)
{
	t_tree	*ret;

	ret = malloc(sizeof(t_tree));
	if (!ret)
		return (NULL);
	ret->redirections = NULL;
	ret->type = s;
	ret->argv = NULL;
	ret->token = NULL;
	ret->left = NULL;
	ret->right = NULL;
	return (ret);
}

t_tree	*parse_e(t_token **head, t_shell *shell)
{
	t_tree	*left;
	t_tree	*right;
	t_tree	*pipe_node;
	t_token	*tokens;

	tokens = *head;
	if (!tokens || !tokens->token)
		return (NULL);
	left = init_tree(WORD);
	left->argv = parse_simple_command(&tokens);
	left->redirections = apply_redirections(left->argv, shell);
	while (tokens && (tokens)->type == PIPE)
	{
		tokens = (tokens)->next;
		right = init_tree(WORD);
		right->argv = parse_simple_command(&tokens);
		right->redirections = apply_redirections(right->argv, shell);
		pipe_node = add_tree_node("|", PIPE);
		pipe_node->right = right;
		pipe_node->left = left;
		left = pipe_node;
	}
	return (left);
}

/*
void	print_tree(t_tree *node, int depth)
{
	if (!node)
		return ;
	for (int i = 0; i < depth; i++)
		printf("    ");
	if (node->type == PIPE)
		printf("[PIPE]\n");
	else if (node->argv)
	{
		printf("[CMD]");
		for (int i = 0; node->argv[i]; i++)
			printf(" %s", node->argv[i]);
		printf("\n");
	}
	else if (node->token)
		printf("[TOKEN] %s\n", node->token);
	else
		printf("[UNKNOWN]\n");
	if (node->left)
		print_tree(node->left, depth + 1);
	if (node->right)
		print_tree(node->right, depth + 1);
}*/
