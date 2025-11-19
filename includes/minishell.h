/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 09:34:36 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/19 19:29:47 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ************************** */
/*           INCLUDES          */
/* ************************** */

# include "../libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <time.h>
# include <unistd.h>
# include <stdbool.h>
# include <limits.h>
# include <termios.h>
# include <fcntl.h>
# include <errno.h> // for errno in strerror in redirections. not sure if this is allowed though
extern volatile sig_atomic_t g_signal_status;


/* ************************** */
/*            ENUMS            */
/* ************************** */

typedef enum e_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	END
}			t_type;

/* ************************** */
/*           STRUCTS           */
/* ************************** */

typedef struct s_token
{
	char			*token;
	t_type			type;
	struct s_token	*next;
}			t_token;

typedef struct s_redir
{
	t_type	type;
	char *filename;
	struct s_redir *next;
}	t_redir;

typedef struct s_tree
{
	t_type			type;
	t_redir			*redirections;
	char			*token;
	char			**argv;
	struct s_tree	*right;
	struct s_tree	*left;
}			t_tree;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}			t_env;

typedef struct s_shell
{
	t_env			*env_list;
	bool			in_pipe;
	int				exit_status;
}			t_shell;


/* ************************** */
/*         REDIRECTION        */
/* ************************** */

int	redir_output(char *filename, t_shell *shell);
int	redir_append(char *filename, t_shell *shell);
int	redir_input(char *filename, t_shell *shell);
t_redir *apply_redirections(char **argv, t_shell *shell);
int is_redirection(char *argv);
void	shell_init(t_shell *shell, char **envp);
int	redir_allocation(t_redir *redir, t_shell *shell);
char	*get_heredoc(char *file, t_shell *shell);
void    delete_line(char **argv, int index);

/* ************************** */
/*          LEXER              */
/* ************************** */

t_token	*lexer(char *input);
int		ft_isspace(char c);
int		is_operator_start(char c);
//size_t	ft_strlicpy(char *dest, const char *src, size_t size);
char    *ft_strndup(const char *s, size_t n);

/* ************************** */
/*          PARSER             */
/* ************************** */

t_tree	*parse_e(t_token **tokens, t_shell *shell);
void	print_tree(t_tree *node, int depth);
t_token *syntax(t_token *tokens, t_shell *shell);

/* ************************** */
/*         EXPANDER            */
/* ************************** */

t_token	*expander(t_token *tokens, t_shell *shell);
char	*expand_env(const char *s, t_shell *shell);
char	*extract_env_key(char *s);
void	append_and_free(char **res, const char *add);
void	append_env_value(char **res, char *value);
char	*ft_strjoin_free(char *s1, char *s2, int flag);

/* ************************** */
/*          BUILTINS           */
/* ************************** */

bool	is_builtin(const char *cmd);
int		execute_builtin(char **cmd, t_shell *shell);
int		builtin_echo(char **argv);
int		builtin_pwd(void);
int		builtin_env(t_shell *shell);
int		builtin_exit(char **argv, t_shell *shell);
int		builtin_export(char **argv, t_shell *shell);
int		builtin_unset(char **argv, t_shell *shell);
int		builtin_cd(char **argv, t_shell *shell);

/* ************************** */
/*         ENVIRONMENT          */
/* ************************** */

t_env	*env_list_init(char **envp);
char	*get_env_value(t_env *env, const char *key);
void	add_or_update_env(t_env **env, char *key, char *value);
void	update_shlvl(t_shell *shell);
int		is_valid_identifier(char *key);
int		env_size(t_env *env);
void	ft_sort_str_tab(char **tab);
char	*env_to_str(t_env *env);

/* ************************** */
/*         EXECUTION            */
/* ************************** */

void	exec_tree(t_tree *tree, t_shell *shell);
void	ft_trim_end(char *arr, char c);
char	*get_path(char *argv, t_shell *shell);

/* ************************** */
/*          SIGNALS             */
/* ************************** */

void	setup_signals_prompt(void);
void	setup_signals_child(void);
void	setup_signals_parent(void);
void	setup_signals_heredoc(void);

/* ************************** */
/*          CLEANUP           */
/* ************************** */

void	free_split(char **argv, int order);
void    cleanup(t_token *tokens, t_tree *tree, t_shell *shell);
void    free_redir(t_redir *redir);
void    free_tokens(t_token *tokens);
void    free_env(t_env *env);
void	free_exec_resources(char **envp, char *path);

/* ************************** */
/*         UTILS	          */
/* ************************** */
char *ft_strtrim_free(char *src, const char *set);
void	print_error(char *s1, char *s2, char *s3);


#endif