/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 20:00:00 by kchatela          #+#    #+#             */
/*   Updated: 2025/12/05 20:10:52 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exec_cmd_prechecks(t_tree *tree, t_shell *shell)
{
	if (!tree || !tree->argv || !tree->argv[0])
		return (1);
	if (handle_var_assignment(tree, shell))
		return (1);
	if (run_parent_builtin(tree, shell))
		return (1);
	return (0);
}

int	exec_cmd_setup(t_tree *tree, t_shell *shell,
	char ***envp, char **path)
{
	*envp = get_envp(shell->env_list);
	*path = get_path(tree->argv[0], shell);
	if (check_path_unset(tree, shell, *envp, *path))
		return (1);
	if (!shell->in_pipe)
		setup_signals_parent();
	return (0);
}

void	exec_cmd_fork_exec(t_tree *tree, t_shell *shell,
	char **envp, char *path)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		free_exec_resources(envp, path);
		return ;
	}
	if (pid == 0)
		child_exec(tree, shell, envp, path);
	waitpid(pid, &status, 0);
	if (!shell->in_pipe)
		setup_signals_prompt();
	free_exec_resources(envp, path);
	handle_cmd_signal(status, shell);
}

int	env_count(t_env *env)
{
	int		count;
	t_env	*tmp;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

char	*env_join(char *key, char *value)
{
	char	*str;
	int		len_k;
	int		len_v;

	len_k = ft_strlen(key);
	len_v = ft_strlen(value);
	str = malloc(len_k + len_v + 2);
	if (!str)
		return (NULL);
	ft_memcpy(str, key, len_k);
	str[len_k] = '=';
	ft_memcpy(str + len_k + 1, value, len_v);
	str[len_k + len_v + 1] = '\0';
	return (str);
}
