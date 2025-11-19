/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:19:27 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/19 19:02:39 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*line;
	t_token	*tokens;
	t_tree	*root;

	(void)argc;
	(void)argv;
	(void)envp;
	shell_init(&shell, envp);
	while (1)
	{
		line = readline("minishell% ");
		if (!line)
		{
			cleanup(NULL, NULL, &shell);
			printf("exit\n");
			break ;
		}
		if (line[0] != '\0')
			add_history(line);
		if (g_signal_status == 1)
		{
			shell.exit_status = 130;
			g_signal_status = 0;
		}
		tokens = lexer(line);
		tokens = expander(tokens, &shell);
		tokens = syntax(tokens, &shell);
		root = parse_e(&tokens, &shell);
		if (g_signal_status == 1)
		{
			shell.exit_status = 130;
			g_signal_status = 0;
			// free(line);
			continue ;
		}
		// print_tree(root, 0);
		exec_tree(root, &shell);
		cleanup(tokens, root, NULL);
		free(line);
	}
	
	rl_clear_history();
	return (0);
}
