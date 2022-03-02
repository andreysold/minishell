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

# include <sys/fcntl.h>			//open
# include <sys/wait.h>			//waitpid
# include <stdio.h>				//perror
# include <sys/errno.h>

# include <string.h> //strerror
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

# define FD_UNUSED	-2

/**********COLOR**********/
#define RESET	"\033[0m"
#define RED		"\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
/***********PRE_PARSER****/

#define	PIPE "bash: syntax error near unexpected token `|'\n"
#define RED1 "bash: syntax error near unexpected token `>'\n"
#define RED2 "bash: syntax error near unexpected token `>>'\n"
#define RED3 "bash: syntax error near unexpected token `<'\n"
#define RED4 "bash: syntax error near unexpected token `<<'\n"
#define	RED5 "bash: syntax error near unexpected token `<<<'\n"
#define	QUOT "bash: syntax error in unclosed quoters\n"

# define START	0
# define MIDDLE	1
# define END	2

int	g_error_status;

typedef struct s_iterat
{
	int	z;
	int	k;
	int	c;
	int	l;
	int	pos;
	int	foq;
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
	char			**cmd;
	char			*last_str;
	int				infile;
	int				outfile;
	char			**str_tl;
	char			*here;
	int				count_word;
	int				count_node;
	char			*tmp;
	int				fl;
	int				flag_error;
	t_envp			*e;
	char			*name;
	t_iter			t;
	struct s_comm	*next;
}	t_comm;

/**********EXECUTOR**********/
int		executor(t_comm **lst, char **env);

/**********PIPEX**********/
void	heredoc(t_comm **tmp);
void	wait_childs(int n);
void	redirect(t_comm *tmp);
int		*open_pipes(t_comm *tmp);
void	pipe_switch(int i, int kind, int *pipes, t_comm *tmp);

/**********UTILS***********/
char	*find_command_path(char *command, char **env);
void	pipex_iterators(t_comm **tmp, int *kind, int *i);
void	bash_error(char *first_part, char *cmd, char *last_part);
void	close_in_out_file(t_comm *tmp);
void	close_pipes(int *pipes, int count_node);

/**********BUILTIN**********/
int		builtins(t_comm **lst, char **env);
int		check_builtin(t_comm **lst, char **env);
int		ft_echo(t_comm *tmp);
int		ft_cd(t_comm *lst);
int		ft_pwd(t_comm *lst);
int		ft_export(t_comm *lst);
int		ft_unset(t_comm **lst);
int		ft_env(t_comm *lst);
int		ft_exit(t_comm *lst);

/**********BUILTIN_UTILS****/
int		locate_env_key(t_envp *envp, char *key, int origin);
char	*get_env_value(t_envp *envp, int location, int origin);

//fixme lower t_envp **envp
void	upd_env_value(t_envp *envp, char *new_value, int location, int origin);
void	add_to_env(t_envp *envp, char *new_key, char *new_value, int origin);

/**********LIST_UTILS*******/
void	remove_element(t_envp **head, int location);
size_t	ft_listlen(t_envp *head);

/**********SIGNALS*******/
void	handler22(int sig);
void	ff(int sig);

char	*ft_global_value(char *str, t_comm *lst, int *i, int *j);

int		ft_lexer(char *str);
int		ft_process4(char *str, t_envp **envp);
int		ft_count_node(char *str);

void	ft_no_malloc(char **str);
void	ft_skip_space(char *str, int *i);

int		ft_lexer(char *str);
char	*ft_one_quotes(char *str, t_comm *lst, int *i, int *j);
char	*ft_two_quotes(char *str, t_comm *lst, int *i, int *j);
char	*ft_tream(char *str);
char	**ft_return_space(char **str);
int		ft_dol_str(char *str, t_comm *lst);
t_comm	*ft_parser4(t_comm *lst, char *str, t_envp *e);
t_comm	*ft_return_node(t_comm *lst);
char	*ft_destroy_space4(char *str, t_comm *lst);
char	*ft_shit_dollar(char *str, t_comm *lst, int *i, int *j);
char	*ft_add_space(char *str);
void	ft_count_node2(char *str, int *i, char c);
t_comm	*ft_check_redir(t_comm *lst);
int		ft_check_str(char *str);
char	*ft_new_sub(int i, t_comm *lst, char *str, int begin);
void	ft_skip_sp(char *str, int *i, int *begin);
void	ft_skip_space(char *str, int *i);
char	*ft_open_file(char *str, int *i, int *j, t_comm *lst);
int		ft_cnode(t_envp *list_env);
t_envp	*ft_node_env(t_envp *e, char **env);
char	*ft_get_value(int count, char **env);
char	*ft_get_key(int count, char **env);
char	**ft_update_env(t_envp *list_env);
char	**ft_get_envp(char **env);
char	*ft_spasibo_norma(char *tmp, t_comm *lst, int *j);
char	*ft_global_value(char *str, t_comm *lst, int *i, int *j);
char	*ft_name_file(t_comm *lst, char *tmp, t_envp *head);
char	*ft_key_file(char *str, int *begin, int len, char *name);
char	*ft_new_str(char *str);
void	handler(int sig);
void	remove_all_list(t_comm *head);
void	ft_skip_quotes(char *str, int *i, char c);
void	remove_all_env_list(t_envp *head);
void	clean_env(char **env, t_comm *lst);
void	ft_get_key_value(char **massiv, t_envp *tmp, char *clean, int *i);
int		ft_is_space(char *str, int *i);
int		ft_return_error(char *str);
int		ft_len_value_dol(char *tmp, t_comm *lst);
int		ft_count_dol_value(char *str, int *i);
int		ft_iskey(char c);
int		ft_del_space(char *str, int *i);
int		ft_count_strings(t_comm *lst);
int		ft_count_redir(char *str, int *i, char c);
int		ft_return_error(char *str);
int		ft_out_error(char *str, int *i, int count_red);
int		ft_infile_error(char *str, int *i, int count_back);
size_t	segments(char const *s, char c);
#endif
