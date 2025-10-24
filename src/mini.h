/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdangwal <pdangwal@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 09:34:36 by pdangwal          #+#    #+#             */
/*   Updated: 2025/10/24 19:45:29 by pdangwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../libft/libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#ifndef TYPES_H
# define TYPES_H

typedef enum e_type
{
	WORD,
	PIPE,
	GREAT,
	DGREAT,
	LESS,
	DLESS,
	END
}					t_type;
#endif

typedef struct s_token
{
	char			*token;
	enum e_type		type;
	struct s_token	*next;
}					t_token;

typedef struct s_tree
{
	enum e_type		type;
	char			*token;
	char			**argv;
	struct s_tree	*right;
	struct s_tree	*left;
}					t_tree;

/*lexing/tokenisation utils*/
t_token				*lexer(char *input);
int					ft_isspace(char c);
int					ft_strlen(const char *s);
char				*ft_strdup(const char *s);
size_t				ft_strlcpy(char *dest, const char *src, size_t size);
int					is_operator_start(char c);

/*parsing functions*/
void				print_tree(t_tree *node, int depth);
t_tree				*parse_e(t_token **tokens);
