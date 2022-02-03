#include "minishell.h"

int	ft_lexer(char *str)
{
	int i;
	int len;

	i = 0;
	len = ft_strlen(str);
	while (str[i])
	{
		if (str[i] == '\'')
		{
			ft_count_node2(str, &i, '\'');
			if (str[i] != '\'')
			{
				write(1, "bash: syntax error in unclosed quoters\n", 40);
				return (-1);
			}
		}
		if (str[i] == '\"')
		{
			ft_count_node2(str, &i, '\"');
			if (str[i] != '\"')
			{
				write(1, "bash: syntax error in unclosed quoters\n", 40);
				return (-1);
			}
		}
		if (str[i] == '|')
		{
			i++;
			while (str[i] && str[i] == ' ')
				i++;
			if (i == len)
			{
				write(1, "bash: syntax error in the absence of commands\n", 47);
				return (-1);
			}
		}
		if (str[i] == '>' || str[i] == '<')
		{
			i++;
			while (str[i] && str[i] == ' ')
				i++;
			if (i == len)
			{
				write(1,"bash: syntax error near unexpected token `newline'\n", 52);
				return (-1);
			}
		}
		i++;
	}
	//printf("A\n");
	return (1);
}

void	ft_free_list(t_comm *lst)
{
	t_comm *head;

	while (lst != NULL)
    {
        head = lst;
        if (lst->command_str)
        {
            int i = 0;
            while (lst->command_str[i])
			{
				printf("|%s|\n", lst->command_str[i]);
                free (lst->command_str[i]);
				i++;
			}
            free (lst->command_str);
        }
        lst = lst->next;
        free (head);
    }
}

t_comm *ft_check_redir(t_comm *lst)
{
	t_comm *head;
	int j;

	j = 0;
	head = lst;
	char *red;
	lst->rd = malloc(sizeof(t_redir));
	if (!lst->rd)
		return (NULL);
	lst->rd->fd = (int *)malloc(sizeof(int) * 4);
	if (!lst->rd->fd)
		return (NULL);
	red = ft_strdup(">");
	while (lst->next != NULL)
	{
		// if (lst->command_str)
		// {
		// 	int i = 0;
		// 	while (lst->command_str[i])
		// 	{
		// 		if (ft_strncmp(lst->command_str[i], red, ft_strlen(red))== 0)
		// 		{
		// 			if (i == 0)
		// 				lst->rd->fd[j] = open(lst->command_str[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
					
		// 			j++;
		// 		}
		// 		i++;
		// 	}
		// }
		lst = lst->next;
	}
	lst = head;
	return (lst);
}
int ft_process4(char **env, char *str)
{
    t_comm *lst;

    lst = malloc(sizeof(t_comm));
    if (!lst)
        return (-1);
    ft_memset((void *)lst, 0, sizeof(t_comm));
    lst = ft_parser4(lst, str, env);
	lst = ft_check_redir(lst);
	ft_free_list(lst);
    return (0);
}

void ft_no_malloc(char **str)
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
			if (ft_lexer(str) != 1)
				exit (0);
			envp = ft_get_envp(env);
			add_history(str);
			if (ft_process4(envp, str) == -1)
				exit (0);
			free (str);
			ft_no_malloc(envp);
		}
	}
}