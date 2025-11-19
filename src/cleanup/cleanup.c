#include "minishell.h"


void    free_split(char **argv, int order)
{
    int i;

    if (!argv)
        return ;
    if (order == 0) // default
    {
    i = 0;
    while(argv[i])
    {
        free(argv[i]);
        argv[i] = NULL;
        i++;
    }
    }
    else
    {
        i = order;
        while(i > 0)
        {
            free(argv[i]);
            argv[i] = NULL;
            i--;
        }
    }
    free(argv);
}

void    free_tokens(t_token *tokens)
{
    t_token *tmp;

    while(tokens)
    {
         tmp = tokens;
        tokens = tokens->next;
        free(tmp->token);
        free(tmp);
    }
}

void    free_redir(t_redir *redir)
{
    t_redir *tmp;

    if (!redir)
        return;
    while(redir)
    {
        if (redir->filename)
            free(redir->filename);
        tmp = redir;
        redir = redir->next;
        free(tmp);
        tmp = NULL;        
    }
}

void    free_tree(t_tree *tree)
{
    if (!tree)
        return ;
    free_tree(tree->left);
    tree->left = NULL;
    free_tree(tree->right);
    tree->right = NULL;
    if (tree->argv)
        free_split(tree->argv, 0);
    if (tree->redirections)
        free_redir(tree->redirections);
    if (tree->token)
        free(tree->token);
    free(tree);
}

void    free_env(t_env *env)
{
    t_env *tmp;

    if (!env)
        return ;
    while(env)
    {
        if (env->value)
            free(env->value);
        if (env->key)
            free(env->key);
        tmp = env;
        env = env->next;
        free(tmp);
        tmp = NULL;
    }
}

void    free_shell(t_shell *shell)
{
    if (!shell)
        return;
    if (shell->env_list)
        free_env(shell->env_list);
    //free(shell);
}

void    cleanup(t_token *tokens, t_tree *tree, t_shell *shell)
{
    if (tokens)
        free_tokens(tokens);
    if (tree)
        free_tree(tree);
    if (shell)
        free_shell(shell);
}