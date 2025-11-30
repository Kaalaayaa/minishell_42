# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kchatela <kchatela@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/13 15:09:03 by kchatela          #+#    #+#              #
#    Updated: 2025/11/19 17:36:26 by kchatela         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a  
SRC = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
 		ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c \
		ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c \
		ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
		ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_strcmp.c \
		ft_printf/ft_printf.c ft_printf/print_char.c ft_printf/print_low_hex.c ft_printf/print_pointer.c ft_printf/print_signed_int.c \
		ft_printf/print_string.c ft_printf/print_unsigned_int.c ft_printf/print_up_hex.c#Variable containing all .c files required to build the project
OBJS := $(SRC:%.c=%.o) #Converts all c files in SRC to object files. Using := ensures that this transformation happens once
CC = cc #compiler
CCFLAGS = -Wall -Werror -Wextra -fPIC #compiler flags
CPPFLAGS = # No need to add -I flag if libft.h is in the current directory


all: $(NAME) #Default target - builds the library

$(NAME) : $(OBJS)
	ar rcs $(NAME) $(OBJS)
	ranlib $(NAME)

# %.o rule will compile one .c file to its correspondig object (.o) file
# The %.o: %.c pattern specifies that in order to build something whose
# file name ends with .o, you need to have a file that has the same prefix
# but then ends with .c rather than .o.
# $<: The name of the first prerequisite.
# $@: This corresponds to the name of the target : what is on the left
#     side of the colon.
%.o: %.c
	$(CC) $(CPPFLAGS) $(CCFLAGS) -o $@ -c $<

clean: #Removes object files (*.o)
	rm -f $(OBJS)

fclean: clean #Removes both object files and the static library (libft.a)
	rm -f $(NAME)

re: fclean all #The re target is used to completely rebuild your project by first cleaning up all generated files (fclean), and then rebuilding everything (all)

.PHONY: all clean fclean re #declare that certain targets are not files
