#include "minishell.h"

static void	remove_env_node(t_env **env, const char *key)
{
	t_env	*curr;
	t_env	*prev;

	curr = *env;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	builtin_unset(char **argv, t_shell *shell)
{
	int	i;
	int	exit_status;

	exit_status = 0;
	if (!argv[1])
		return (0);
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			printf("unset: `%s': invalid parameter name\n", argv[i]);
			exit_status = 1;
		}
		else
			remove_env_node(&shell->env_list, argv[i]);
		i++;
	}
	return (exit_status);
}
