CC		=	gcc
RM		=	rm -f
LIBFT	=	srcs/libft
CFLAGS	=	-I./includes -g  -I../../../.brew/opt/readline/include  -I./ #-Wall -Wextra -Werror
RLFLAG	=	-lreadline
NAME	=	minishell
SRCS	=	minishell.c

#EXECUTOR
SRCS	+=	srcs/executor/executor.c \
			srcs/executor/builtins.c

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
			srcs/parser/env.c\

OBJS	=	$(patsubst %.c, %.o, $(SRCS))

all:
			$(MAKE) $(NAME)

$(NAME):	$(OBJS)
			$(MAKE) -C $(LIBFT)
			$(CC) -lreadline  -L ../../../.brew/opt/readline/lib  -o $(NAME) $(OBJS) $(LIBFT)/libft.a
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

# LIBFT = ./libft/libft.a

# CFLAGS = -Wall -Werror -Wextra -I../../.brew/opt/readline/include  -I./
# NAME = minishell srcs/parser/parse_dollar.c \
#             srcs/parser/process.c \
#             srcs/parser/parse_quotes.c \
#             srcs/parser/parse_utils.c \
# 			srcs/parser/parse_redir.c\
# 			srcs/parser/pre_parser.c\
# 			srcs/parser/env.c\

# # NAME_B = minishell_bonus

# SRC = minishell.c 

# CC = gcc

# LIB = ar -rcs $(NAME)

# OBJ = $(SRC:.c=.o)

# all: $(NAME)

# $(NAME): $(OBJ)
# 		$(MAKE) -C ./srcs/libft
# 		$(CC) -lreadline -L ../../.brew/opt/readline/lib  $(CFLAGS) -o $(NAME) $(OBJ) ./srcs/libft/libft.a

# #$(NAME1): $(OBJ1)
# #		$(MAKE) -C ./libft
# #		$(CC) $(CFLAGS) -o $(NAME_B) $(OBJ1) ./libft/libft.a

# %.o: %.c
# 		gcc -c $< -o $@

# clean:
# 		$(MAKE) clean -C ./libft
# 		rm -rf $(OBJ)

# fclean: clean
# 		$(MAKE) fclean -C ./libft
# 		rm -rf $(NAME)
# re: fclean all

# .PHONY:	all clean fclean re