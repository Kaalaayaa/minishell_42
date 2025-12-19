/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/12/05 19:51:23 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static size_t	get_env_var_value(const char *s, size_t i, t_shell *sh,
	char **res)
{
	size_t	j;
	char	*name;
	char	*value;

	j = i + 1;
	while (s[j] && (ft_isalnum(s[j]) || s[j] == '_'))
		j++;
	if (j == i + 1)
	{
		append_and_free(res, "$");
		return (i + 1);
	}
	name = ft_substr(s, i + 1, j - i - 1);
	value = get_env_value(sh->env_list, name);
	append_env_value(res, value);
	free(name);
	return (j);
}

size_t	handle_env(const char *s, size_t i, t_shell *sh, char **res)
{
	char	*pid_str;

	if (s[i + 1] == '?')
		return (i + env_exit_status(sh, res));
	if (s[i + 1] == '$')
	{
		pid_str = ft_itoa(getpid());
		append_and_free(res, pid_str);
		free(pid_str);
		return (i + 2);
	}
	return (get_env_var_value(s, i, sh, res));
}

size_t	copy_plain_text(const char *s, size_t i, char **res)
{
	size_t	j;
	char	*part;

	j = i;
	while (s[j] && s[j] != '\'' && s[j] != '"')
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

static size_t	process_expand_char(const char *str, size_t i, t_shell *shell,
	char **res)
{
	if (str[i] == '"')
		return (handle_double_quote(str, i, shell, res));
	else if (str[i] == '\'')
		return (handle_single_quote(str, i, res));
	else if (str[i] == '$' && !str[i + 1])
		return (handle_single_sign(i, res));
	else if (str[i] == '$')
		return (handle_env(str, i, shell, res));
	else
		return (copy_plain_text(str, i, res));
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
		i = process_expand_char(str, i, shell, &res);
	return (res);
}
