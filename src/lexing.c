
#include "mini.h"

void	add_token(t_token **tokens, enum e_type type, char *s)
{
	t_token	*new;
	t_token	*tmp;

	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->token = ft_strdup(s);
	new->type = type;
	new->next = NULL;
	if (*tokens == NULL)
		*tokens = new;
	else
	{
		tmp = *tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

int	word_handle(const char *s, t_token **token)
{
	int		i;
	char	*d;
	char	qoute;

	i = 0;
	while (s[i] && !ft_isspace(s[i]) && !is_operator_start(s[i]))
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			qoute = s[i++];
			while (s[i] && s[i] != qoute)
				i++;
			if (s[i] == '\0')
				return (i); // incase of misaligned qoutes;
			i++;
		}
		else
			i++;
	}
	d = malloc(sizeof(char) * (i + 1));
	ft_strlcpy(d, s, i);
	add_token(token, WORD, d);
	return (i);
}
// when qoutes are present then it should ignore spaces

char	*adjust_input(char *i, t_token **tokens, enum e_type type, char *s)
{
	add_token(tokens, type, s);
	i += ft_strlen(s);
	return (i);
}

t_token	*lexer(char *input)
{
	t_token	*tokens;

	tokens = NULL;
	while (*input)
	{
		if (ft_isspace(*input))
			input++;
		else if (*input == '|')
			input = adjust_input(input, &tokens, PIPE, "|");
		else if (*input == '>' && *(input + 1) == '>')
			input = adjust_input(input, &tokens, DGREAT, ">>");
		else if (*input == '>')
			input = adjust_input(input, &tokens, GREAT, ">");
		else if (*input == '<' && *(input + 1) == '<')
			input = adjust_input(input, &tokens, DLESS, "<<");
		else if (*input == '<')
			input = adjust_input(input, &tokens, LESS, "<");
		else
			input += word_handle(input, &tokens);
	}
	return (tokens);
}

int	main(void)
{
	t_token *ret;
	char *line;

	while (1)
	{
		line = readline("minishell% ");
		add_history(line);
		ret = lexer(line);
		while (ret)
		{
			printf("%s : %d\n", ret->token, ret->type);
			ret = ret->next;
		}
		free(line);
	}
	return (0);
}
