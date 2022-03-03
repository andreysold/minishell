CC		=	gcc
RM		=	rm -f
LIBFT	=	srcs/libft
CFLAGS	=	-I./includes -ggdb3 -I../../../.brew/opt/readline/include -Wall -Wextra -Werror -Wunused
RLFLAG	=	-lreadline #-L../../../.brew/opt/readline/lib
NAME	=	minishell
SRCS	=	minishell.c

#EXECUTOR
SRCS	+=	srcs/executor/executor.c \

##BUILTINS
SRCS	+=	srcs/executor/builtins/builtins.c \
			srcs/executor/builtins/builtins_utils.c \
			srcs/executor/builtins/ft_echo.c \
			srcs/executor/builtins/ft_cd.c \
			srcs/executor/builtins/ft_pwd.c \
			srcs/executor/builtins/ft_export.c \
			srcs/executor/builtins/ft_unset.c \
			srcs/executor/builtins/ft_env.c \
			srcs/executor/builtins/ft_exit.c \

##PIPEX
SRCS	+=	srcs/executor/pipex/pipex.c \
			srcs/executor/pipex/heredoc.c \
			srcs/executor/pipex/utils.c \
			srcs/executor/pipex/utils1.c \

#UTILS
SRCS	+=	srcs/list_utils/list_utils.c\

#SIGNALS
SRCS	+=	srcs/signal.c \

#PARSER
SRCS	+=	srcs/parser/parse_dollar.c \
			srcs/parser/process.c \
			srcs/parser/parse_quotes.c \
			srcs/parser/parse_utils.c \
			srcs/parser/parse_redir.c\
			srcs/parser/pre_parser.c\
			srcs/parser/env.c\
			srcs/all_free.c\
			srcs/parser/env_utils.c\
			srcs/parser/dollar_utils.c\
			srcs/parser/process_utils.c\
			srcs/parser/redir_utils.c\
			srcs/parser/pre_parser_utils.c\

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
