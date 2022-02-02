CC	=	gcc
RM	=	rm -f
LIBFT	=	./libft
CFLAGS	=	-I./includes #-Wall -Wextra -Werror
RLFLAG	=	-lreadline
NAME	=	minishell
SRCS	=   minishell.c \
            parse_dollar.c \
            parse_quotes.c \
            parse_utils.c \
            process.c

OBJS	=	$(patsubst %.c, %.o, $(SRCS))

all:
			$(MAKE) -C $(LIBFT)
			$(MAKE) $(NAME)

$(NAME):	$(OBJS)
			$(CC) -o $(NAME) $(RLFLAG) $(OBJS) $(LIBFT)/libft.a
			@echo "minishell is ready to use ✅ "

%.o: %.c	includes/minishell.h
			$(CC) $(CFLAGS) -c $< -o $@

clean:
			@$(MAKE) -C $(LIBFT)/ clean
			@$(RM)	$(OBJS)
			@echo ".o Deleted 🗿"

fclean: 	clean
			@$(MAKE) -C $(LIBFT)/ fclean
			@$(RM)	 $(NAME)
			@echo "Deleted 😬"

re:			fclean all

.PHONY: all clean fclean re