/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/12 15:34:04 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	change_directory(char *target, char *oldcwd, t_shell *shell,
		int print_path)
{
	char	newcwd[PATH_MAX];

	if (chdir(target) != 0)
	{
		perror("cd");
		return (1);
	}
	if (getcwd(newcwd, sizeof(newcwd)))
	{
		add_or_update_env(&shell->env_list, "OLDPWD", oldcwd);
		add_or_update_env(&shell->env_list, "PWD", newcwd);
		if (print_path)
			printf("%s\n", newcwd);
	}
	return (0);
}

static char	*handle_cd_target(char **argv, t_shell *shell, int *pp)
{
	char	*target;

	if (!argv[1] || ft_strcmp(argv[1], "~") == 0)
	{
		target = get_env_value(shell->env_list, "HOME");
		if (!target)
			print_error(2, "minishell: cd: HOME not set\n");
		return (target);
	}
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		target = get_env_value(shell->env_list, "OLDPWD");
		*pp = 1;
		if (!target)
			print_error(2, "minishell: cd: OLDPWD not set\n");
		return (target);
	}
	return (argv[1]);
}

int	builtin_cd(char **argv, t_shell *shell)
{
	char	*target;
	char	oldcwd[PATH_MAX];
	int		print_path;

	print_path = 0;
	if (argv[1] && argv[2])
	{
		print_error(2, "minishell: cd: %s", "too many arguments\n");
		return (1);
	}
	if (getcwd(oldcwd, sizeof(oldcwd)) == NULL)
		oldcwd[0] = '\0';
	target = handle_cd_target(argv, shell, &print_path);
	if (!target)
		return (1);
	return (change_directory(target, oldcwd, shell, print_path));
}
