#include "mini.h"

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

char	*ft_strdup(const char *s)
{
	char *str;
	int i;
	int j;

	i = 0;
	while (s[i])
		i++;
	str = (char *)malloc((i + 1) * sizeof(char));
	if (str == NULL)
	{
		return (NULL);
	}
	j = 0;
	while (j <= i)
	{
		str[j] = ((char *)s)[j];
		j++;
	}
	str[i] = '\0';
	return (str);
}

int	is_operator_start(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	unsigned int	i;

	i = 0;
	if (size == 0)
		return (ft_strlen((char *)src));
	while (src[i] && i < size)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	while (src[i])
		i++;
	return (i);
}