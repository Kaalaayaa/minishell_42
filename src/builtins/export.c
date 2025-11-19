#include "minishell.h"

void	print_sorted_env(t_env *env)
{
	int		i;
	char	**arr;
	t_env	*curr;

	arr = malloc(sizeof(char *) * (env_size(env) + 1));
	if (!arr)
		return ;
	i = 0;
	curr = env;
	while (curr)
	{
		arr[i] = env_to_str(curr);
		curr = curr->next;
		i++;
	}
	arr[i] = NULL;
	ft_sort_str_tab(arr);
	i = 0;
	while (arr[i])
	{
		printf("declare -x %s\n", arr[i]);
		free(arr[i++]);
	}
	free(arr);
}

static int	handle_env_var(char *arg, t_shell *shell)
{
	char	*equal;
	char	*key;
	char	*value;

	equal = ft_strchr(arg, '=');
	if (equal)
	{
		key = strndup(arg, equal - arg); // ADD ft_strndup to LIBFT !!!!
		value = equal + 1;
	}
	else
	{
		key = ft_strdup(arg);
		value = NULL;
	}
	if (!is_valid_identifier(key))
	{
		print_error("minishell: export: `", key, "': not a valid identifier\n");
		free(key);
		return (1);
	}
	add_or_update_env(&shell->env_list, key, value);
	free(key);
	return (0);
}

int	builtin_export(char **argv, t_shell *shell)
{
	int	i;
	int	exit_status;

	if (!argv[1])
		return (print_sorted_env(shell->env_list), 0);
	i = 1;
	exit_status = 0;
	while (argv[i])
	{
		if (handle_env_var(argv[i], shell))
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
