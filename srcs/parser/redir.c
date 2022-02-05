#include "../../includes/minishell.h"

int ft_search_red(char *str)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (str[i])
    {
        if (str[i] == '>')
            count += 3;
        else
            count++;
        i++;
    }
    return (count);
}


char *ft_add_redir(char *tmp, int *j, char c, int *i)
{
    tmp[(*j)++] = ' ';
    tmp[(*j)++] = c;
    tmp[(*j)++] = ' ';
    (*i)++;
    return (tmp);
}

char *ft_add_two_redir(char *tmp, int *j, char c, int *i)
{
    tmp[(*j)++] = ' ';
    tmp[(*j)++] = c;
    tmp[(*j)++] = c;
    tmp[(*j)++] = ' ';
    (*i) += 2;
    return (tmp);
}
char *ft_add_space(char *str)
{
    int i;
    int j;
    int count;
    char *tmp;

    count = ft_search_red(str);
    tmp = (char *)malloc(sizeof(char) * count + 1);
    if (!tmp)
        return (NULL);
    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] == '>' && str[i + 1] != '>')
            tmp = ft_add_redir(tmp, &j, '>', &i);
        else if (str[i] == '>' && str[i + 1] == '>')
            tmp = ft_add_two_redir(tmp, &j, '>', &i);
        else if (str[i] == '<' && str[i + 1] != '<')
            tmp = ft_add_redir(tmp, &j, '<', &i);
        else if (str[i] == '<' && str[i + 1] == '<')
            tmp = ft_add_two_redir(tmp, &j, '<', &i);
        else
            tmp[j++] = str[i++];
    }
    tmp[j] = '\0';
    return (tmp);
}

t_comm *ft_check_redir(t_comm *lst)
{
	t_comm *head;
	char *redirect;
	char *redirect2;
	char *redirect3;

	head = lst;	
	redirect = ft_strdup(">"); // outfile
	redirect2 = ft_strdup("<"); // infile
	redirect3 = ft_strdup(">>");
	while (lst->next != NULL)
	{
		if (lst->command_str) 
		{
			int i = 0;
			while (lst->command_str[i])
			{
				if (ft_strncmp(lst->command_str[i], redirect, ft_strlen(redirect)) == 0)
				{
					lst->outfile = open(lst->command_str[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
					if (lst->outfile == -1)
						perror("Cant open file");
				}
				else if (ft_strncmp(lst->command_str[i], redirect2, ft_strlen(redirect2)) == 0)
				{
					lst->infile = open(lst->command_str[i + 1], O_RDONLY);
					if (lst->infile == -1)
						perror("Cant open file");
				}
				else if (ft_strncmp(lst->command_str[i], redirect, ft_strlen(redirect)) == 0)
				{
					lst->outfile = open(lst->command_str[i + 1], O_CREAT | O_WRONLY | O_TRUNC | O_APPEND, 0644);
					if (lst->outfile == -1)
						perror("Cant open file");
				}
                i++;
			}
		}
		lst = lst->next;
	}
	lst = head;
    free (redirect);
    free (redirect2);
    free (redirect3);
	return (lst);
}

