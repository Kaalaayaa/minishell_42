/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 15:56:19 by pdangwal          #+#    #+#             */
/*   Updated: 2025/11/19 19:20:00 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strtrim_free(char *src, const char *set)
{
	char	*dest;

	if (!src)
		return (NULL);
	if (!set)
		return (src);
	dest = ft_strtrim(src, set);
	free(src);
	return (dest);
}
