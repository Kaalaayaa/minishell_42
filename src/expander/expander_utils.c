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

char	*ft_strjoin_free(char *s1, char *s2, int flag)
{
	char	*res;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	res = ft_strjoin(s1, s2);
	if (flag == 1 || flag == 3)
		free(s1);
	if (flag == 2 || flag == 3)
		free(s2);
	return (res);
}

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
