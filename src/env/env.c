/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:23:35 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/12 15:34:04 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_node_env(char *str)
{
	t_env	*node;
	char	*equal_sign;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	equal_sign = strchr(str, '=');
	if (equal_sign)
	{
		node->key = strndup(str, equal_sign - str);
		node->value = strdup(equal_sign + 1);
	}
	else
	{
		node->key = strdup(str);
		node->value = NULL;
	}
	node->next = NULL;
	return (node);
}

t_env	*env_list_init(char **envp)
{
	t_env	*head;
	t_env	*tail;
	t_env	*new_node;
	int		i;

	head = NULL;
	tail = NULL;
	i = -1;
	while (envp[++i])
	{
		new_node = create_node_env(envp[i]);
		if (!new_node)
			return (head);
		if (!head)
			head = new_node;
		else
			tail->next = new_node;
		tail = new_node;
	}
	return (head);
}
