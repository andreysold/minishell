#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "pipex.h"
//#include "../srcs/libft/libft.h"

/**********COLOR**********/
#define RESET	"\033[0m"
#define RED		"\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

typedef struct s_iterat
{
    int	i;
    int	j;
    int	k;
    int	z;
    int	l;
    int	pos;
    int	foq; // flag on quotes
}   t_iter;

typedef struct s_comm
{
    char	**command_str; // двумернный массив из las_str(separator ' ')
    char	*last_str; // подмножество строки
    char	*outfile;
    char	*mysor;
    char	**mysor2;
    char	*infile;
    char	*append;
    char	*herdok;
    int		count_word;
	int		count_node;
    t_iter	t;
    struct	s_comm *next;
}			t_comm;

/**********EXECUTOR**********/
int		pipex(t_comm *lst, char **env);
int		pipex_alt(t_comm *lst, char **env);
int		executor(t_comm *lst, char **env);

int		ft_lexer(char *str);
int		ft_process4(char **env, char *str);
int		ft_count_node(char *str);

void	ft_no_malloc(char **str);

char	*ft_one_quotes(char *str, char *tmp, int *i, int *j);
char	*ft_two_quotes(char *str, char **env,char *tmp, int *i, int *j);
char	*ft_tream(char *str);
char	**ft_return_space(char **str);
int		ft_dol_str(char *str, char **env);
t_comm	*ft_parser4(t_comm *lst, char *str, char **env);
char	*ft_destroy_space4(char *str, char **env);
char	*ft_shit_dollar(char *str, char **env, char *m_tmp, int *i, int *j);
void	ft_count_node2(char *str, int *i, char c);

#endif