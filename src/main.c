/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdangwal <pdangwal@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:19:27 by pdangwal          #+#    #+#             */
/*   Updated: 2025/10/24 19:41:07 by pdangwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"


// this main function is only for testing
int	main(int argc, char *argv[], char *env[])
{
	char *line;
	t_token *tokens;
	t_tree *root;

	if (argc != 1)
		return (0);
	while (1)
	{
		line = readline("minishell% ");
		add_history(line);
		tokens = lexer(line);
		root = parse_e(&tokens);
		print_tree(root, 0);
		free(line);
	}

	return (0);
}
