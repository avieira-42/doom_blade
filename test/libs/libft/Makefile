# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: avieira- <avieira-@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/21 12:18:39 by avieira-          #+#    #+#              #
#    Updated: 2025/11/27 05:49:27 by avieira-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS = -Wall -Wextra -Werror
NAME = libft.a

SRCS = srcs/ft_atoi.c srcs/ft_bzero.c srcs/ft_isalnum.c srcs/ft_isalpha.c
SRCS += srcs/ft_isascii.c srcs/ft_isdigit.c srcs/ft_isspace.c
SRCS += srcs/ft_isprint.c srcs/ft_memcpy.c srcs/ft_memset.c srcs/ft_strlen.c
SRCS += srcs/ft_tolower.c srcs/ft_memmove.c srcs/ft_strnstr.c srcs/ft_strchr.c
SRCS += srcs/ft_strrchr.c srcs/ft_strtrim.c srcs/ft_split.c srcs/ft_putendl_fd.c
SRCS += srcs/ft_strlcat.c srcs/ft_itoa.c srcs/ft_strdup.c srcs/ft_strncmp.c
SRCS += srcs/ft_memcmp.c srcs/ft_calloc.c srcs/ft_substr.c srcs/ft_strjoin.c
SRCS += srcs/ft_strmapi.c srcs/ft_striteri.c srcs/ft_putchar_fd.c 
SRCS += srcs/ft_putstr_fd.c srcs/ft_toupper.c srcs/ft_strlcpy.c srcs/ft_memchr.c
SRCS += srcs/ft_putnbr_fd.c srcs/ft_printf.c srcs/ft_printadd.c
SRCS += srcs/ft_printchar.c srcs/ft_printhex.c srcs/ft_printnbr.c
SRCS += srcs/ft_printunsign_nbr.c srcs/ft_printstr.c srcs/ft_issign.c
SRCS += srcs/ft_free_matrix.c srcs/i_merge_sort.c srcs/ft_atol.c
SRCS += srcs/ft_lstnew_bonus.c srcs/ft_lstdelone_bonus.c srcs/ft_strcmp.c
SRCS += srcs/ft_lstsize_bonus.c srcs/ft_lstlast_bonus.c srcs/ft_dblylst_clear.c
SRCS += srcs/ft_lstadd_back_bonus.c srcs/ft_lstadd_front_bonus.c
SRCS += srcs/ft_lstclear_bonus.c srcs/ft_lstiter_bonus.c srcs/ft_lstmap_bonus.c
SRCS += srcs/ft_dblylst_new.c srcs/ft_dblylst_addback.c srcs/ft_dblylst_last.c 
SRCS += srcs/ft_dblylst_size.c srcs/ft_is_int.c
SRCS += srcs/get_next_line.c srcs/get_next_line_utils.c

OBJS = $(SRCS:.c=.o)

# ================== PROGRESS BAR ================== #

TOTAL := $(words $(SRCS))
BAR_LENGTH = 30
COUNTER = 0

define progress_bar
	$(eval COUNTER=$(shell echo $$(($(COUNTER)+1))))
	@percent=$$((100 * $(COUNTER) / $(TOTAL))); \
	filled=$$(( $(BAR_LENGTH) * $(COUNTER) / $(TOTAL) )); \
	empty=$$(( $(BAR_LENGTH) - $$filled )); \
	bar="$$(printf '%0.s#' $$(seq 1 $$filled))$$(printf '%0.s-' $$(seq 1 $$empty))"; \
	printf "\rBuilding libft binaries [%s] %3d%%" "$$bar" $$percent;
endef

# ================== RULES ================== #

all: $(NAME)
	@echo "\n\033[32mlibft build complete!\033[0m"

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)

%.o: %.c
	@$(call progress_bar)
	@cc $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
