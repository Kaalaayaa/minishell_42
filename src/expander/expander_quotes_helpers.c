/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdangwal <pdangwal@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 17:30:00 by pdangwal          #+#    #+#             */
/*   Updated: 2025/12/19 17:30:00 by pdangwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_single_sign(size_t i, char **res)
{
	append_and_free(res, "$");
	i = i + 1;
	return (i);
}

size_t	handle_double_quote(const char *str, size_t i, t_shell *shell,
	char **res)
{
	size_t	j;
	char	*content;
	char	*expanded;

	j = i + 1;
	while (str[j] && str[j] != '"')
	{
		if (str[j] == '\\' && str[j + 1])
			j++;
		j++;
	}
	content = ft_substr(str, i + 1, j - i - 1);
	expanded = expand_env_inside_dquote(content, shell);
	append_and_free(res, expanded);
	free(expanded);
	free(content);
	if (str[j] == '"')
		i = j + 1;
	else
		i = j;
	return (i);
}
