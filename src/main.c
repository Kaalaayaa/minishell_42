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

static void	check_signal(t_shell *shell)
{
	if (g_signal_status == 1)
	{
		shell->exit_status = 130;
		g_signal_status = 0;
	}
}

static void	process_command(char *line, t_shell *shell)
{
	t_token	*tokens;
	t_tree	*root;

	if (line[0] != '\0')
		add_history(line);
	check_signal(shell);
	tokens = lexer(line);
	tokens = expander(tokens, shell);
	tokens = syntax(tokens, shell);
	root = parse_e(&tokens, shell);
	if (g_signal_status == 1)
	{
		check_signal(shell);
		cleanup(tokens, root, NULL);
		return ;
	}
	exec_tree(root, shell);
	cleanup(tokens, root, NULL);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*line;

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
		process_command(line, &shell);
		free(line);
	}
	rl_clear_history();
	return (0);
}
