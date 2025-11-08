/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 09:34:36 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/04 19:06:12 by kchatela         ###   ########.fr       */
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
# include <time.h>
# include <unistd.h>
# include <stdbool.h>
# include <limits.h>
# include <termios.h>
# include <fcntl.h>

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
	int				exit_status;
}			t_shell;

/* ************************** */
/*       LEXER / TOKENIZER     */
/* ************************** */

t_token		*lexer(char *input);
int			ft_isspace(char c);
char		*ft_strdup(const char *s);
size_t		ft_strlcpy(char *dest, const char *src, size_t size);
int			is_operator_start(char c);

/* ************************** */
/*           PARSING           */
/* ************************** */

void		print_tree(t_tree *node, int depth);
t_tree		*parse_e(t_token **tokens);
t_redir *apply_redirections(char **argv);

/* ************************** */
/*           BUILTINS          */
/* ************************** */

void		execute_builtin(char **cmd, t_shell *shell);
bool		is_builtin(const char *cmd);
int			builtin_echo(char **argv);
int			builtin_pwd(void);
int			builtin_env(t_shell *shell);
int			builtin_exit(char **argv, t_shell *shell);
int			builtin_export(char **argv, t_shell *shell);
int			builtin_unset(char **argv, t_shell *shell);
int			builtin_cd(char **argv, t_shell *shell);

/* ************************** */
/*             UTILS           */
/* ************************** */
void		shell_init(t_shell *shell, char **envp);
int			is_valid_identifier(char *key);
int			ft_strcmp(const char *s1, const char *s2); // ADD to LIBFT !!!
void		add_or_update_env(t_env **env, char *key, char *value);
char		*env_to_str(t_env *env);
void		ft_sort_str_tab(char **tab);
int			env_size(t_env *env);
char		*get_env_value(t_env *env, const char *key);
void	redir_heredoc(char *file);
/* ************************** */
/*        ENV                 */
/* ************************** */

t_env		*env_list_init(char **envp);
void		update_shlvl(t_shell *shell);

/* ************************** */
/*	    EXECUION	      */
/* ************************** */

void	exec_tree(t_tree *tree, t_shell *shell);
void    ft_trim_end(char *arr, char c);

/* ************************** */
/*	    SIGNALS      	      */
/* ************************** */

void setup_signals_prompt(void);
void	setup_signals_child(void);
void    setup_signals_parent(void);

#endif
