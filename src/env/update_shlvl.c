/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_shlvl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/12 15:34:04 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_shlvl_num(const char *shlvl_str)
{
	int	shlvl;

	if (!shlvl_str || !ft_isdigit(shlvl_str[0]))
		return (1);
	shlvl = ft_atoi(shlvl_str) + 1;
	return (shlvl);
}

void	update_shlvl(t_shell *shell)
{
	char	*shlvl_str;
	char	*new_shlvl;
	int		shlvl;

	shlvl_str = get_env_value(shell->env_list, "SHLVL");
	shlvl = get_shlvl_num(shlvl_str);
	new_shlvl = ft_itoa(shlvl);
	if (!new_shlvl)
		return ;
	add_or_update_env(&shell->env_list, "SHLVL", new_shlvl);
	free(new_shlvl);
}
