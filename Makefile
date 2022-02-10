#
# Created by denser on 24.01.2022.
#

NAME	= tcp_echo_srv
CC		= gcc
CFLAGS	= -Wall -Wextra -Werror

SRCDIR  = ./src/
INCDIR  = ./inc/
OBJDIR  = ./obj/

INCL	= $(wildcard $(INCDIR)*.h)

SRC		= main.c request.c respond.c server.c signal_catch.c \
		socket_info.c itoa.c clients.c

SRCS	= $(addprefix $(SRCDIR), $(SRC))
OBJ	=	$(addprefix $(OBJDIR), $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ) $(SRCS) $(INCL)
	$(CC) $(OBJ) -o $(NAME)

$(OBJDIR)%.o: $(SRCDIR)%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I $(INCDIR) -o $@ -c $<

$(OBJDIR):
		mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean count