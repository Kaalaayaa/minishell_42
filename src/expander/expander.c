#include "minishell.h"
#include <ctype.h>

static void	update_quote(char c, char *q)
{
	if (!*q && (c == '\'' || c == '"'))
		*q = c;
	else if (c == *q)
		*q = 0;
}

static char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	in_quote;
	char	*res;

	if (!str)
		return (NULL);
	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	in_quote = 0;
	while (str[i])
	{
		if (in_quote == '"' && str[i] == '\\' && str[i + 1])
		{
			i++;
			res[j++] = str[i];
			i++;
			continue;
		}
		update_quote(str[i], &in_quote);
		if (!((str[i] == '\'' || str[i] == '"') && (in_quote == 0 || in_quote == str[i])))
			res[j++] = str[i];
		i++;
	}
	res[j] = '\0';
	return (res);
}

t_token	*expander(t_token *tokens, t_shell *shell)
{
	t_token	*head;
	char	*old;
	char *old2;

	if (!tokens)
		return (NULL);
	head = tokens;
	while (tokens)
	{
		if (tokens->token && ft_strcmp(tokens->token, "$EMPTY") == 0)
		{
				tokens = tokens->next;
				if (tokens == NULL)
					break;
		}	
		if (tokens->token && ft_strchr(tokens->token, '$'))
		{
			old = tokens->token;
			tokens->token = expand_env(old, shell);
			free(old);
		}
		old2 = tokens->token;
		tokens->token = remove_quotes(tokens->token);
		if (old2 && old2 != tokens->token)
			free(old2);
		tokens = tokens->next;
	}
	return (head);
}
