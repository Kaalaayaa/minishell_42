/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/12 15:34:04 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	escape_quotes(char *quoted_text, char *escaped)
{
	size_t	n;
	size_t	k;

	n = 0;
	k = 0;
	while (quoted_text[n])
	{
		if (quoted_text[n] == '"' || quoted_text[n] == '\'')
			escaped[k++] = '\x1f';
		escaped[k++] = quoted_text[n];
		n++;
	}
	escaped[k] = '\0';
}

static size_t	handle_single_quote(const char *str, size_t i, char **res)
{
	size_t	j;
	char	*quoted_text;
	char	*escaped;

	j = i + 1;
	while (str[j] && str[j] != '\'')
		j++;
	quoted_text = ft_substr(str, i + 1, j - i - 1);
	escaped = malloc(ft_strlen(quoted_text) * 2 + 1);
	if (!escaped)
		return (j + 1);
	escape_quotes(quoted_text, escaped);
	append_and_free(res, escaped);
	free(escaped);
	free(quoted_text);
	if (str[j] == '\'')
		i = j + 1;
	else
		i = j;
	return (i);
}

static size_t	handle_env(const char *s, size_t i, t_shell *sh, char **res)
{
	size_t	j;
	char	*name;
	char	*value;

	if (s[i + 1] == '?')
		return (i + env_exit_status(sh, res));
	j = i + 1;
	while (s[j] && (ft_isalnum(s[j]) || s[j] == '_'))
		j++;
	name = ft_substr(s, i + 1, j - i - 1);
	value = get_env_value(sh->env_list, name);
	append_env_value(res, value);
	free(name);
	return (j);
}

static size_t	copy_plain_text(const char *s, size_t i, char **res)
{
	size_t	j;
	char	*part;

	j = i;
	while (s[j] && s[j] != '\'')
	{
		if (s[j] == '$')
		{
			if (s[j + 1] == '"' || s[j + 1] == ' ' || s[j + 1] == '\t')
				j++;
			else
				break ;
		}
		j++;
	}
	part = ft_substr(s, i, j - i);
	append_and_free(res, part);
	free(part);
	return (j);
}

static int	handle_single_sign(size_t i, char **res)
{
	append_and_free(res, "$");
	i = i + 1;
	return (i);
}

char	*expand_env(const char *str, t_shell *shell)
{
	char	*res;
	size_t	i;

	if (!str)
		return (ft_strdup(""));
	res = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			i = handle_single_quote(str, i, &res);
		else if (str[i] == '$' && !str[i + 1])
			i = handle_single_sign(str[i], &res);
		else if (str[i] == '$')
			i = handle_env(str, i, shell, &res);
		else
			i = copy_plain_text(str, i, &res);
	}
	return (res);
}
