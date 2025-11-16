#include "minishell.h"

int	is_valid_exit_nbr(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	get_exit_status(const char *str)
{
	int			i;
	int			sign;
	long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return ((result * sign) % 256);
}

int	builtin_exit(char **argv, t_shell *shell)
{
	int	exit_status;

	exit_status = shell->exit_status;
    // if no second arg exit with last exit_status
	if (!argv[1])
		exit(exit_status);
    //handle error too many args
	if (argv[2] && is_valid_exit_nbr(argv[1]))
	{
		printf("exit: too many arguments\n");
		shell->exit_status = 1;
		return (1);
	}
    //handle error non numeric argument and exit with status 255
	if (!is_valid_exit_nbr(argv[1]))
	{
		printf("exit: %s: numeric argument required\n", argv[1]);
		shell->exit_status = 255;
		exit(255);
	}
    // get exit status from second arg and exit
	exit_status = get_exit_status(argv[1]);
	exit(exit_status);
}
