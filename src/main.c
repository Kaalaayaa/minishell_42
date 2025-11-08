/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:19:27 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/06 17:03:58 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *ft_strncpy(char *src, char c)
{
    int i;
    char *ret;

    ret = NULL;
    i = 0;
    if (src[0] == '\'')
        return (ret);
    else if (src[0] == '"')
        src++;
    while(src[i] && src[i] != c)
        i++;
    ret = malloc(sizeof(char) * (i + 1));
    i = 0;
    while(src[i] && src[i] != c)
    {
        ret[i] = src[i];
        i++;
    }
    ret[i] = '\0';
    return (ret);
}

char *extract_line(t_shell *shell, char *s)
{
    t_env *list;


    list = shell->env_list;
    if (!shell || !s)
        return (NULL);

    while(list)
    {
        if (ft_strnstr(s, list->key, ft_strlen(s)) != NULL)
            return (ft_strjoin(ft_strncpy(s, '$'), ft_strdup(list->value)));
        list = list->next;
    }
    return (s);
}

t_token *expander(t_token *tokens, t_shell *shell)
{
    t_token *ret;

    if (!tokens)
        return NULL;
    ret = tokens;
    while(tokens)
    {
        if (ft_strchr(tokens->token, '$') != NULL)
            tokens->token = extract_line(shell, tokens->token);
        tokens = tokens->next;
    }
    return (ret);
}

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
		root = parse_e(&tokens);
		//print_tree(root, 0);
        exec_tree(root, &shell);
        free(line);
    }
    return (0);
}
