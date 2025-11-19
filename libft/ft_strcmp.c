#include "libft.h"

int ft_strcmp(const char *s1, const char *s2)
{
    size_t i = 0;
    if (!s1 || !s2)
        return (s1 - s2);
    while (s1[i] && s2[i])
    {
        if (s1[i] != s2[i])
            return (s1[i] - s2[i]);
        i++;
    }
    return (s1[i] - s2[i]);
}