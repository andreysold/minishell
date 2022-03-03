CC		=	gcc
RM		=	rm -f
LIBFT	=	srcs/libft
CFLAGS	=	-I./includes -ggdb3 -I../../8.1.2/include/readline/include -Wall -Wextra -Werror
RLFLAG	=	-lreadline -L../../8.1.2/include/readline/lib
NAME	=	minishell
B_NAME	=	minishell_bonus
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
SRCS	+=	srcs/signal.c

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

B_OBJS	=	$(patsubst %.c, %.o, $(SRCS))

all:
			@$(MAKE) -C $(LIBFT)
			$(MAKE) $(NAME)

$(NAME):	$(OBJS) $(LIBFT)/libft.h $(LIBFT)/libft.a
			@$(CC) -o $(NAME) $(RLFLAG) $(OBJS) $(LIBFT)/libft.a
			@echo "minishell is ready to use ✅ "

bonus:
			@$(MAKE) -C $(LIBFT)
			@$(MAKE) $(B_NAME)

$(B_NAME):	$(B_OBJS) $(LIBFT)/libft.h $(LIBFT)/libft.a
			@$(MAKE) -C $(LIBFT)
			@$(CC) -o $(B_NAME) $(RLFLAG) $(B_OBJS) $(LIBFT)/libft.a
			@echo "minishell_bonus is ready to use ✅ "

%.o: %.c	includes/minishell.h Makefile
			@$(CC) $(CFLAGS) -c $< -o $@

clean:
			@$(MAKE) -C $(LIBFT)/ clean
			@$(RM)	$(OBJS) $(B_OBJS)
			@echo ".o Deleted 🗿"

fclean: 	clean
			@$(MAKE) -C $(LIBFT)/ fclean
			@$(RM)	 $(NAME) $(B_NAME)
			@echo "Deleted 😬"

re:			fclean all

run:		all
			./$(NAME)

.PHONY: all bonus clean fclean re run
