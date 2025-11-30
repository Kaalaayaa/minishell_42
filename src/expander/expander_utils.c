/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/12 15:34:04 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_env_key(char *s)
{
	int		i;
	char	*key;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
		i++;
	key = malloc(sizeof(char) * (i + 1));
	if (!key)
		return (NULL);
	ft_strlcpy(key, s, i + 1);
	return (key);
}

void	append_and_free(char **res, const char *add)
{
	char	*tmp;

	tmp = ft_strjoin(*res, add);
	free(*res);
	*res = tmp;
}

void	append_env_value(char **res, char *value)
{
	char	*env;

	if (value)
		env = ft_strdup(value);
	else
		env = ft_strdup("");
	append_and_free(res, env);
	free(env);
}

void	update_quote(char c, char *q)
{
	if (!*q && (c == '\'' || c == '"'))
		*q = c;
	else if (c == *q)
		*q = 0;
}

size_t	env_exit_status(t_shell *sh, char **res)
{
	char	*status;

	status = ft_itoa(sh->exit_status);
	append_and_free(res, status);
	free(status);
	return (2);
}
