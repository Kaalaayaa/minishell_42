/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error_format.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdangwal <pdangwal@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 17:10:00 by pdangwal          #+#    #+#             */
/*   Updated: 2025/12/19 17:46:00 by pdangwal         ###   ########.fr       */
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
	return (i);
}

static int	handle_format_quote(char *buf, int pos, char next)
{
	if (next == '\'' || next == '"')
	{
		buf[pos] = next;
		return (1);
	}
	return (0);
}

static int	handle_escape(char *buf, int pos, char next)
{
	if (next)
	{
		buf[pos] = next;
		return (1);
	}
	return (0);
}

int	process_special_char(char *set, int i, t_fmt *fmt)
{
	int	written;

	written = 0;
	if (set[i] == '\\' && set[i + 1])
		written = handle_escape(fmt->buf, fmt->pos, set[i + 1]);
	else if (set[i + 1])
		written = handle_format_quote(fmt->buf, fmt->pos, set[i + 1]);
	else if (!set[i + 1])
	{
		fmt->buf[fmt->pos] = set[i];
		written = 1;
	}
	else
		written = append_str_to_buf(fmt->buf, fmt->pos,
				"format not recognised\n", 4096);
	return (written);
}

int	process_format(char *set, int *i, t_fmt *fmt)
{
	int	written;

	if (set[*i] == '%' && set[*i + 1] == '%')
	{
		fmt->buf[fmt->pos] = set[*i + 1];
		written = 1;
	}
	else if (set[*i] == '%' && set[*i + 1] == 's')
		written = handle_format_string(fmt->buf, fmt->pos, fmt->list);
	else if (set[*i] == '%' || set[*i] == '\\')
	{
		written = process_special_char(set, *i, fmt);
		if (written > 1 || (set[*i + 1] && set[*i + 1] != set[*i]))
			(*i)++;
		return (written);
	}
	else
	{
		fmt->buf[fmt->pos] = set[*i];
		return (1);
	}
	if (written > 1 || (set[*i + 1] && set[*i + 1] != set[*i]))
		(*i)++;
	return (written);
}
