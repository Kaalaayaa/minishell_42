/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdangwal <pdangwal@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 17:10:00 by pdangwal          #+#    #+#             */
/*   Updated: 2025/12/19 17:10:00 by pdangwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**get_envp(t_env *env)
{
	char	**ret;
	t_env	*tmp;
	int		count;
	int		i;

	count = env_count(env);
	ret = malloc(sizeof(char *) * (count + 1));
	if (!ret)
		return (NULL);
	tmp = env;
	i = 0;
	while (tmp)
	{
		ret[i] = env_join(tmp->key, tmp->value);
		if (!ret[i])
		{
			free_split(ret);
			return (NULL);
		}
		i++;
		tmp = tmp->next;
	}
	ret[i] = NULL;
	return (ret);
}
