/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galetha <galetha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 20:40:14 by wjonatho          #+#    #+#             */
/*   Updated: 2022/02/22 14:15:18 by galetha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include "../srcs/libft/libft.h"
# include <fcntl.h>
# include <errno.h>
 # include <readline/readline.h>
 # include <readline/history.h>
//#include "../../../home/linuxbrew/.linuxbrew/Cellar/readline/8.1.2/include/readline/readline.h"

//#include "../../../home/linuxbrew/.linuxbrew/Cellar/readline/8.1.2/include/readline/history.h"
//#include "/Users/galetha/.brew/Cellar/readline/8.1.2/include/readline/readline.h"
//#include "/Users/galetha/.brew/Cellar/readline/8.1.2/include/readline/history.h"
#include "pipex.h"

# define FD_UNUSED	-2

/**********COLOR**********/
# define RESET	"\033[0m"
# define RED	"\033[31m"
# define GREEN	"\033[32m"
# define YELLOW	"\033[33m"
# define BLUE	"\033[34m"

int g_error_status;

typedef struct s_iterat
{
	int z;
	int k;
	int c;
	int l;
	int pos;
	int foq;
}	t_iter;

typedef struct s_redir
{
	char	*tmp1;
	char	*tmp2;
	char	*tmp3;
	char	*tmp4;
}	t_redir;

typedef struct s_envp
{
	char			*key;
	char			*value;
	char			*key_orig;
	char			*value_orig;
	int				count;
	struct s_envp	*next;
}	t_envp;

typedef struct s_comm
{
	char			**cmd; // двумернный массив из las_str(separator ' ') //malloc
	char			*last_str; // подмножество строки
	int				infile;
	int				outfile;
	char			**str_tl;
	char			*here;
	int				count_word;
	int				count_node;
	char			*tmp;
	int				fl;
	t_envp			*e;
	char			*name; // name of files
	t_iter			t; // структура итераторов
	struct s_comm	*next;
}	t_comm;

int		pipex(t_comm *lst, char **env);
int		executor(t_comm *lst, char **env);
void	bash_error(char *first_part, char *cmd, char *last_part);

/**********BUILTIN**********/
int		builtins(t_comm *lst, char **env);
int		check_builtin(t_comm *lst, char **env);
int		ft_echo(t_comm *tmp);
int		ft_cd(t_comm *lst);
int		ft_pwd(t_comm *lst);
int		ft_export(t_comm *lst);
int		ft_exit(t_comm *lst);

/**********BUILTIN_UTILS****/
char	*get_env_value(t_envp *envp, int location, int origin);
void	upd_env_value(t_envp *envp, char *new_value, int location, int origin);
void	add_to_env(t_envp *envp, char *new_key, char *new_value, int origin);
int		locate_env_key(t_envp *envp, char *key, int origin);

char	*ft_global_value(char *str, t_comm *lst, int *i, int *j);

int		ft_lexer(char *str);
int		ft_process4(char *str, t_envp *envp);
int		ft_count_node(char *str);

void ft_no_malloc(char **str);
int	ft_lexer(char *str);
char *ft_one_quotes(char *str, t_comm *lst, int *i, int *j);
char *ft_two_quotes(char *str, t_comm *lst, int *i, int *j);
char *ft_tream(char *str);
char **ft_return_space(char **str);
int ft_dol_str(char *str, t_comm *lst);
t_comm *ft_parser4(t_comm *lst, char *str,  t_envp *e);
char *ft_destroy_space4(char *str, t_comm *lst);
char *ft_shit_dollar(char *str, t_comm *lst, int *i, int *j);
char *ft_add_space(char *str);
void ft_count_node2(char *str, int *i, char c);
t_comm *ft_check_redir(t_comm *lst);
int ft_check_str(char *str);
char *ft_new_sub(int i, t_comm *lst, char *str, int begin);
void    ft_skip_sp(char *str, int *i, int *begin);
void	ft_skip_space(char *str, int *i);
char *ft_open_file(char *str, int *i, int *j, t_comm *lst);
int	ft_cnode(t_envp *list_env);
t_envp *ft_node_env(t_envp *e, char **env);
char *ft_get_value(int count, char **env);
char *ft_get_key(int count, char **env);
char **ft_update_env(t_envp *list_env);
char **ft_get_envp(char **env);
void	handler(int sig);


#endif
