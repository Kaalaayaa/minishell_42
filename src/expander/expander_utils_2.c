/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/12/05 17:46:17 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_char(char **res, char ch)
{
	char	c[2];

	c[0] = ch;
	c[1] = '\0';
	append_and_free(res, c);
}

static void	append_marked_char(char **res, char ch)
{
	char	c[3];

	c[0] = '\x1f';
	c[1] = ch;
	c[2] = '\0';
	append_and_free(res, c);
}

static size_t	handle_dquote_var(const char *str, size_t i, t_shell *shell,
	char **res)
{
	size_t	j;
	char	*name;
	char	*value;
	char	*pid_str;

	if (str[i + 1] == '?')
		return (i + env_exit_status(shell, res));
	if (str[i + 1] == '$')
	{
		pid_str = ft_itoa(getpid());
		append_and_free(res, pid_str);
		free(pid_str);
		return (i + 2);
	}
	j = i + 1;
	while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
		j++;
	name = ft_substr(str, i + 1, j - i - 1);
	value = get_env_value(shell->env_list, name);
	append_env_value(res, value);
	free(name);
	return (j);
}

static size_t	process_dquote_char(const char *str, size_t i, t_shell *shell,
	char **res)
{
	if (str[i] == '$' && (str[i + 1] == '?' || ft_isalnum(str[i + 1])
			|| str[i + 1] == '_'))
		return (handle_dquote_var(str, i, shell, res));
	else if (str[i] == '\'' || str[i] == '"')
	{
		append_marked_char(res, str[i]);
		return (i + 1);
	}
	else
	{
		append_char(res, str[i]);
		return (i + 1);
	}
}

char	*expand_env_inside_dquote(const char *str, t_shell *shell)
{
	char	*res;
	size_t	i;

	if (!str)
		return (ft_strdup(""));
	res = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1])
			i++;
		i = process_dquote_char(str, i, shell, &res);
	}
	return (res);
}
