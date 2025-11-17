/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:19:27 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/12 18:38:07 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    check_syntax(t_tree *tree);

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
	(void)envp;

    t_shell shell;
    char *line;
	t_token *tokens;
	t_tree *root;

    shell_init(&shell, envp);

    while (1)
    {
        line = readline("minishell% ");
        if(!line)
        {
            printf("exit\n");
            break ;
        }
        if (line[0] != '\0')
            add_history(line);
        
        if (g_signal_status == 1)
		{
			shell.exit_status = 130;
			g_signal_status = 0;
		}
        
		tokens = lexer(line);
        tokens = expander(tokens, &shell);
        tokens = syntax(tokens, &shell);
		root = parse_e(&tokens, &shell);
		if (g_signal_status == 1)
		{
			shell.exit_status = 130;
			g_signal_status = 0;
			free(line);
			continue;
		}
		//print_tree(root, 0);
        exec_tree(root, &shell);
        free(line);
    }
    return (0);
}

void check_syntax(t_tree *tree)
{
    if (!tree)
        return;

    if (!tree->argv && tree->redirections)
    {
        fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
        return;
    }
    if (tree->type == PIPE)
    {
        if (!tree->left || (!tree->left->argv && !tree->left->redirections))
        {
            fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
            return;
        }
        if (!tree->right || (!tree->right->argv && !tree->right->redirections))
        {
            fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
            return;
        }
    }
    check_syntax(tree->left);
    check_syntax(tree->right);
}
