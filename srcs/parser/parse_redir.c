#include "../../includes/minishell.h"

char *ft_new_sub(int i, char *name, char *str, int begin)
{
    int k = 0;
    while (begin < i)
    {
        name[k] = str[begin];
        k++;
        begin++;
    }
    name[k] = '\0';
    return (name);
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

void    ft_one_redir(t_comm *lst, char *str, int *i, int *begin)
{
    ft_skip_sp(str, i, begin);
    lst->name = (char *)malloc(sizeof(char) * ((*i) - (*begin)) + 1);
    lst->name = ft_new_sub(*i, lst->name, str, *begin);
    lst->outfile = open(lst->name,  O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (lst->outfile == -1)
    {
        write(1, "bash: ", 6);
        write(2, lst->name, ft_strlen(lst->name));
        write(1, ":", 1);
        perror("");
    }
    free (lst->name);
}

void    ft_back_redir(t_comm *lst, char *str, int *i, int *begin)
{
    ft_skip_sp(str, i, begin);
    lst->name = (char *)malloc(sizeof(char) * ((*i) - (*begin)) + 1);
    lst->name = ft_new_sub(*i, lst->name, str, *begin);
    lst->infile = open(lst->name, O_RDONLY , 0644);
    if (lst->infile == -1)
    {
        write(1, "bash: ", 6);
        write(2, lst->name, ft_strlen(lst->name));
        write(1, ":", 1);
        perror("");
        exit (0);
    }
    free (lst->name);
}

void    ft_add_redir(t_comm *lst, char *str, int *i, int *begin)
{
    (*i)++;
    ft_skip_sp(str, i, begin);
    lst->name = (char *)malloc(sizeof(char) * ((*i) - (*begin)) + 1);
    lst->name = ft_new_sub(*i, lst->name, str, *begin);
    lst->outfile = open(lst->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (lst->outfile == -1)
    {
        write(1, "bash: ", 6);
        write(2, lst->name, ft_strlen(lst->name));
        write(1, ":", 1);
        perror("");
    }
    free (lst->name);
}

char *ft_open_file(char *str, char *tmp, int *i, int *j, t_comm *lst)
{
    int begin;
    int end;
    int k;

    end = 0;
    begin = 0;
    while (str[(*i)])
    {
        k = 0;
        if (str[(*i)] == '>' && str[(*i) + 1] != '>')
            ft_one_redir(lst, str, i, &begin);
        else if (str[(*i)] == '>' && str[(*i) + 1] == '>')
            ft_add_redir(lst, str, i, &begin);
        else if (str[(*i)] == '<' && str[(*i) + 1] != '<')
            ft_back_redir(lst, str, i, &begin);
        else
            tmp[(*j)++] = str[(*i)++];
    }
    return (tmp);
}