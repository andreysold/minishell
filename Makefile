CC		=	gcc
RM		=	rm -f
LIBFT	=	srcs/libft
CFLAGS	=	-I./includes -ggdb3 #-Wall -Wextra -Werror
RLFLAG	=	-lreadline
NAME	=	minishell
SRCS	=	minishell.c

#EXECUTOR
SRCS	+=	srcs/executor/executor.c \
			srcs/executor/builtins.c \
			srcs/executor/builtins_utils.c

##PIPEX
SRCS	+=	srcs/executor/pipex/pipex.c \
            srcs/executor/pipex/utils.c

#PARSER
SRCS	+=	srcs/parser/parse_dollar.c \
            srcs/parser/process.c \
            srcs/parser/parse_quotes.c \
            srcs/parser/parse_utils.c \
			srcs/parser/parse_redir.c\
			srcs/parser/pre_parser.c\

OBJS	=	$(patsubst %.c, %.o, $(SRCS))

all:
			$(MAKE) $(NAME)

$(NAME):	$(OBJS)
			$(MAKE) -C $(LIBFT)
			$(CC) -o $(NAME) $(RLFLAG) $(OBJS) $(LIBFT)/libft.a
			@echo "minishell is ready to use ✅ "

%.o: %.c	includes/minishell.h includes/pipex.h
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

run:		all
			./$(NAME)

.PHONY: all clean fclean re run