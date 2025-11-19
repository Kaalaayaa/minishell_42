NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iincludes -g 

# Source files
SRCS = src/main.c  src/lexer/lexing.c src/lexer/lexer_utils.c \
	   src/parser/parsing.c src/parser/parsing_utils.c \
	   src/builtins/builtins.c src/builtins/cd.c src/builtins/echo.c src/builtins/utils.c \
	   src/builtins/env.c src/builtins/exit.c src/builtins/export.c src/builtins/export_utils.c \
	   src/builtins/pwd.c src/builtins/unset.c \
	   src/env/env.c src/env/update_shlvl.c \
	   src/signals/signal.c src/signals/signal_heredoc.c src/utils/shell_init.c src/redirection/redirection_2.c\
	   src/utils/utils.c src/execution/exec.c src/execution/exec_utils.c src/redirection/redirection.c \
	   src/redirection/redirection_utils.c src/expander/expander.c src/expander/expander_utils.c \
	   src/expander/expander_2.c src/parser/syntax.c src/cleanup/cleanup.c


# Object files
OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

# Build libft
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Compile and link
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -L$(LIBFT_DIR) -lft -lreadline -o $@

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rules
clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
