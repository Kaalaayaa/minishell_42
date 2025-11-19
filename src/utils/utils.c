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

#include "minishell.h"

void	write_string(char *s, int fd)
{
	int i;
	char c;
	i = 0;
	while(s[i])
	{
		c = s[i];
		write(fd, &c, 1);
		i++;
	}
}

void	print_error(char *s1, char *s2, char *s3)
{
	if (s1 != NULL)
		write_string(s1, 2);
	if (s2 != NULL)
		write_string(s2, 2);
	if (s3 != NULL)
		write_string(s3, 2);
}

char *ft_strtrim_free(char *src, const char *set)
{
	char *dest;

	if (!src)
		return (NULL);
	if (!set)
		return (src);
	dest = ft_strtrim(src, set);
	free(src);	
	return (dest);
}
