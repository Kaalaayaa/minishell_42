/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/12/05 19:50:15 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_quote_state
{
	int		i;
	int		j;
	char	in_quote;
	char	*res;
}	t_quote_state;

static void	process_char(char *str, t_quote_state *state)
{
	if (str[state->i] == '\x1f')
	{
		state->i++;
		state->res[state->j++] = str[state->i];
	}
	else if (state->in_quote == '"' && str[state->i]
		== '\\' && str[state->i + 1])
	{
		state->res[state->j] = str[++state->i];
		state->j++;
	}
	else if ((str[state->i] == '\'' || str[state->i] == '"')
		&& (state->in_quote == 0 || state->in_quote == str[state->i]))
		update_quote(str[state->i], &state->in_quote);
	else
		state->res[state->j++] = str[state->i];
}

static char	*remove_quotes(char *str)
{
	t_quote_state	state;

	if (!str)
		return (NULL);
	state.res = malloc(ft_strlen(str) + 1);
	if (!state.res)
		return (NULL);
	state.i = 0;
	state.j = 0;
	state.in_quote = 0;
	while (str[state.i])
	{
		process_char(str, &state);
		state.i++;
	}
	state.res[state.j] = '\0';
	return (state.res);
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
	if (token->token && token->token[0] == '\0')
	{
		free(token->token);
		token->token = ft_strdup("$EMPTY");
	}
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
