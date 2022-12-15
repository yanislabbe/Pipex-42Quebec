# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ylabbe <ylabbe@student.42quebec.c>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/15 18:59:27 by ylabbe            #+#    #+#              #
#    Updated: 2022/11/15 18:59:30 by ylabbe           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ---------- get_next_line --------------------------------------------------- #
GNLSRC = ./get_next_line/get_next_line.c \
			./get_next_line/get_next_line_utils.c \
			main.c
GNLOBJ = $(GNLSRC:.c=.o)

# ---------- ft_printf ------------------------------------------------------- #
PRINTFSRC = @cd ft_printf && make
PRINTFOUT = ft_printf/libftprintf.a

# ---------- libft ----------------------------------------------------------- #
LIBFTSRC = @cd libft && make
LIBFTOUT = libft/libft.a

# ---------- commandes ------------------------------------------------------- #
COMP = @gcc
FLAGS = -Wall -Werror -Wextra -g
REMOVE = @rm -rfd
AR = @ar rcs
NAME = pipex

# ---------- règles ---------------------------------------------------------- #
all: $(NAME)
	@echo "\033[33;32mCompilation du projet terminé"

$(NAME): $(GNLOBJ) libftpeche ft_printfaubergine
	$(COMP) $(GNLOBJ) $(LIBFTOUT) $(PRINTFOUT) -o $(NAME)

libftpeche:
	$(LIBFTSRC)

ft_printfaubergine:
	$(PRINTFSRC)

%.o: %.c projets.h Makefile
	$(COMP) $(FLAGS) -c $< -o $@

clean:
	$(REMOVE) $(GNLOBJ) $(MAINOBJ)
	@cd libft && make clean
	@cd ft_printf && make clean
	@echo "\033[33;32mclean terminé"

fclean: clean
	$(REMOVE) $(NAME)
	@cd libft && make fclean
	@cd ft_printf && make fclean
	@echo "\033[33;32mfclean terminé"

re: fclean all
