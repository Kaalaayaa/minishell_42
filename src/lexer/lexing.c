/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/12 15:34:04 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token(t_token **tokens, enum e_type type, char *s)
{
	t_token	*new;
	t_token	*tmp;

	new = malloc(sizeof(*new));
	if (!new)
		return ;
	new->token = s;
	new->type = type;
	new->next = NULL;
	if (*tokens == NULL)
		*tokens = new;
	else
	{
		tmp = *tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

int	word_handle(const char *s, t_token **token)
{
	int		i;
	char	qoute;

	i = 0;
	while (s[i] && !ft_isspace(s[i]) && !is_operator_start(s[i]))
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			qoute = s[i++];
			while (s[i] && s[i] != qoute)
			{
				if (qoute == '"' && s[i] == '\\' && s[i + 1])
					i++;
				i++;
			}
			if (s[i] == '\0')
				return (i);
			i++;
		}
		else
			i++;
	}
	add_token(token, WORD, ft_strndup(s, i));
	return (i);
}

char	*adjust_input(char *i, t_token **tokens, enum e_type type, char *s)
{
	add_token(tokens, type, ft_strdup(s));
	i += ft_strlen(s);
	return (i);
}

t_token	*lexer(char *input)
{
	t_token	*tokens;

	tokens = NULL;
	while (*input)
	{
		if (ft_isspace(*input))
			input++;
		else if (*input == '|')
			input = adjust_input(input, &tokens, PIPE, "|");
		else if (*input == '>' && *(input + 1) == '>')
			input = adjust_input(input, &tokens, WORD, ">>");
		else if (*input == '>')
			input = adjust_input(input, &tokens, WORD, ">");
		else if (*input == '<' && *(input + 1) == '<')
			input = adjust_input(input, &tokens, WORD, "<<");
		else if (*input == '<')
			input = adjust_input(input, &tokens, WORD, "<");
		else
			input += word_handle(input, &tokens);
	}
	return (tokens);
}
