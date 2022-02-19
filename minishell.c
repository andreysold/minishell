#include <sys/stat.h> //fixme delete
#include "minishell.h"

void	ft_free_list(t_comm *lst)
{
	t_comm *head;
	t_envp *head2;

	while (lst)
    {
        head = lst;
        if (lst->command_str)
        {
            int i = 0;
            while (lst->command_str[i])
                i++;
            ft_no_malloc(lst->command_str);
        }
        lst = lst->next;
        free (head);
    }
}


int ft_process4(char *str, t_envp *list_env)
{
    t_comm *lst;
	char **new_env;

    lst = malloc(sizeof(t_comm));
    if (!lst)
        return (-1);
    ft_memset((void *)lst, 0, sizeof(t_comm));
	new_env = ft_update_env(list_env);
   	lst = ft_parser4(lst, str, list_env);
	// if (executor(lst, new_env) == -1)
	//  	return (1);
	// printf("ABC\n");
	executor(lst, new_env);
	// ft_free_list(lst);
    return (0);
}

void	ft_no_malloc(char **str)
{
	int i;

	i = 0;
	while (str[i])
	{
		free (str[i]);
		i++;
	}
	free (str);
}

void	handler(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_redisplay();
	write(1,"  \b\b\n", 5);
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
	g_error_status = 1;
	// g_error = 1;
}


void	ft_up_shlvl(t_envp *list_env)
{
	t_envp *head;
	int shlvl;
	char *tmp;
	int locate;

	locate = 0;
	head = list_env;
	
	locate = locate_env_key(head, "SHLVL", 0);
	if (locate == -1)
		add_to_env(list_env, "SHLVL", "1", 0);
	else
	{	
		shlvl = ft_atoi(get_env_value(list_env, locate, 0)) + 1;
		tmp = ft_itoa(shlvl);
		upd_env_value(list_env, tmp, locate, 0);
	}
}
int main(int ac, char **av, char **env)
{
	char		*str;
	char		*name;
	char		**envp;
	t_envp		*list_env;

	list_env = malloc(sizeof(t_envp));
	if (!list_env)
		return (-1);
	envp = ft_get_envp(env);
	list_env = ft_node_env(list_env, envp);
	ft_up_shlvl(list_env);
//	signal(SIGQUIT, SIG_IGN);
	while (1)
    {
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, handler);
        str = readline("bash:");
        if (str && *str)
        {
           	add_history(str);
            if (ft_lexer(str) != -1)
            {
				if (ft_process4(str, list_env) == -1)
					exit(0);
            }
            else
                free (str);
        }
		else if (str == NULL)
			exit(0);

	}
	return (0);
}