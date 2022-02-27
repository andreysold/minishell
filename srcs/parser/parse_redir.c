#include "../../includes/minishell.h"

char *ft_key_file(char *str, int *begin, int len, char *name)
{
	int k;

	k = 0;
	while ((*begin) < len)
	{
		if (str[(*begin)] == '^')
			str[(*begin)] -= 62;
		name[k] = str[(*begin)];
		k++;
		(*begin)++;
	}
	name[k] = '\0';
	return (name);
}

char *ft_name_file(t_comm *lst, int i, char *tmp, t_envp *head)
{
	while (lst->name[i])
	{
		if (lst->name[i] == '$')
		{
			i++;
			tmp = ft_substr(lst->name, i, ft_strlen(lst->name) - i);
			while (head)
			{
				if (ft_strncmp(head->key, tmp, ft_strlen(head->key)) == 0)
				{
					free (tmp);
					free (lst->name);
					lst->name = ft_strdup(head->value);
					printf("%s\n", lst->name);
					return (lst->name);
				}
				head = head->next;
			}
		}
		i++;
	}
	// printf("%s\n", lst->name);
	return (lst->name);
}

char *ft_new_sub(int i, t_comm *lst, char *str, int begin)
{
	t_envp *head;
	char *tmp;
	int fl;
	int count;

	fl = 0;
	count = 0;
	head = lst->e;
	tmp = ft_key_file(str, &begin, i, lst->name);
	return (ft_name_file(lst, i, tmp, head));
}

void    ft_skip_sp(char *str, int *i, int *begin)
{
	(*i)++;
	while (str[(*i)] == ' ')
		(*i)++;
	(*begin) = (*i);
	while (str[(*i)] && str[(*i)] != ' ' && str[(*i)] != '>' && str[(*i)] != '<')
		(*i)++;
}

int ft_one_redir(t_comm *lst, char *str, int *i, int *begin)
{
	ft_skip_sp(str, i, begin);
	lst->name = (char *)malloc(sizeof(char) * ((*i) - (*begin)) + 1);
	lst->name = ft_new_sub(*i, lst, str, *begin);
	printf("|%s|\n", lst->name);
	lst->outfile = open(lst->name,  O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (!lst->outfile)
		return (-1);
	if (lst->outfile == -1)
	{
		write(1, "bash: ", 6);
		write(2, lst->name, ft_strlen(lst->name));
		write(1, ":", 1);
		perror("");
	}
	free (lst->name);
	return (0);
}

int ft_back_redir(t_comm *lst, char *str, int *i, int *begin)
{
	ft_skip_sp(str, i, begin);
	lst->name = (char *)malloc(sizeof(char) * ((*i) - (*begin)) + 1);
	lst->name = ft_new_sub(*i, lst, str, *begin);
	lst->infile = open(lst->name, O_RDONLY , 0644);
	if (!lst->infile)
		return (-1);
	if (lst->infile == -1)
	{
		write(1, "bash: ", 6);
		write(2, lst->name, ft_strlen(lst->name));
		write(1, ":", 1);
		perror("");
		g_error_status = 1;
	}
	free (lst->name);
	return (0);
}

int ft_add_redir(t_comm *lst, char *str, int *i, int *begin)
{
	(*i)++;
	ft_skip_sp(str, i, begin);
	lst->name = (char *)malloc(sizeof(char) * ((*i) - (*begin) + 1));
	lst->name = ft_new_sub(*i, lst, str, *begin);
	lst->outfile = open(lst->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (!lst->outfile)
		return (-1);
	if (lst->outfile == -1)
	{
		write(1, "bash: ", 6);
		write(2, lst->name, ft_strlen(lst->name));
		write(1, ":", 1);
		perror("");
	}
	free (lst->name);
	return (0);
}

int    ft_herdok(t_comm *lst, char *str, int *i, int *begin)
{
	int beg;
	int k;

	k = 0;
	beg = 0;
	(*i) += 2;
	while (str[(*i)] && str[(*i)] == ' ')
		(*i)++;
	beg = (*i);
	while (str[(*i)] && str[(*i)] != ' ')
	{
		(*i)++;
		k++;
	}
	lst->here = ft_substr(str, beg, k);
	// printf("|%s|\n", lst->here);
	// (*i) += ft_strlen(lst->here);
	lst->infile = open(".tmp", O_TRUNC | O_WRONLY | O_CREAT, 0644);
	if (lst->infile == -1)
	{
		write(1, "bash: ", 6);
		write(2, lst->name, ft_strlen(lst->name));
		write(1, ":", 1);
		perror("");
	}
	return (0);
}

char *ft_open_file(char *str, int *i, int *j, t_comm *lst)
{
	int begin;
	// int end;
	// int k;
	
	// end = 0;
	// begin = 0;
	while (str[(*i)])
	{
		// k = 0;
		if (str[(*i)] == '>' && str[(*i) + 1] != '>')
			ft_one_redir(lst, str, i, &begin);
		else if (str[(*i)] == '>' && str[(*i) + 1] == '>')
			ft_add_redir(lst, str, i, &begin);
		else if (str[(*i)] == '<' && str[(*i) + 1] != '<')
			ft_back_redir(lst, str, i, &begin);
		else if (str[(*i)] == '<' && str[(*i) + 1] == '<')
			ft_herdok(lst, str, i, &begin);
		else
			lst->tmp[(*j)++] = str[(*i)++];
	}
	return (lst->tmp);
}