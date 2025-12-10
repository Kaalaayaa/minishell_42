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

static int	handle_format_percent(char *buf, int pos, char next)
{
	if (next == '%')
	{
		buf[pos] = next;
		return (1);
	}
	return (0);
}

static int	handle_format_string(char *buf, int pos, va_list *list)
{
	return (append_str_to_buf(buf, pos, va_arg(*list, char *), 4096));
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

static int	process_special_char(char *set, int i, char *buf,
			int pos, va_list *list)
{
	int	written;

	written = 0;
	if (set[i + 1] && set[i + 1] == '%')
		written = handle_format_percent(buf, pos, set[i + 1]);
	else if (set[i + 1] && set[i + 1] == 's')
		written = handle_format_string(buf, pos, list);
	else if (set[i] == '\\' && set[i + 1])
		written = handle_escape(buf, pos, set[i + 1]);
	else if (set[i + 1])
		written = handle_format_quote(buf, pos, set[i + 1]);
	else if (!set[i + 1])
	{
		buf[pos] = set[i];
		written = 1;
	}
	else
		written = append_str_to_buf(buf, pos, "format not recognised\n", 4096);
	return (written);
}

static int	process_format(char *set, int *i, char *buf,
			int pos, va_list *list)
{
	int	written;

	if (set[*i] == '%' || set[*i] == '\\')
	{
		written = process_special_char(set, *i, buf, pos, list);
		if (written > 1 || (set[*i + 1] && set[*i + 1] != set[*i]))
			(*i)++;
		return (written);
	}
	else
	{
		buf[pos] = set[*i];
		return (1);
	}
}

void	print_error(int fd, char *set, ...)
{
	char	buffer[4096];
	va_list	list;
	int		i;
	int		buf_pos;
	int		written;

	i = 0;
	buf_pos = 0;
	va_start(list, set);
	while (set[i] && buf_pos < 4095)
	{
		written = process_format(set, &i, buffer, buf_pos, &list);
		if (written == -1)
			break ;
		buf_pos += written;
		i++;
	}
	va_end(list);
	if (buf_pos > 0)
		write(fd, buffer, buf_pos);
}