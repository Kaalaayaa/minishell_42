/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/12/05 20:07:40 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_append(char *file, t_shell *shell)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		print_error(2, "minishell: %s%s%s%s", file, ": ", strerror(errno), "\n");
		shell->exit_status = 1;
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int	redir_output(char *file, t_shell *shell)
{
	int	fd;
	int	ret;

	ret = 1;
	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		print_error(2, "minishell: %s%s%s%s", file, ": ", strerror(errno), "\n");
		shell->exit_status = 1;
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (ret);
}

int	redir_input(char *file, t_shell *shell)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		print_error(2, "minishell: %s: %s\n", file, strerror(errno));
		shell->exit_status = 1;
		return (0);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

void	print_heredoc_error(char *line_number, const char *file)
{
	print_error(2, "minishell: warning: here-document at line %s%s%s%s",
		line_number, " delimited by end-of-file (wanted `", (char *)file, "')");
	free(line_number);
}
