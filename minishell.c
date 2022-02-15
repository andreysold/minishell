#include <sys/stat.h> //fixme delete
#include "minishell.h"


void	ft_free_list(t_comm *lst)
{
	t_comm *head;
	t_envp *head2;

	while (lst->e)
	{
		// printf("")
		head2 = lst->e;
		if (lst->e->key)
			free (lst->e->key);
		if (lst->e->value)
			free (lst->e->value);
		lst->e = lst->e->next;
		free (head2);
	}
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

char *ft_get_key(int count, char **env)
{
	char *str;
	int i;

	i = 0;
	while (env[i])
	{
		if (i == count)
		{
			int z = 0;
			while (env[i][z] && env[i][z] != '=')
				z++;
			str = ft_substr(env[i], 0, z);
			break ;
		}
		i++;
	}
	return (str);
}

char *ft_get_value(int count, char **env)
{
	char *str;
	int i;

	i = 0;
	while (env[i])
	{
		if (i == count)
		{
			int z = 0;
			while (env[i][z] && env[i][z] != '=')
				z++;
			str = ft_substr(env[i], z + 1, ft_strlen(env[i]) - z);
			break ;
		}
		i++;
	}
	return (str);
}

t_envp *ft_node_env(t_envp *e, char **env)
{
	int i;
	int count;
	t_envp *tmp;

	count = 0;
	i = 0;
	while (env[count])
		count++;
	e = NULL;
	while (count-- > 0)
	{
		tmp = malloc(sizeof(t_envp));
		if (!tmp)
			return (NULL);
		tmp->key = ft_get_key(count, env);
		tmp->value = ft_get_value(count, env);
		tmp->next = e;
		e = tmp;
	}
	return (e);
}

int ft_process4(char **env, char *str)
{
    t_comm *lst;
	t_envp *envp;

	envp = malloc(sizeof(t_envp));
	if (!envp)
		return (-1);
    lst = malloc(sizeof(t_comm));
    if (!lst)
        return (-1);
	envp = ft_node_env(envp, env);
    ft_memset((void *)lst, 0, sizeof(t_comm));
   	lst = ft_parser4(lst, str, envp);
	// if (executor(lst, env) == -1)
	//  	return (1);
	//executor(lst, env);
	ft_free_list(lst);
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

char **ft_get_envp(char **env)
{
	char **envp;
	int i = 0;

	while (env[i])
		i++;
	envp = (char **)malloc(sizeof(char *) * i + 1);
	if (!envp)
		return (NULL);
	i = 0;
	while (env[i])
	{
		envp[i] = ft_substr(env[i], 0, ft_strlen(env[i]));
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

int ft_check_str(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '>' && !(ft_isdigit(str[i + 1])))
        {
            i += 2;
            if (str[i] == '>')
                return (-1);
        }
        i++;
    }
    return (1);
}

int main(int ac, char **av, char **env)
{
	char		*str;
	char		*name;
	char		**envp;
	extern char	**environ;

	(void)ac;
	(void)av;
	while (1)
    {
        str = readline("bash:");
        if (str && *str)
        {
            envp = ft_get_envp(env);
            add_history(str);
            if (ft_lexer(str) != -1)
            {
                if (ft_process4(envp, str) == -1)
                    exit(0);
            }
            else
                free (str);
            ft_no_malloc(envp);
        }
	 }
}