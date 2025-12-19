/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/12/05 20:10:53 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_trim_end(char *arr, char c)
{
	size_t	len;

	if (!arr)
		return ;
	len = ft_strlen(arr);
	if (len > 0 && arr[len - 1] == c)
		arr[len - 1] = '\0';
}

char	*get_path_env(t_shell *shell)
{
	t_env	*env;

	env = shell->env_list;
	while (env && env->key)
	{
		if (ft_strncmp(env->key, "PATH", 4) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*full_path(char *argv)
{
	if (!argv)
		return (NULL);
	if (access(argv, X_OK) == 0)
		return (ft_strdup(argv));
	return (NULL);
}

void	filter_empty_args(char **argv)
{
	int	i;
	int	j;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		if (ft_strcmp(argv[i], "$EMPTY") == 0)
		{
			j = i;
			while (argv[j])
			{
				argv[j] = argv[j + 1];
				j++;
			}
		}
		else
			i++;
	}
}

char	**check_path(char *argv, t_shell *shell)
{
	char	*path_env;
	char	**paths;

	if (!argv)
		return (NULL);
	path_env = get_path_env(shell);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	return (paths);
}

void	close_fd_in_range(int a, int b)
{
	a++;
	while (a < b)
	{
		close(a);
		a++;
	}
}
