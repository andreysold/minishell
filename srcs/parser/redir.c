#include "../../includes/minishell.h"



int ft_check_str(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '>' && ft_isdigit(str[i + 1]))
        {
            i += 2;
            if (str[i] == '>')
                return (-1);
        }
        i++;
    }
    return (1);
}

int ft_search_red(char *str) 
{
    int i;
    int count;

    i = 0;
    count = 0;
    // if (ft_check_str(str) == -1)
    //     exit (0);
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
    t_redir *r;

    r = malloc(sizeof(t_redir));
    if (!r)
        return (NULL);
    r->tmp1 = ft_strdup(">");
    r->tmp2 = ft_strdup("<");
    r->tmp3 = ft_strdup(">>");
	head = lst;	
	while (lst->next != NULL)
	{
        lst->outfile = -2;
        lst->infile = -2;
		if (lst->command_str) 
		{
			int i = 0;
			while (lst->command_str[i])
			{
				if (ft_strncmp(lst->command_str[i], r->tmp1, ft_strlen(r->tmp1)) == 0)
				{
					lst->outfile = open(lst->command_str[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
					if (lst->outfile == -1)
						perror("Cant open file");
				}
				else if (ft_strncmp(lst->command_str[i], r->tmp2, ft_strlen(r->tmp2)) == 0)
				{
					lst->infile = open(lst->command_str[i + 1], O_RDONLY);
					if (lst->infile == -1) // ???? fixme
                    {
                        write(1, "bash: ", 6);
                        write(1, lst->command_str[i + 1], ft_strlen(lst->command_str[i + 1]));
                        write(1, ": ", 2);
						perror("");
                    }
				}
				else if (ft_strncmp(lst->command_str[i], r->tmp3, ft_strlen(r->tmp3)) == 0)
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
    free (r->tmp1);
    free (r->tmp2);
    free (r->tmp3);
    free (r);
	return (lst);
}

