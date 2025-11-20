/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/12 15:34:04 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <ctype.h>

static void	update_quote(char c, char *q)
{
	if (!*q && (c == '\'' || c == '"'))
		*q = c;
	else if (c == *q)
		*q = 0;
}

static char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	in_quote;
	char	*res;

	if (!str)
		return (NULL);
	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	in_quote = 0;
	while (str[i])
	{
		if (in_quote == '"' && str[i] == '\\' && str[i + 1])
		{
			i++;
			res[j++] = str[i];
			i++;
			continue ;
		}
		update_quote(str[i], &in_quote);
		if (!((str[i] == '\'' || str[i] == '"')
		&& (in_quote == 0 || in_quote == str[i])))
			res[j++] = str[i];
		i++;
	}
	res[j] = '\0';
	return (res);
}

static void	refresh_token(t_token *token, t_shell *shell)
{
	char	*old;

	if (token->token && ft_strchr(token->token, '$'))
	{
		old = token->token;
		token->token = expand_env(old, shell);
		free(old);
	}
	old = token->token;
	token->token = remove_quotes(token->token);
	if (old && old != token->token)
		free(old);
}

t_token	*expander(t_token *tokens, t_shell *shell)
{
	t_token	*head;

	if (!tokens)
		return (NULL);
	head = tokens;
	while (tokens)
	{
		if (tokens->token && ft_strcmp(tokens->token, "$EMPTY") == 0)
		{
			tokens = tokens->next;
			if (tokens == NULL)
				break ;
		}
		refresh_token(tokens, shell);
		tokens = tokens->next;
	}
	return (head);
}
