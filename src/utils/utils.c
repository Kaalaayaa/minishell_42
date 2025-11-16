/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 15:56:19 by pdangwal          #+#    #+#             */
/*   Updated: 2025/10/31 17:09:45 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_words(char **s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

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