#include "../includes/minishell.h"

int	redir_append(char *file, t_shell *shell)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		print_error("minishell: ", file, ": ");
		print_error(strerror(errno), "\n", NULL);
		shell->exit_status = 1;
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int	redir_output(char *file, t_shell *shell)
{
	int fd;
	int ret;

	ret = 1;
	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		print_error("minishell: ", file, ": ");
		print_error(strerror(errno), "\n", NULL);
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
		print_error("minishell: ", file, ": ");
		print_error(strerror(errno), "\n", NULL);
		shell->exit_status = 1;
		return (0);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}
