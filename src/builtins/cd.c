#include "minishell.h"

char	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	change_directory(char *target, char *oldcwd, t_shell *shell, int print_path)
{
	char	newcwd[PATH_MAX];
    //change dir and if return -1 print error
	if (chdir(target) != 0)
	{
		perror("cd");
		shell->exit_status = 1;
		return (1);
	}
	if (getcwd(newcwd, sizeof(newcwd)))
	{
		add_or_update_env(&shell->env_list, "OLDPWD", oldcwd);
		add_or_update_env(&shell->env_list, "PWD", newcwd);
		if (print_path)
			printf("%s\n", newcwd);
	}
	shell->exit_status = 0;
	return (0);
}

int	builtin_cd(char **argv, t_shell *shell)
{
	char	*target;
	char	oldcwd[PATH_MAX];
	int		print_path;

	print_path = 0;
    //save current directory in oldcwd
	if (getcwd(oldcwd, sizeof(oldcwd)) == NULL)
		oldcwd[0] = '\0';
    //handle cd or cd ~ 
	if (!argv[1] || ft_strcmp(argv[1], "~") == 0) // if argv[1][0] == "~"
		target = get_env_value(shell->env_list, "HOME");
    // handle cd - 
	else if (ft_strcmp(argv[1], "-") == 0) //if argv[1][0] == "-"
	{
		target = get_env_value(shell->env_list, "OLDPWD");
		print_path = 1;
	}
    //handle normal directory
	else
		target = argv[1];
	if (!target)
	{
		printf("cd: HOME not set\n");
		shell->exit_status = 1;
		return (1);
	}
	return (change_directory(target, oldcwd, shell, print_path));
}
