/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdangwal <pdangwal@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:27:07 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/20 19:27:24 by pdangwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_path(char *argv, t_shell *shell)
{
	char	*tmp;
	char	**paths;
	char	*fullpath;
	int		i;

	if (ft_strchr(argv, '/'))
		return (full_path(argv));
	paths = check_path(argv, shell);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		ft_trim_end(paths[i], ':');
		tmp = ft_strjoin(paths[i++], "/");
		fullpath = ft_strjoin_free(tmp, argv, 1);
		if (access(fullpath, X_OK) == 0)
		{
			free_split(paths);
			return (fullpath);
		}
		free(fullpath);
	}
	free_split(paths);
	return (NULL);
}

void	check_slash_path(char **envp, char *path, t_tree *tree)
{
	struct stat	st;

	if (stat(tree->argv[0], &st) != 0)
	{
		free_exec_resources(envp, path);
		print_and_exit("minishell: ", tree->argv[0],
			": No such file or directory\n", 127);
	}
	if (S_ISDIR(st.st_mode))
	{
		free_exec_resources(envp, path);
		print_and_exit("minishell: ", tree->argv[0], ": Is a directory\n", 126);
	}
}

void	check_normal_path(char **envp, char *path, t_tree *tree)
{
	struct stat	st;

	if (!path)
	{
		free_exec_resources(envp, path);
		print_and_exit("minishell: ", tree->argv[0], ": command not found\n",
			127);
	}
	if (stat(path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			free_exec_resources(envp, path);
			print_and_exit("minishell: ", tree->argv[0], ": Is a directory\n",
				126);
		}
	}
}

void	verify_path(char **envp, char *path, t_tree *tree)
{
	if (ft_strchr(tree->argv[0], '/'))
	{
		check_slash_path(envp, path, tree);
		return ;
	}
	check_normal_path(envp, path, tree);
}

void	free_exec_resources(char **envp, char *path)
{
	if (envp)
		free_split(envp);
	if (path)
		free(path);
}
