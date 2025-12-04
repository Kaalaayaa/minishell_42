/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdangwal <pdangwal@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 00:00:00 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/30 00:00:00 by pdangwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_valid_identifier_prefix(char *str, int len)
{
	int	i;

	if (!str || len <= 0 || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (i < len)
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	handle_var_assignment(t_tree *tree, t_shell *shell)
{
	char	*equal;
	char	*key;
	char	*value;

	if (!tree || !tree->argv || !tree->argv[0])
		return (0);
	if (tree->argv[0][0] == '\0')
	{
		shell->exit_status = 0;
		return (1);
	}
	equal = ft_strchr(tree->argv[0], '=');
	if (equal && is_valid_identifier_prefix(tree->argv[0],
			equal - tree->argv[0]))
	{
		key = strndup(tree->argv[0], equal - tree->argv[0]);
		value = equal + 1;
		add_or_update_env(&shell->env_list, key, value);
		free(key);
		shell->exit_status = 0;
		return (1);
	}
	return (0);
}

int	check_path_unset(t_tree *tree, t_shell *shell, char **envp, char *path)
{
	char	*cmd_path;

	if (!path && !ft_strchr(tree->argv[0], '/') && !get_path_env(shell))
	{
		cmd_path = ft_strjoin("./", tree->argv[0]);
		if (access(cmd_path, F_OK) != 0)
		{
			free_exec_resources(envp, path);
			print_error("minishell: ", tree->argv[0],
				": No such file or directory\n");
			free(cmd_path);
			shell->exit_status = 127;
			return (1);
		}
		free(cmd_path);
	}
	return (0);
}

void	handle_pipe_status(int status, t_shell *shell)
{
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(2, "Quit (core dumped)\n", 19);
	}
}

void	handle_cmd_signal(int status, t_shell *shell)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
	}
	update_exit_status(status, shell);
	if (!shell->in_pipe && WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		write(2, "Quit (core dumped)\n", 19);
}
