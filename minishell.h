#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "libft/libft.h"
#include <fcntl.h>
#include <errno.h>
#include <readline/history.h>

typedef struct s_iterat // итераторы для подгонки под норму
{
    int i;
    int j;
    int k;
    int z;
    int l;
    int pos;
    int foq; // flag on quotes
}   t_iter;


typedef struct s_redirect {
    int *fd; // фдшники файлов
    int *fd2; // для фдшников функций
}   t_redir;

typedef struct s_comm
{
    char **command_str; // двумернный массив из las_str(separator ' ')
    char *last_str; // подмножество строки 
    char *outfile; // возможно не понадобится
    char *infile; // возможно не понадобится 
    char *append;
    char *herdok;
    int count_word;
    t_iter t; // структура итераторов
    struct  s_comm *next;
    t_redir *rd; // структура под редиректы.
}   t_comm;


int	ft_lexer(char *str);
int ft_process4(char **env, char *str);
int ft_count_node(char *str);

void ft_no_malloc(char **str);

char *ft_one_quotes(char *str, char *tmp, int *i, int *j);
char *ft_two_quotes(char *str, char **env,char *tmp, int *i, int *j);
char *ft_tream(char *str);
char **ft_return_space(char **str);
int ft_dol_str(char *str, char **env);
t_comm *ft_parser4(t_comm *lst, char *str, char **env);
char *ft_destroy_space4(char *str, char **env);
char *ft_shit_dollar(char *str, char **env, char *m_tmp, int *i, int *j);
void ft_count_node2(char *str, int *i, char c);
#endif