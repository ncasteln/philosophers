# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/08 08:58:13 by ncasteln          #+#    #+#              #
#    Updated: 2023/10/12 15:18:28 by ncasteln         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CFLAGS = -g -pthread -Wall -Wextra -Werror
FSANITIZE = -fsanitize=thread

# ------------------------------------------------------------------------- SRC
SRC = main.c \
	validation.c \
	threads.c \
	mutex.c \
	philo_routine.c \
	meal_monitor.c \
	death_monitor.c \
	time.c \
	create_philo.c \
	create_monastery.c \
	utils.c \
	print_utils.c \
	heap_utils.c \
	error.c
VPATH = ./src/
OBJS_DIR = ./objs/
OBJS = $(addprefix $(OBJS_DIR), $(SRC:.c=.o))
HEADER = ./include/philo.h

# ----------------------------------------------------------------------- RULES
all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(NAME) successfully compiled."

$(OBJS_DIR)%.o: %.c $(HEADER)
	@mkdir -p $(OBJS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ -I./include/

fsanitize: $(OBJS)
	@$(CC) $(CFLAGS) $(FSANITIZE) $(OBJS) -o $(NAME)

clean:
	@rm -rf $(OBJS_DIR)
	@echo "objs cleaned."

fclean: clean
	@rm -rf $(NAME)
	@echo "$(NAME) removed."

re: fclean all

.PHONY: all clean fclean re bonus sanitize
