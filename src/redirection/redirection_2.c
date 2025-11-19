#include "minishell.h"

void    delete_line(char **argv, int index)
{
    int i;

    if (!argv)
        return ;
    free(argv[index]);
    i = index;
    while(argv[i])
    {
        argv[i] = argv[i + 1];
        i++;
    }
}

static char *heredoc_collect(const char *file)
{
	char *line;
	char *new;

	new = malloc(1);
	if (!new)
		return (NULL);
	new[0] = '\0';
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_error("minishell: warning: here-document delimited by end-of-file (wanted `", (char *)file, "')\n");
			break ;
		}
		if (ft_strcmp(line, file) == 0)
		{
			free(line);
			break ;
		}
		new = ft_strjoin_free(new, "\n", 1);
		new = ft_strjoin_free(new, line, 3);
	}
	return (ft_strtrim_free(new, "\n"));
}

static void heredoc_child(const char *file, int fdw)
{
	char *new;

	setup_signals_heredoc();
	new = heredoc_collect(file);
	if (new)
		write(fdw, new, ft_strlen(new));
	close(fdw);
	exit(0);
}

static char *heredoc_parent(int fd_read, pid_t pid, t_shell *shell)
{
	int status;
	char buffer[1024];
	int bytes;

	setup_signals_parent();
	waitpid(pid, &status, 0);
	setup_signals_prompt();
	if (WIFEXITED(status))
	{
		shell->exit_status = WEXITSTATUS(status);
		if (shell->exit_status == 130)
			g_signal_status = 1;
	}
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	bytes = read(fd_read, buffer, sizeof(buffer) - 1);
	buffer[bytes] = '\0';
	close(fd_read);
	return (ft_strdup(buffer));
}

char	*get_heredoc(char *file, t_shell *shell)
{
	pid_t   pid;
    int     fd[2];

	if (pipe(fd) == -1)
		return (NULL);
	pid = fork();
	if (pid == -1)
		return (NULL);
	if (pid == 0)
	{
		close(fd[0]);
		heredoc_child(file, fd[1]);
	}
	close(fd[1]);
	return (heredoc_parent(fd[0], pid, shell));
}