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

static int	append_str_to_buf(char *buf, int pos, const char *str, int max)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] && pos + i < max - 1)
	{
		buf[pos + i] = str[i];
		i++;
	}
	return (i);  // Returns number of chars written
}

void	print_error(int fd, char *set, ...)
{
	char	buffer[4096];
	va_list list;
	int		i;
	int		buf_pos;

	i = 0;
	buf_pos = 0;
	va_start(list, set);
	while(set[i] && buf_pos < 4095)
	{
		if (set[i] == '%' || set[i] == '\\')
		{
			if (set[i + 1] && set[i + 1] == '%')
			{
				buffer[buf_pos++] = set[i + 1];
				i++;  // Skip the format character
			}
			else if (set[i + 1] && set[i + 1] == 's')
			{
				buf_pos += append_str_to_buf(buffer, buf_pos, 
				                              va_arg(list, char *), 4096);
				i++;  // Skip the format character
			}
			/*else if (set[i + 1] && (set[i + 1] == 'd' || set[i + 1] == 'i'))
			{
				buf_pos += append_nbr_to_buf(buffer, buf_pos, 
				                              va_arg(list, int), 4096);
				i++;  // Skip the format character
			}*/
			else if (set[i] == '\\' && set[i + 1])
			{
				buffer[buf_pos++] = set[i + 1];
				i++;  // Skip the format character
			}
			else if (set[i + 1] && (set[i + 1] == '\'' || set[i + 1] == '"'))
			{
				buffer[buf_pos++] = set[i + 1];
				i++;  // Skip the format character
			}
			else if (!set[i + 1])
			{
				buffer[buf_pos++] = set[i];
			}
			else
			{
				buf_pos += append_str_to_buf(buffer, buf_pos, 
				                              "format not recognised\n", 4096);
				break;
			}
		}
		else
		{
			buffer[buf_pos++] = set[i];
		}
		i++;
	}
	va_end(list);
	if (buf_pos > 0)
		write(fd, buffer, buf_pos);
}

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
