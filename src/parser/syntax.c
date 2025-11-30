/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/12 15:34:04 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_syntax_error(const char *token)
{
	if (token)
		write(STDERR_FILENO,
			"minishell: syntax error near unexpected token `", 47);
	else
		write(STDERR_FILENO,
			"minishell: syntax error near unexpected token `newline'\n", 56);
	if (token)
	{
		write(STDERR_FILENO, token, ft_strlen(token));
		write(STDERR_FILENO, "'\n", 2);
	}
}

static int	check_redirection_syntax(t_token *current)
{
	t_token	*next;

	next = current->next;
	if (next == NULL)
	{
		print_syntax_error(NULL);
		return (0);
	}
	if (is_redirection(next->token))
	{
		print_syntax_error(next->token);
		return (0);
	}
	if (next->type == PIPE)
	{
		print_syntax_error(next->token);
		return (0);
	}
	return (1);
}

static int	check_pipe_syntax(t_token *current, t_token *tokens)
{
	t_token	*next;

	next = current->next;
	if (current == tokens || !next || next->type == PIPE)
	{
		print_syntax_error("|");
		return (0);
	}
	return (1);
}

t_token	*syntax(t_token *tokens, t_shell *shell)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (is_redirection(current->token))
		{
			if (!check_redirection_syntax(current))
				return (shell->exit_status = 2, NULL);
		}
		else if (current->type == PIPE)
		{
			if (!check_pipe_syntax(current, tokens))
				return (shell->exit_status = 2, NULL);
		}
		current = current->next;
	}
	return (tokens);
}
