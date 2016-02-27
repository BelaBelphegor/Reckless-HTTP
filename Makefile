NAME		= ./reckless
SRCS		= main.c \
			  server.c \
			  http_request.c \
			  logger.c
SRCSPATH	= ./srcs/
INCLUDES	= ./includes/
CC			= clang
CFLAGS		= -Wall -Werror -Wextra -O2
INCLUDES_O	= -I $(INCLUDES)
# if external lib is needed INCLUDES_C	=
SRC			= $(addprefix $(SRCSPATH), $(SRCS))
OBJS		= $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(CFLAGS)

%.o: %.c
	$(CC) -o $@ $(INCLUDES_O) -c $<

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re
