#include "minishell.h"

int	ft_lexer(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
				i++;
			if (str[i] != '\'')
				return (-1);
		}
		if (str[i] == '\"')
		{
			i++;
			while (str[i] && str[i] != '\"')
				i++;
			if (str[i] != '\"')
				return (-1);
		}
		i++;
	}
	return (0);
}

int ft_process4(char **env, char *str)
{
    t_comm *lst;
    t_comm *head;

    lst = malloc(sizeof(t_comm));
    if (!lst)
        return (-1);
    ft_memset((void *)lst, 0, sizeof(t_comm));
	lst = NULL;
    lst = ft_parser4(lst, str, env);
	int i = 0;

	while (lst)
	{
		if (lst->command_str)
		{
			i = 0;
			while (lst->command_str[i])
				printf("%s\n", lst->command_str[i++]);
		}
		lst = lst->next;
	}
	printf("%d\n", i);
    // while (lst != NULL)
    // {
    //     head = lst;
    //     if (lst->command_str)
    //     {
    //         int i = 0;
    //         while (lst->command_str[i])
    //         {
    //             free (lst->command_str[i]);
    //             i++;
    //         }
    //         free (lst->command_str);
    //     }
    //     // if (lst->last_str)
    //     //     free (lst->last_str);
    //     lst = lst->next;
    //     free (head);
    // }
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

int main(int ac, char **av, char **env)
{
	char *str;

	(void)ac;
	(void)av;
	// while (1)
	// {
		str = readline("bash:");
		if (str && *str)
		{
			add_history(str);
			if (ft_process4(env, str) == -1)
				exit (0);
			free (str);
		}
	// }
}