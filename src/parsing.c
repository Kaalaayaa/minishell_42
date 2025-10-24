/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdangwal <pdangwal@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:21 by pdangwal          #+#    #+#             */
/*   Updated: 2025/10/24 19:43:10 by pdangwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

// the kleene star (*) means 0 or more repeitions

/* <<grammer tree for minishell>>
 * <command_line> 	-> <command> ('|' <command>)*
 * <command> 		-> <simple_command> (<redirection_list>)*
 * <simple_command> 	-> <WORD> (<ARG>)*;
 * <redirection_list>	-> <redirection> | <redirection> <redirection_list>
 * <redirection> 	-> ('>' | '<' | '>>' | '<<') <WORD>
 */

t_token	*scan_token(t_token *token)
{
	t_token	*next;

	if (!token || !token->next)
		return (NULL);
	next = token->next;
	return (next);
}

t_tree	*add_tree_node(char *dst, t_type s)
{
	t_tree	*tree;

	tree = malloc(sizeof(t_tree));
	tree->token = ft_strdup(dst);
	tree->type = s;
	tree->left = NULL;
	tree->right = NULL;
	return (tree);
}

char	**parse_simple_command(t_token *tokens)
{
	t_token	*tmp;
	char	**argv;
	int		i;

	i = 0;
	tmp = tokens;
	while (tmp && tmp->type == WORD)
	{
		tmp = tmp->next;
		i++;
	}
	argv = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (tokens && tokens->type == WORD)
	{
		argv[i] = ft_strdup(tokens->token);
		i++;
		tokens = tokens->next;
	}
	argv[i] = NULL;
	return (argv);
}

t_tree	*init_tree(t_type s)
{
	t_tree	*ret;

	ret = malloc(sizeof(t_tree));
	ret->type = s;
	ret->argv = NULL;
	ret->token = NULL;
	ret->left = NULL;
	ret->right = NULL;
	return (ret);
}

t_tree	*parse_e(t_token **tokens)
{
	t_tree	*left;
	t_tree	*right;
	t_tree	*pipe_node;

	left = init_tree(WORD);
	left->argv = parse_simple_command(*tokens);
	while (*tokens && (*tokens)->type == WORD)
		*tokens = (*tokens)->next;
	while (*tokens && (*tokens)->type == PIPE)
	{
		*tokens = (*tokens)->next;
		right = init_tree(WORD);
		right->argv = parse_simple_command(*tokens);
		pipe_node = add_tree_node("|", PIPE);
		pipe_node->right = right;
		pipe_node->left = left;
		left = pipe_node;
		while (*tokens && (*tokens)->type == WORD)
			*tokens = (*tokens)->next;
	}
	return (left);
}

void	print_tree(t_tree *node, int depth)
{
	if (!node)
		return ;
	// print indentation
	for (int i = 0; i < depth; i++)
		printf("    ");
	// print this node
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
	// recursively print children
	if (node->left)
		print_tree(node->left, depth + 1);
	if (node->right)
		print_tree(node->right, depth + 1);
}
