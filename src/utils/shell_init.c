/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/20 15:34:04 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	shell_init(t_shell *shell, char **envp)
{
	if (!shell)
		return ;
	shell->env_list = NULL;
	shell->in_pipe = false;
	shell->line_number = 0;
	if (envp)
		shell->env_list = env_list_init(envp);
	shell->exit_status = 0;
	update_shlvl(shell);
	setup_signals_prompt();
}
