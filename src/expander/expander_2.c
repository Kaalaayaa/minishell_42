#include "minishell.h"

static size_t	handle_single_quote(const char *str, size_t i, char **res)
{
	size_t	j;
	char	*quoted_text;

	j = i + 1;
	while (str[j] && str[j] != '\'')
		j++;
	quoted_text = ft_substr(str, i + 1, j - i - 1);
	append_and_free(res, quoted_text);
	free(quoted_text);
	if (str[j] == '\'')
		i = j + 1;
	else
		i = j;
	return (i);
}

static size_t	env_exit_status(t_shell *sh, char **res)
{
	char	*status;

	status = ft_itoa(sh->exit_status);
	append_and_free(res, status);
	free(status);
	return (2);
}

static size_t	handle_env(const char *s, size_t i, t_shell *sh, char **res)
{
	size_t	j;
	char	*name;
	char	*value;

	if (s[i + 1] == '?')
		return (i + env_exit_status(sh, res));
	j = i + 1;
	while (s[j] && (ft_isalnum(s[j]) || s[j] == '_'))
		j++;
	if (j == i + 1)
	{
		append_and_free(res, "$");
		return (i + 1);
	}
	name = ft_substr(s, i + 1, j - i - 1);
	value = get_env_value(sh->env_list, name);
	append_env_value(res, value);
	free(name);
	return (j);
}

static size_t	copy_plain_text(const char *s, size_t i, char **res)
{
	size_t	j;
	char	*part;

	j = i;
	while (s[j] && s[j] != '$' && s[j] != '\'')
		j++;
	part = ft_substr(s, i, j - i);
	append_and_free(res, part);
	free(part);
	return (j);
}

char	*expand_env(const char *str, t_shell *shell)
{
	char	*res;
	size_t	i;

	if (!str)
		return (ft_strdup(""));
	res = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			i = handle_single_quote(str, i, &res);
		else if (str[i] == '$')
			i = handle_env(str, i, shell, &res);
		else
			i = copy_plain_text(str, i, &res);
	}
	return (res);
}
