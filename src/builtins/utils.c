#include "minishell.h"

int	is_valid_identifier(char *key)
{
	int	i;

	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	add_or_update_env(t_env **env, char *key, char *value)
{
	t_env	*node;

	if (!env || !key)
		return;

	node = *env;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
		{
			free(node->value);
			if (value != NULL)
				node->value = ft_strdup(value);
			else
				node->value = NULL;
			return;
		}
		node = node->next;
	}

	node = malloc(sizeof(t_env));
	if (!node)
		return;

	node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;

	node->next = *env;
	*env = node;
}

static void swap_strings(char **s1, char **s2)
{
    char *tmp;

    tmp = *s1;
    *s1 = *s2;
    *s2 = tmp;
}

void ft_sort_str_tab(char **tab)
{
    int i;
    int j;

    if (!tab)
        return;
    i = 0;
    while (tab[i])
    {
        j = i + 1;
        while (tab[j])
        {
            if (ft_strcmp(tab[i], tab[j]) > 0)
                swap_strings(&tab[i], &tab[j]);
            j++;
        }
        i++;
    }
}
