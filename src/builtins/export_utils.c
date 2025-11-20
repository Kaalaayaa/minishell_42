/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/12 15:34:04 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*join_first(char *res, char *s1, int *i)
{
	while (s1[*i])
	{
		res[*i] = s1[*i];
		(*i)++;
	}
	return (res);
}

static char	*join_rest(char *res, char *str, int *pos)
{
	int	j;

	j = 0;
	while (str[j])
		res[(*pos)++] = str[j++];
	return (res);
}

char	*ft_strjoin_three(char *s1, char *s2, char *s3)
{
	int		len;
	int		i;
	char	*res;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	res = malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	i = 0;
	res = join_first(res, s1, &i);
	res = join_rest(res, s2, &i);
	res = join_rest(res, s3, &i);
	res[i] = '\0';
	return (res);
}

int	env_size(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

char	*env_to_str(t_env *env)
{
	char	*str;

	if (!env || !env->key)
		return (NULL);
	if (env->value)
	{
		str = ft_strjoin_three(env->key, "=", env->value);
		if (!str)
			return (NULL);
		return (str);
	}
	str = ft_strdup(env->key);
	if (!str)
		return (NULL);
	return (str);
}
