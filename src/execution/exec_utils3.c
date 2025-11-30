/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdangwal <pdangwal@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:28:32 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/20 19:28:33 by pdangwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	write_lines(char *argv)
{
	int	fd[2];

	pipe(fd);
	if (fork() == 0)
	{
		close(fd[0]);
		write(fd[1], argv, ft_strlen(argv));
		write(fd[1], "\n", 1);
		close(fd[1]);
		exit(0);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
}

void	update_exit_status(int status, t_shell *shell)
{
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}

void	print_and_exit(char *s1, char *s2, char *s3, int exitcode)
{
	print_error(s1, s2, s3);
	exit(exitcode);
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
