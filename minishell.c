#include "includes/minishell.h"

int	ft_lexer(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			ft_count_node2(str, &i, '\'');
			if (str[i] != '\'')
				return (-1);
		}
		if (str[i] == '\"')
		{
			ft_count_node2(str, &i, '\"');
			if (str[i] != '\"')
				return (-1);
		}
		i++;
	}
	return (0);
}

void	ft_free_list(t_comm *lst)
{
	t_comm *head;

	while (lst->next != NULL)
    {
        head = lst;
        if (lst->command_str)
        {
            int i = 0;
            while (lst->command_str[i])
			{
				printf("|%s\n", lst->command_str[i]);
                free (lst->command_str[i]);
				i++;
			}
            free (lst->command_str);
        }
        lst = lst->next;
        free (head);
    }
}

int ft_process4(char **env, char *str)
{
	t_comm *lst;

	lst = malloc(sizeof(t_comm));
	if (!lst)
		return (-1);
	ft_memset((void *)lst, 0, sizeof(t_comm));
	lst = ft_parser4(lst, str, env);
	executor(lst, env);
//	write(2, "!!\n", 3);
	ft_free_list(lst);
	return (0);
}

void ft_no_malloc(char **str)
{
	int i =0;
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

int main(int ac, char **av, char **env)
{
	char *str;
	char **envp;

	(void)ac;
	(void)av;
	while (1)
	{
		str = readline("bash:");
		if (str && *str)
		{
			///parser
			envp = ft_get_envp(env);
			add_history(str);
			if (ft_process4(envp, str) == -1)
				exit (0);
			///executor

			free (str);
			ft_no_malloc(envp);
		}
		else
			exit(0);
	}
}