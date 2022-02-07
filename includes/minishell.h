#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../srcs/libft/libft.h"
#include <fcntl.h>
#include <errno.h>
#include <readline/history.h>

/**********COLOR**********/
#define RESET	"\033[0m"
#define RED		"\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

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

typedef struct s_redir
{
    char *tmp1;
    char *tmp2;
    char *tmp3;
    char *tmp4;
}   t_redir;

typedef struct s_comm
{
    char **command_str; // двумернный массив из las_str(separator ' ')
    char *last_str; // подмножество строки 
    int infile;
    int outfile;
    int count_word;
    int count_node;
    char **envp;
    t_iter t; // структура итераторов
    struct  s_comm *next;

}   t_comm;

int pipex_alt(t_comm *lst, char **env);
int executor(t_comm * lst, char **env);

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
char *ft_add_space(char *str);
void ft_count_node2(char *str, int *i, char c);
t_comm *ft_check_redir(t_comm *lst);
int ft_check_str(char *str);

#endif