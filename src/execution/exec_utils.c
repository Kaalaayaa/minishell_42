#include "../includes/minishell.h"

void    ft_trim_end(char *arr, char c)
{
    size_t  len;

    if (!arr)
        return ;

    len = ft_strlen(arr);
    if (len > 0 && arr[len - 1] == c)
            arr[len-1] = '\0';
    
}


char	*get_path_env(t_shell *shell)
{
	t_env *env;

	env = shell->env_list;
	while(env->key)
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
	{
		return (ft_strdup(argv));
	}
	return (NULL);
}

char	**check_path(char *argv, t_shell *shell)
{
	char *path_env;
	char **paths;

	if (!argv)
		return (NULL);
	
	path_env = get_path_env(shell);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	return (paths);
}

char	*get_path(char *argv, t_shell *shell)
{
	char	*tmp;
	char	**paths;
	char 	*fullpath;
	int		i;

	if (ft_strchr(argv, '/'))
		return (full_path(argv));
	paths = check_path(argv, shell);
	if (!paths)
		return (NULL);
	i = 0;
	while(paths[i])
	{
		ft_trim_end(paths[i], ':');
		tmp = ft_strjoin(paths[i], "/");
		
		fullpath = ft_strjoin(tmp, argv);
		free(tmp);
		if (access(fullpath, X_OK) == 0)
		{
			free_split(paths, 0);
			return (fullpath);
		}
		free(fullpath);
		i++;
	}
	free_split(paths, 0);
	return (NULL);
	
}